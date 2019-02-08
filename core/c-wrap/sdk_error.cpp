extern "C"
{
#include "sdk_error.h"
}
#include <string>

std::string sdk_last_error;

void set_sdk_last_error(const char *msg) {
	sdk_last_error = msg;
}

int sdk_last_error_msg(char *msg_buffer, size_t length) {
	if (length < sdk_last_error.size()) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
	strcpy(msg_buffer, sdk_last_error.c_str());
	return SDK_NO_ERROR;
}
