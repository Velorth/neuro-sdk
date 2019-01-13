#ifndef CCHANNELS_H
#define CCHANNELS_H

#include "lib_export.h"
#include "cdevice.h"
#include "clistener.h"
#include <stdbool.h>
#include <stddef.h>


typedef struct _AnyChannel AnyChannel;
typedef struct _IntChannel IntChannel;
typedef struct _DoubleChannel DoubleChannel;
typedef struct _BaseDoubleChannel BridgeDoubleChannel;
typedef struct _BatteryChannel BatteryIntChannel;
typedef struct _SignalChannel SignalDoubleChannel;
typedef struct _BipolarChannel BipolarDoubleChannel;
typedef struct _ResistanceChannel ResistanceDoubleChannel;
typedef struct _SpectrumChannel SpectrumDoubleChannel;

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

SDK_SHARED void AnyChannel_delete(AnyChannel *channel);
SDK_SHARED int AnyChannel_get_info(AnyChannel *channel, ChannelInfo *out_info);
SDK_SHARED int AnyChannel_get_sampling_frequency(AnyChannel *channel, float * out_frequency);
SDK_SHARED int AnyChannel_add_length_callback(AnyChannel *channel, void(*callback)(AnyChannel *, size_t), ListenerHandle *handle);
SDK_SHARED int AnyChannel_get_total_length(AnyChannel *channel, size_t *out_length);

SDK_SHARED int IntChannel_read_data(IntChannel *channel, size_t offset, size_t length, int *out_buffer);
SDK_SHARED int DoubleChannel_read_data(DoubleChannel *channel, size_t offset, size_t length, double *out_buffer);

SDK_SHARED BatteryIntChannel* create_BatteryIntChannel(Device *device_ptr);
SDK_SHARED int BatteryIntChannel_get_buffer_size(BatteryIntChannel *channel, size_t *out_buffer_size);

SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel(Device *device_ptr);
SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel_info(Device *device_ptr, ChannelInfo info);
SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel_info_filters(Device *device_ptr, ChannelInfo info, Filter *filters, size_t filter_count);
SDK_SHARED SignalDoubleChannel* create_SignalDoubleChannel_filters(Device *device_ptr, Filter *filters, size_t filter_count);
SDK_SHARED int SignalDoubleChannel_get_buffer_size(SignalDoubleChannel *channel, size_t *out_buffer_size);

SDK_SHARED BipolarDoubleChannel* create_BipolarDoubleChannel(BridgeDoubleChannel *first, BridgeDoubleChannel *second);

SDK_SHARED ResistanceDoubleChannel* create_ResistanceChannel_info(Device *device_ptr, ChannelInfo info);
SDK_SHARED int ResistanceChannel_get_buffer_size(ResistanceDoubleChannel *channel, size_t *out_buffer_size);

SDK_SHARED SpectrumDoubleChannel* create_SpectrumChannel(DoubleChannel *);
SDK_SHARED int SpectrumChannel_get_hz_per_spectrum_sample(SpectrumDoubleChannel* channel, double* out_step);
SDK_SHARED int SpectrumChannel_get_spectrum_length(SpectrumDoubleChannel *channel, size_t *out_spectrum_length);

typedef int(*ReadDataFunc)(size_t offset, size_t length, double *out_buffer);
typedef int(*GetFrequencyFunc)(float * out_frequency);
typedef int(*SetFrequencyFunc)(float frequency);
typedef int(*AddLengthCallbackFunc)(void(*callback)(BridgeDoubleChannel *, size_t), ListenerHandle *handle);
typedef int(*GetTotalLengthFunc)(size_t *out_length);
typedef int(*GetBufferSizeFunc)(size_t *out_buffer_size);
typedef Device*(*GetDeviceFunc)();

SDK_SHARED BridgeDoubleChannel* create_BridgeDoubleChannel_info(ChannelInfo, ReadDataFunc, GetFrequencyFunc, SetFrequencyFunc, AddLengthCallbackFunc, GetTotalLengthFunc, GetBufferSizeFunc, GetDeviceFunc);
SDK_SHARED void BridgeDoubleChannel_delete(BridgeDoubleChannel *channel);

#endif // CCHANNELS_H
