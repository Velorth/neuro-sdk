#ifndef SATURATION_CAST_H
#define SATURATION_CAST_H

#include <type_traits>
#include <limits>

template <typename Dst, typename Src>
inline typename std::enable_if<std::is_signed<Dst>::value && std::is_signed<Src>::value && (sizeof(Dst) < sizeof(Src)), Dst>::type
saturation_cast(Src value) {

    if (value > std::numeric_limits<Dst>::max())
        return std::numeric_limits<Dst>::max();
    else if (value < std::numeric_limits<Dst>::min())
        return std::numeric_limits<Dst>::min();
    else
        return static_cast<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<std::is_signed<Dst>::value && std::is_signed<Src>::value && (sizeof(Dst) >= sizeof(Src)), Dst>::type
saturation_cast(Src value) {

        return static_cast<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<std::is_unsigned<Dst>::value && std::is_unsigned<Src>::value && (sizeof(Dst) < sizeof(Src)), Dst>::type
saturation_cast(Src value) {
    if (value > std::numeric_limits<Dst>::max())
        return std::numeric_limits<Dst>::max();
    else
        return static_cast<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<std::is_unsigned<Dst>::value && std::is_unsigned<Src>::value && (sizeof(Dst) >= sizeof(Src)), Dst>::type
saturation_cast(Src value) {

        return static_cast<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<(std::numeric_limits<Dst>::max() < std::numeric_limits<Src>::max()), Dst>::type
saturation_cast_signed_to_unsigned(Src value){
    if (value > static_cast<Src>(std::numeric_limits<Dst>::max()))
            return std::numeric_limits<Dst>::max();
        else
            return static_cast<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<(std::numeric_limits<Dst>::max() >= std::numeric_limits<Src>::max()), Dst>::type
saturation_cast_signed_to_unsigned(Src value){

            return static_cast<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<std::is_unsigned<Dst>::value && std::is_signed<Src>::value, Dst>::type
saturation_cast(Src value) {
    if (value < 0)
        return 0;
    else
        return saturation_cast_signed_to_unsigned<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<std::is_signed<Dst>::value && std::is_unsigned<Src>::value && (std::numeric_limits<Dst>::max() < std::numeric_limits<Src>::max()) , Dst>::type
saturation_cast(Src value) {
    if (value > static_cast<Src>(std::numeric_limits<Dst>::max()))
            return std::numeric_limits<Dst>::max();
        else
            return static_cast<Dst>(value);
}

template <typename Dst, typename Src>
inline typename std::enable_if<std::is_signed<Dst>::value && std::is_unsigned<Src>::value && (std::numeric_limits<Dst>::max() >= std::numeric_limits<Src>::max()) , Dst>::type
saturation_cast(Src value) {

            return static_cast<Dst>(value);
}

#endif //SATURATION_CAST_H
