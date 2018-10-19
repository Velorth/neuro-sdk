#ifndef GATT_WRAPPER_H
#define GATT_WRAPPER_H

#include <string>
#include <type_traits>
#include <sstream>

namespace Neuro {

template <typename T, std::enable_if_t<std::is_integral<T>::value, T>* Deduce = nullptr>
std::string to_hex_string(T value){
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
}

inline std::string to_narrow(const wchar_t *s, char def = '?', const std::locale& loc = std::locale()) {
	std::ostringstream stm;
	while (*s != L'\0') {
		stm << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, def);
	}
	return stm.str();
}


}
#endif // GATT_WRAPPER_H
