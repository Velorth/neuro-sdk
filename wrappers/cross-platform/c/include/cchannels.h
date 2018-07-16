#ifndef CCHANNELS_H
#define CCHANNELS_H

#include "lib_export.h"
#include "cdevice.h"

typedef struct _CommonChannelInterface CommonChannelInterface;
typedef struct _BatteryChannel BatteryChannel;
typedef struct _SignalChannel SignalChannel;
typedef void* ListenerHandle;

typedef enum _Filter {
	LowPass_1Hz_SF125,
	LowPass_1Hz_SF125_Reverse,
	LowPass_5Hz_SF125,
	LowPass_5Hz_SF125_Reverse,
	LowPass_15Hz_SF125,
	LowPass_15Hz_SF125_Reverse,
	LowPass_27Hz_SF125,
	LowPass_27Hz_SF125_Reverse,
	LowPass_30Hz_SF250,
	LowPass_30Hz_SF250_Reverse,

	HighPass_2Hz_SF250,
	HighPass_2Hz_SF250_Reverse,
	HighPass_3Hz_SF125,
	HighPass_3Hz_SF125_Reverse,
	HighPass_5Hz_SF125,
	HighPass_5Hz_SF125_Reverse,
	HighPass_11Hz_SF125,
	HighPass_11Hz_SF125_Reverse,

	BandStop_45_55Hz_SF250
} Filter;

SDK_SHARED void free_listener_handle(ListenerHandle *handle);

SDK_SHARED BatteryChannel* create_BatteryChannel(Device *device_ptr);
SDK_SHARED void BatteryChannel_delete(BatteryChannel *channel);
SDK_SHARED int BatteryChannel_read_data(BatteryChannel *channel, size_t offset, size_t length, int *out_buffer);
SDK_SHARED int BatteryChannel_get_sampling_frequency(BatteryChannel *channel, float * out_frequency);
SDK_SHARED int BatteryChannel_add_length_callback(BatteryChannel *channel, void(*callback)(BatteryChannel *, size_t), ListenerHandle *handle);
SDK_SHARED int BatteryChannel_get_total_length(BatteryChannel *channel, size_t *out_length);
SDK_SHARED int BatteryChannel_get_buffer_size(BatteryChannel *channel, size_t *out_buffer_size);

SDK_SHARED SignalChannel* create_SignalChannel(Device *device);
SDK_SHARED SignalChannel* create_SignalChannel_info(Device *device, ChannelInfo info);
SDK_SHARED SignalChannel* create_SignalChannel_info_filters(Device *device, ChannelInfo info, Filter *filters, size_t filter_count);
SDK_SHARED SignalChannel* create_SignalChannel_filters(Device *device, Filter *filters, size_t filter_count);
SDK_SHARED void SignalChannel_delete(SignalChannel *channel);
SDK_SHARED int SignalChannel_read_data(SignalChannel *channel, size_t offset, size_t length, double *out_buffer);
SDK_SHARED int SignalChannel_get_sampling_frequency(SignalChannel *channel, float * out_frequency);
SDK_SHARED int SignalChannel_add_length_callback(SignalChannel *channel, void(*callback)(SignalChannel *, size_t), ListenerHandle *handle);
SDK_SHARED int SignalChannel_get_total_length(SignalChannel *channel, size_t *out_length);
SDK_SHARED int SignalChannel_get_buffer_size(SignalChannel *channel, size_t *out_buffer_size);

#endif // CCHANNELS_H
