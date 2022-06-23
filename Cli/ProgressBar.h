#pragma once
#include <Cli/Terminal.h>
#include <Core/Thread.h>
#include <JR/ErrorOr.h>
#include <JR/Keywords.h>
#include <JR/MultiProducerQueue.h>
#include <JR/Queue.h>
#include <JR/StringView.h>
#include <JR/Types.h>
#include <JR/Vector.h>
#include <stdio.h>
#include <utility>

namespace Cli {

class SpecificProgressBar {
public:
    constexpr SpecificProgressBar(c_string message, u32 progress_to)
        : m_message(message)
        , m_current_progress(0)
        , m_progress_to(progress_to)
        , m_has_progress(true)
    {
    }

    constexpr SpecificProgressBar(c_string message)
        : m_message(message)
        , m_has_progress(false)
    {
    }

    constexpr void reset(c_string message, u32 progress_to)
    {
        *this = SpecificProgressBar(message, progress_to);
    }

    constexpr void reset(c_string message)
    {
        *this = SpecificProgressBar(message);
    }

    constexpr ErrorOr<void> increment_progress()
    {
        m_current_progress++;
        if (m_current_progress > m_progress_to) {
            return Error::from_string_literal(
                "tried to increment past end of progress bar");
        }
        return {};
    }

    constexpr void update(FILE* progress_file)
    {
        fprintf(progress_file, "%lc %s",
            next_spinner(),
            m_message);
        if (m_has_progress) {
            fprintf(progress_file, " [%d/%d]",
                m_current_progress,
                m_progress_to);
        }
    }

    constexpr bool is_done() const { return m_current_progress == m_progress_to; }

private:
    constexpr char16_t next_spinner()
    {
        // ⠀⠁⠂⠃⠄⠅⠆⠇⠈⠉⠊⠋⠌⠍⠎⠏⠐⠑⠒⠓⠔⠕⠖⠗⠘⠙⠚⠛⠜⠝⠞⠟⠠⠡⠢⠣⠤⠥⠦⠧⠨⠩⠪⠫⠬⠭⠮⠯⠰⠱⠲⠳⠴⠵⠶⠷⠸⠹⠺⠻⠼⠽⠾⠿
        if (is_done())
            return L'⠿';
        char16_t spinners[] = {
            L'⣷',
            L'⣯',
            L'⣟',
            L'⡿',
            L'⢿',
            L'⣻',
            L'⣽',
            L'⣾',
        };
        let spinners_size = sizeof(spinners) / sizeof(spinners[0]);
        m_current_spinner = (m_current_spinner + 1) % spinners_size;
        return spinners[m_current_spinner];
    }

    c_string m_message { nullptr };
    u32 m_current_progress { 0 };
    u32 m_progress_to { 0 };
    u8 m_current_spinner { 0 };
    bool m_has_progress { false };
};

class ProgressBars {
public:
    struct ProgressBar {
        constexpr ErrorOr<void> request_update()
        {
            TRY(progress_bars->request_update_for(progress_bar_id));
            return {};
        }

        constexpr ErrorOr<void> request_increment()
        {
            TRY(progress_bars->request_increment_for(progress_bar_id));
            return {};
        }

        constexpr void reset(c_string message, u32 progress_to)
        {
            progress_bars->reset_bar_at(progress_bar_id,
                message, progress_to);
        }

        constexpr void reset(c_string message)
        {
            progress_bars->reset_bar_at(progress_bar_id, message);
        }

        ProgressBars* progress_bars;
        ID<SpecificProgressBar> progress_bar_id;
    };

    static ErrorOr<ProgressBars> create(FILE* progress_file = stdout)
    {
        setlocale(LC_CTYPE, "");
        return ProgressBars {
            TRY(Vector<SpecificProgressBar>::create()),
            TRY(MultiProducerQueue<ID<SpecificProgressBar>>::create()),
            TRY(MultiProducerQueue<ID<SpecificProgressBar>>::create()),
            progress_file
        };
    }

    constexpr void destroy() const
    {
        if (is_valid()) {
            m_bars.destroy();
            m_bars_to_increment.destroy();
            m_bars_to_update.destroy();
        }
    }

    constexpr bool is_valid() const
    {
        return m_bars.is_valid() && m_bars_to_increment.is_valid() && m_bars_to_update.is_valid();
    }

    constexpr ErrorOr<ProgressBar> create_bar(c_string message, u32 progress_to)
    {
        var bar = ProgressBar {
            this,
            TRY(m_bars.append({ message, progress_to }))
        };
        TRY(bar.request_update());
        return bar;
    }

    constexpr ErrorOr<ProgressBar> create_bar(c_string message)
    {
        var bar = ProgressBar {
            this,
            TRY(m_bars.append({ message }))
        };
        TRY(bar.request_update());
        return bar;
    }

    constexpr ErrorOr<void> request_update_for(ID<SpecificProgressBar> id)
    {
        TRY(m_bars_to_update.enqueue(id));
        return {};
    }

    constexpr ErrorOr<void> request_increment_for(ID<SpecificProgressBar> id)
    {
        TRY(m_bars_to_increment.enqueue(id));
        return {};
    }

    constexpr void reset_bar_at(ID<SpecificProgressBar> id, c_string message,
        u32 progress_to)
    {
        m_bars[id].reset(message, progress_to);
    }

    constexpr void reset_bar_at(ID<SpecificProgressBar> id, c_string message)
    {
        m_bars[id].reset(message);
    }

    ErrorOr<void> async_start()
    {
        // Cli::Terminal::save_screen(m_progress_file);
        // Cli::Terminal::clear_screen(m_progress_file);
        m_update_thread = TRY(Core::Thread::create([this] {
            update_loop();
        }));
        m_update_thread->set_priority(Core::ThreadPriority::Low);
        m_update_thread->start();
        return {};
    }

    constexpr ErrorOr<void> wait_until_done()
    {
        if (!m_update_thread.has_value())
            return Error::from_string_literal("async_start was never called");
        while (!is_done()) { }
        // Cli::Terminal::clear_screen(m_progress_file);
        // Cli::Terminal::restore_screen(m_progress_file);
        return {};
    }

private:
    constexpr void update_loop()
    {
        while (!is_done()) {
            if (!m_bars_to_increment.is_empty()) {
                let bar = m_bars_to_increment.unchecked_dequeue();
                (void)increment_progress_for(bar); // FIXME
            }
            if (!m_bars_to_update.is_empty()) {
                let bar = m_bars_to_update.unchecked_dequeue();
                update(bar);
            }
            usleep(100000);
        }
    }

    constexpr bool is_done() const
    {
        for (let& bar : m_bars) {
            if (!bar.is_done())
                return false;
        }
        return true;
    }

    constexpr ErrorOr<void> increment_progress_for(ID<SpecificProgressBar> id)
    {
        TRY(m_bars[id].increment_progress());
        return {};
    }

    constexpr void update(ID<SpecificProgressBar> id)
    {
        Terminal::hide_cursor(m_progress_file);
        Terminal::move_cursor_to_start_of_line(id.raw_id() + 1, m_progress_file);
        Terminal::clear_current_line(m_progress_file);
        m_bars[id].update(m_progress_file);
        Terminal::show_cursor(m_progress_file);
    }

    constexpr ProgressBars(Vector<SpecificProgressBar> bars,
        MultiProducerQueue<ID<SpecificProgressBar>>&& bars_to_update,
        MultiProducerQueue<ID<SpecificProgressBar>>&& bars_to_increment,
        FILE* progress_file)
        : m_bars(bars)
        , m_bars_to_update(std::move(bars_to_update)) // FIXME
        , m_bars_to_increment(std::move(bars_to_increment)) // FIXME
        , m_progress_file(progress_file)
    {
    }

    Vector<SpecificProgressBar> m_bars;
    MultiProducerQueue<ID<SpecificProgressBar>> m_bars_to_update;
    MultiProducerQueue<ID<SpecificProgressBar>> m_bars_to_increment;
    FILE* m_progress_file { nullptr };
    Maybe<Core::Thread> m_update_thread {};
};
using ProgressBar = ProgressBars::ProgressBar;

}
