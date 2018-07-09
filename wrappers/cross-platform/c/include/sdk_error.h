#ifndef SDK_ERROR_H
#define SDK_ERROR_H

typedef int ret_code;

#define SDK_NO_ERROR 0
#define ERROR_UNHANDLED_EXCEPTION 1
#define ERROR_EXCEPTION_WITH_MESSAGE 2

const char *sdk_last_error_msg();

#endif // SDK_ERROR_H
