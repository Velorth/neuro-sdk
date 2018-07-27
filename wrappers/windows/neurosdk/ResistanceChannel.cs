using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class ResistanceChannel : BaseChannel<double>
    {
        private readonly IntPtr _channelPtr;
        private readonly IntPtr _listenerPtr;

        public ResistanceChannel(Device device) : base(device)
        {
            _channelPtr = create_ResistanceChannel(device.DevicePtr);
            if (_channelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(ResistanceChannel_add_length_callback(_channelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            SdkError.ThrowIfError(ResistanceChannel_get_info(_channelPtr, out var info));
            Info = info;
        }

        public ResistanceChannel(Device device, ChannelInfo info) : base(device)
        {
            _channelPtr = create_ResistanceChannel_info(device.DevicePtr, info);
            if (_channelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(ResistanceChannel_add_length_callback(_channelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            Info = info;
        }

        ~ResistanceChannel()
        {
            ResistanceChannel_delete(_channelPtr);
            free_listener_handle(_listenerPtr);
        }

        public override event EventHandler<int> LengthChanged;
        public sealed override ChannelInfo Info { get; set; }

        public override int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(ResistanceChannel_get_total_length(_channelPtr, out var length));
                return (int)length;
            }
        }

        public override int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(ResistanceChannel_get_buffer_size(_channelPtr, out var size));
                return (int)size;
            }
        }

        public override float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(ResistanceChannel_get_sampling_frequency(_channelPtr, out var frequency));
                return frequency;
            }
            set => SdkError.ThrowIfError(ResistanceChannel_set_sampling_frequency(_channelPtr, value));
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
                    ResistanceChannel_read_data(_channelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr));
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
        private static extern void free_listener_handle(IntPtr resistanceChannelPtr);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr channelPtr, IntPtr length);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_ResistanceChannel(IntPtr devicePtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_ResistanceChannel_info(IntPtr devicePtr, ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_ResistanceChannel_info_filters(IntPtr devicePtr, ChannelInfo info, Filter[] filtersArray, IntPtr filterCount);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_ResistanceChannel_filters(IntPtr devicePtr, Filter[] filtersArray, IntPtr filterCount);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ResistanceChannel_delete(IntPtr resistanceChannelPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_info(IntPtr resistanceChannelPtr, out ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_read_data(IntPtr resistanceChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_sampling_frequency(IntPtr resistanceChannelPtr, out float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_set_sampling_frequency(IntPtr resistanceChannelPtr, float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_add_length_callback(IntPtr resistanceChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_total_length(IntPtr resistanceChannelPtr, out IntPtr totalLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_buffer_size(IntPtr resistanceChannelPtr, out IntPtr bufferSize);
    }
}
