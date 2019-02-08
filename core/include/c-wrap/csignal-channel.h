#ifndef CSIGNAL_CHANNEL_H
#define CSIGNAL_CHANNEL_H

#include "cchannels.h"

typedef struct _SignalDoubleChannel SignalDoubleChannel;

SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel(Device *device_ptr);
SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel_info(Device *device_ptr, ChannelInfo info);
SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel_info_filters(Device *device_ptr, ChannelInfo info, Filter *filters, size_t filter_count);
SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel_filters(Device *device_ptr, Filter *filters, size_t filter_count);
SDK_SHARED int SignalDoubleChannel_get_buffer_size(SignalDoubleChannel *channel, size_t *out_buffer_size);

#endif // CSIGNAL_CHANNEL_H
