#ifndef FILTER_HPP
#define FILTER_HPP

#include <QVariant>


class Filter
{
public:
    // Enum
    enum class FilterType {
        LOCATIONS,
        LOCATIONS_CONTAIN,
        LOCATIONS_SET,
        MATERIALS,
        MATERIALS_CONTAIN,
        MATERIALS_SET,
        TIME,
        LOCATIONS_MAX,
        MATERIALS_MAX,
    };

    // Constructor
    explicit Filter(FilterType type, const QVariant &value);
    Filter(const Filter &other);

    bool operator==(const Filter &other) const;
    // bool operator<(const Filter &other) const;

    FilterType type() const;
    QVariant value() const;
private:
    FilterType m_type;
    QVariant m_value;
};

#endif // FILTER_HPP
