#pragma once
#include <Cli/ProgressBar.h>
#include <Core/Thread.h>
#include <Core/Threads.h>
#include <JR/Queue.h>
#include <JR/ErrorOr.h>
#include <JR/Function.h>
#include <JR/Keywords.h>
#include <JR/Maybe.h>
#include <JR/Queue.h>
#include <JR/Vector.h>

namespace Core {

using Job = Function<void()>;

class Worker {
public:
    bool is_free() const { return !m_job.has_value(); }
    void give_job(Job&& job) { m_job = std::move(job); }

    void request_finish()
    {
        m_is_done = true;
    }

    ErrorOr<void> start()
    {
        m_thread = TRY(Core::Thread::create([this] {
            do_work();
        }));
        m_thread->set_priority(Core::ThreadPriority::RealTime);
        m_thread->start();
        return {};
    }

private:
    void do_work()
    {
        while (!is_done()) {
            if (m_job.has_value()) {
                m_job.value()();
                m_job.reset();
            }
        }
    }

    bool is_done() const { return m_is_done; }

    Maybe<Core::Thread> m_thread;
    Maybe<Job> m_job;
    bool m_is_done { false };
};

class Workers {
public:
    static ErrorOr<Workers> create(u16 number_of_workers)
    {
        var workers = TRY(Vector<Worker>::create());
        for (u16 i = 0; i < number_of_workers; i++)
            TRY(workers.append({}));
        return Workers {
            TRY(Queue<Job>::create()),
            std::move(workers)
        };
    }

    constexpr void destroy() const
    {
        // FIXME: Add the rest of the members.
        if (is_valid()) {
            m_workers.destroy();
            m_jobs.destroy();
        }
    }

    constexpr bool is_valid() const
    {
        return m_workers.is_valid() && m_jobs.is_valid();
    }

    static ErrorOr<Workers> create_default_amount()
    {
        return create(Core::Threads::in_machine());
    }

    ErrorOr<void> add_job(Job&& job)
    {
        TRY(m_jobs.enqueue(std::move(job)));
        return {};
    }

    ErrorOr<void> async_start()
    {
        for (var& worker : m_workers)
            TRY(worker.start());

        m_work_scheduler = TRY(Core::Thread::create([&] {
            while (!is_done()) {
                if (!m_jobs.is_empty()) {
                    let worker_id = find_free_worker();
                    if (!worker_id.has_value()) {
                        usleep(1000);
                        continue;
                    }
                    var job = m_jobs.unchecked_dequeue();
                    var& worker = m_workers[*worker_id];
                    worker.give_job(std::move(job));
                }
            }
            block_until_workers_are_done();
        }));
        m_work_scheduler->set_priority(Core::ThreadPriority::Low);
        m_work_scheduler->start();
        return {};
    }

    ErrorOr<void> wait_until_done_and_release_error_if_any()
    {
        block_until_workers_are_done();
        request_finish();
        return m_work_result;
    }

private:
    void set_error_if_any(ErrorOr<void> result)
    {
        if (result.is_error()) {
            m_work_result = result;
        }
    }

    void request_finish()
    {
        for (var& worker : m_workers)
            worker.request_finish();
        m_is_done = true;
    }

    bool is_done() const { return m_is_done; }

    Workers(Queue<Job>&& jobs,
        Vector<Worker>&& workers)
        : m_jobs(std::move(jobs))
        , m_workers(std::move(workers))
    {
    }

    void block_until_workers_are_done()
    {
        for (let& worker : m_workers)
            while (!worker.is_free()) { }
    }

    Maybe<ID<Worker>> find_free_worker()
    {
        for (u32 i = 0; i < m_workers.size(); i++) {
            if (m_workers[i].is_free())
                return ID<Worker>(i);
        }
        return {};
    }

    Queue<Job> m_jobs;
    Vector<Worker> m_workers;
    Maybe<Core::Thread> m_work_scheduler {};
    ErrorOr<void> m_work_result {};
    bool m_is_done { false };
};

}
