#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <format>

inline namespace arba
{
namespace uuid
{
/**
 * @brief The uuid class
 *
 * Implementation is taken from stduuid: https://github.com/mariusbancila/stduuid/blob/master/include/uuid.h
 */
class uuid
{
public:
    uuid();
    uuid(const std::string_view& str);
    bool is_nil() const;
    std::string to_string() const;
    inline const std::array<uint8_t, 16>& data() const { return data_; }
    inline std::array<uint8_t, 16>& data() { return data_; }

    template <class UrngU64T>
        requires std::is_unsigned_v<std::invoke_result_t<UrngU64T>>
                 && std::is_integral_v<std::invoke_result_t<UrngU64T>>
                 && (sizeof(std::invoke_result_t<UrngU64T>) == sizeof(uint64_t))
    static uuid make_random_uuid(UrngU64T& rng)
    {
        uuid id;
        uint64_t uval = rng();
        auto iter = id.data().begin();
        for (unsigned i = 0; i < 8; ++i, ++iter)
            *iter = static_cast<uint64_t>((uval >> (i * 8)) & 0xFF);
        uval = rng();
        for (unsigned i = 0; i < 8; ++i, ++iter)
            *iter = static_cast<uint64_t>((uval >> (i * 8)) & 0xFF);
        //  https://www.rfc-editor.org/rfc/rfc4122#section-4.4
        // set variant: must be 0b10xxxxxx
        id.data()[8] &= 0xBF;
        id.data()[8] |= 0x80;
        // set version: must be 0b0100xxxx
        id.data()[6] &= 0x4F; // 0b01001111
        id.data()[6] |= 0x40; // 0b01000000
        return id;
    }

    template <class UrngU64T>
        requires std::is_unsigned_v<std::invoke_result_t<UrngU64T>>
                 && std::is_integral_v<std::invoke_result_t<UrngU64T>>
                 && (sizeof(std::invoke_result_t<UrngU64T>) == sizeof(uint64_t))
    static uuid make_random_uuid(UrngU64T&& rng)
    {
        UrngU64T tmp_rng(std::move(rng));
        return make_random_uuid(tmp_rng);
    }

    static uuid make_random_uuid();

    auto operator<=>(const uuid&) const = default;
    friend std::ostream& operator<<(std::ostream& stream, const uuid& uuid);

private:
    std::array<uint8_t, 16> data_;
};

}
}

template <>
struct std::hash<::arba::uuid::uuid>
{
    std::size_t operator()(const ::arba::uuid::uuid& uuid) const;
};

template <class CharT>
struct std::formatter<::arba::uuid::uuid, CharT>
{
    template <class FormatParseContext>
    constexpr auto parse(FormatParseContext& ctx)
    {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const ::arba::uuid::uuid& id, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}", id.to_string());
    }

private:
    bool pr_{ true };
    bool bm_{ true };
};
