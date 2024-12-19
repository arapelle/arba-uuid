#include <algorithm>
#include <arba/hash/murmur_hash.hpp>
#include <arba/rand/rand.hpp>
#include <arba/uuid/uuid.hpp>
#include <iomanip>
#include <sstream>

inline namespace arba
{
namespace uuid
{
namespace detail
{
template <typename TChar>
constexpr inline unsigned char hex2char(TChar const ch)
{
    if (ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9'))
        return ch - static_cast<TChar>('0');
    if (ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f'))
        return 10 + ch - static_cast<TChar>('a');
    if (ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F'))
        return 10 + ch - static_cast<TChar>('A');
    return 0;
}

template <typename TChar>
constexpr inline bool is_hex(TChar const ch)
{
    return (ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9'))
           || (ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f'))
           || (ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F'));
}

template <typename TChar>
constexpr inline unsigned char hexpair2char(TChar const a, TChar const b)
{
    return (hex2char(a) << 4) | hex2char(b);
}
}

uuid::uuid() : data_{ 0 }
{
}

uuid::uuid(const std::string_view& str)
{
    char digit = 0;
    bool firstDigit = true;
    int hasBraces = 0;
    size_t index = 0;
    size_t size = str.size();

    std::array<uint8_t, 16> data{ { 0 } };

    if (str.empty())
        return;

    if (str[0] == '{')
        hasBraces = 1;
    if (hasBraces && str[size - 1] != '}')
        return;

    for (size_t i = hasBraces; i < size - hasBraces; ++i)
    {
        if (str[i] == '-')
        {
            if (firstDigit)
                continue;
            else
                return;
        }

        if (index >= 16 || !detail::is_hex(str[i]))
            return;

        if (firstDigit)
        {
            digit = str[i];
            firstDigit = false;
        }
        else
        {
            data[index++] = detail::hexpair2char(digit, str[i]);
            firstDigit = true;
        }
    }

    if (index < data.size())
        return;

    data_ = data;
}

bool uuid::is_nil() const
{
    return std::count(data_.cbegin(), data_.cend(), 0) == 16;
}

std::string uuid::to_string() const
{
    std::ostringstream stream;
    stream << *this;
    return stream.str();
}

uuid uuid::make_random_uuid()
{
    return make_random_uuid(static_cast<uint64_t (&)()>(rand::rand_u64));
}

std::ostream& operator<<(std::ostream& stream, const uuid& uuid)
{
    return stream << std::hex << std::setfill('0')
                  << std::setw(2) << (int)uuid.data_[0] << std::setw(2) << (int)uuid.data_[1]
                  << std::setw(2) << (int)uuid.data_[2] << std::setw(2) << (int)uuid.data_[3]
                  << '-' << std::setw(2) << (int)uuid.data_[4] << std::setw(2) << (int)uuid.data_[5]
                  << '-' << std::setw(2) << (int)uuid.data_[6] << std::setw(2) << (int)uuid.data_[7]
                  << '-' << std::setw(2) << (int)uuid.data_[8] << std::setw(2) << (int)uuid.data_[9]
                  << '-' << std::setw(2) << (int)uuid.data_[10] << std::setw(2) << (int)uuid.data_[11]
                  << std::setw(2) << (int)uuid.data_[12] << std::setw(2) << (int)uuid.data_[13]
                  << std::setw(2) << (int)uuid.data_[14] << std::setw(2) << (int)uuid.data_[15];
}

}
}

namespace std
{

std::size_t hash<::arba::uuid::uuid>::operator()(const ::arba::uuid::uuid& uuid) const
{
    uint64_t hash = ::arba::hash::murmur_hash_64(&uuid.data().front(), uuid.data().size());
    return static_cast<std::size_t>(hash);
}

}
