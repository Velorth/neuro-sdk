#ifndef SDK_ERROR_H
#define SDK_ERROR_H

#include "lib_export.h"

typedef int ret_code;

#define SDK_NO_ERROR 0
#define ERROR_UNHANDLED_EXCEPTION 1
#define ERROR_EXCEPTION_WITH_MESSAGE 2

SDK_SHARED int sdk_last_error_msg(char *msg_buffer, size_t length);

#endif // SDK_ERROR_H
