extern "C"
{
#include "sdk_error.h"
}
#include <string>

std::string sdk_last_error;

const char* sdk_last_error_msg() {
	return sdk_last_error.c_str();
}
