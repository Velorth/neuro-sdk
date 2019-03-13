#ifndef MAKE_ARRAY_H
#define MAKE_ARRAY_H

namespace detail
{

    template <typename T, std::size_t...Is>
    std::array<T, sizeof...(Is)> make_array(const T& value, std::index_sequence<Is...>)
    {
        return {{(static_cast<void>(Is), value)...}};
    }
}

template <typename T, std::size_t N>
std::array<T, N> make_array(const T& value)
{
    return detail::make_array(value, std::make_index_sequence<N>());
}

#endif // MAKE_ARRAY_H
