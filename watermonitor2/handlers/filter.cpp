#include "filter.hpp"

Filter::Filter(FilterType type, const QVariant &value)
    : m_type(type), m_value(value)
{
}

Filter::Filter(const Filter &other)
    : m_type(other.type()), m_value(other.value())
{
}

Filter::FilterType Filter::type() const
{
    return m_type;
}

QVariant Filter::value() const
{
    return m_value;
}

bool Filter::operator==(const Filter &other) const
{
    return m_type == other.type() && m_value == other.value();
}

// bool Filter::operator<(const Filter &other) const
// {
//     return m_type < other.type() || (m_type == other.type() && m_value < other.value());
// }
