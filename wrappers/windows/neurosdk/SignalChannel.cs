using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class SignalChannel : BaseChannel<double>
    {
        private readonly IntPtr _listenerPtr;

        public SignalChannel(Device device) : base(device)
        {
            ChannelPtr = create_SignalChannel(device.DevicePtr);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(SignalChannel_add_length_callback(ChannelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            SdkError.ThrowIfError(SignalChannel_get_info(ChannelPtr, out var info));
            Info = info;
        }

        public SignalChannel(Device device, ChannelInfo info) : base(device)
        {
            ChannelPtr = create_SignalChannel_info(device.DevicePtr, info);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(SignalChannel_add_length_callback(ChannelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            Info = info;
        }

        public SignalChannel(Device device, ChannelInfo info, Filter[] filters) : base(device)
        {
            ChannelPtr = create_SignalChannel_info_filters(device.DevicePtr, info, filters, (IntPtr)filters.Length);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(SignalChannel_add_length_callback(ChannelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            Info = info;
        }

        public SignalChannel(Device device, Filter[] filters) : base(device)
        {
            ChannelPtr = create_SignalChannel_filters(device.DevicePtr, filters, (IntPtr)filters.Length);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(SignalChannel_add_length_callback(ChannelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            SdkError.ThrowIfError(SignalChannel_get_info(ChannelPtr, out var info));
            Info = info;
        }

        ~SignalChannel()
        {
            SignalChannel_delete(ChannelPtr);
            free_listener_handle(_listenerPtr);
        }

        public override event EventHandler<int> LengthChanged;
        public override ChannelInfo Info { get; set; }

        public override int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(SignalChannel_get_total_length(ChannelPtr, out var length));
                return (int)length;
            }
        }

        public override int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(SignalChannel_get_buffer_size(ChannelPtr, out var size));
                return (int)size;
            }
        }

        public override float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(SignalChannel_get_sampling_frequency(ChannelPtr, out var frequency));
                return frequency;
            }
            set => SdkError.ThrowIfError(SignalChannel_set_sampling_frequency(ChannelPtr, value));
        }

        public override double[] ReadData(int offset, int length)
        {
            if (length <= 0)
            {
                return new double[0];
            }

            var bufferPtr = Marshal.AllocHGlobal(length * sizeof(double));
            try
            {
                SdkError.ThrowIfError(
                    SignalChannel_read_data(ChannelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr));
                var buffer = new double[length];
                Marshal.Copy(bufferPtr, buffer, 0, length);
                return buffer;
            }
            finally
            {
                Marshal.FreeHGlobal(bufferPtr);
            }
        }

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif
        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_listener_handle(IntPtr signalChannelPtr);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr channelPtr, IntPtr length);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SignalChannel(IntPtr devicePtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SignalChannel_info(IntPtr devicePtr, ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SignalChannel_info_filters(IntPtr devicePtr, ChannelInfo info, Filter[] filtersArray, IntPtr filterCount);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SignalChannel_filters(IntPtr devicePtr, Filter[] filtersArray, IntPtr filterCount);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void SignalChannel_delete(IntPtr signalChannelPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalChannel_get_info(IntPtr signalChannelPtr, out ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalChannel_read_data(IntPtr signalChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalChannel_get_sampling_frequency(IntPtr signalChannelPtr, out float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalChannel_set_sampling_frequency(IntPtr signalChannelPtr, float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalChannel_add_length_callback(IntPtr signalChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalChannel_get_total_length(IntPtr signalChannelPtr, out IntPtr totalLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalChannel_get_buffer_size(IntPtr signalChannelPtr, out IntPtr bufferSize);
    }
}
