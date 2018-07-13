#ifndef CCHANNELS_H
#define CCHANNELS_H

#include "lib_export.h"
#include "cdevice.h"

typedef struct _CommonChannelInterface CommonChannelInterface;
typedef struct _BatteryChannel BatteryChannel;
typedef struct _SignalChannel SignalChannel;
typedef void* ListenerHandle;

SDK_SHARED int free_listener_handle(ListenerHandle *handle);

SDK_SHARED BatteryChannel* create_BatteryChannel(Device *device);
SDK_SHARED void BatteryChannel_delete(BatteryChannel *channel);
SDK_SHARED int BatteryChannel_read_data(BatteryChannel *channel, size_t offset, size_t length, double *out_buffer);
SDK_SHARED int BatteryChannel_get_sampling_frequency(BatteryChannel *channel, float *frequency);
SDK_SHARED int BatteryChannel_add_length_callback(BatteryChannel *channel, void(*callback)(size_t), ListenerHandle *handle);
SDK_SHARED int BatteryChannel_get_total_length(BatteryChannel *channel, size_t *out_length);
SDK_SHARED int BatteryChannel_get_buffer_size(BatteryChannel *channel, size_t *out_buffer_size);

SDK_SHARED SignalChannel* create_SignalChannel(Device *device);
SDK_SHARED void SignalChannel_delete(SignalChannel *channel);
SDK_SHARED int SignalChannel_read_data(SignalChannel *channel, size_t offset, size_t length, double *out_buffer);
SDK_SHARED int SignalChannel_get_sampling_frequency(SignalChannel *channel, float *frequency);
SDK_SHARED int SignalChannel_add_length_callback(SignalChannel *channel, void(*callback)(size_t), ListenerHandle *handle);
SDK_SHARED int SignalChannel_get_total_length(SignalChannel *channel, size_t *out_length);
SDK_SHARED int SignalChannel_get_buffer_size(SignalChannel *channel, size_t *out_buffer_size);

#endif // CCHANNELS_H
