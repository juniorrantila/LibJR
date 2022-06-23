#include <JR/ErrorOr.h>
#include <JR/SingleSplit.h>
#include <JR/StringView.h>
#include <stdio.h>
#include <JR/String.h>

ErrorOr<char*> StringView::to_allocated_c_string() const
{
    char* value = nullptr;
    let rc = asprintf(&value, "%.*s", m_size, m_data);
    if (rc == -1)
        return Error::allocation_failure();
    return value;
}

ErrorOr<String> StringView::to_string() const
{
    return String::create_from(*this);
}

ErrorOr<SingleSplit> StringView::split_on_first(char character) const
{
    for (u32 i = 0; i < size(); i++) {
        if (m_data[i] == character) {
            var first = split_view(0, i);
            var other = split_view(i, size());
            return SingleSplit { first, other };
        }
    }
    return Error::from_string_literal(
        "could not find any instances of character");
}

template<>
ErrorOr<i8> StringView::as_number<i8>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<i16> StringView::as_number<i16>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<i32> StringView::as_number<i32>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<i64> StringView::as_number<i64>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<u8> StringView::as_number<u8>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<u16> StringView::as_number<u16>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<u32> StringView::as_number<u32>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<u64> StringView::as_number<u64>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<f32> StringView::as_number<f32>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<f64> StringView::as_number<f64>() const
{
    return Error::unimplemented();
}

template<>
ErrorOr<f128> StringView::as_number<f128>() const
{
    return Error::unimplemented();
}
