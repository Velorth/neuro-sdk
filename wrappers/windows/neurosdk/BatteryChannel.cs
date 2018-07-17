using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class BatteryChannel : BaseChannel<int>
    {
        private readonly IntPtr _channelPtr;
        private readonly IntPtr _listenerPtr;

        public BatteryChannel(Device device) : base(device)
        {
            _channelPtr = create_BatteryChannel(device.DevicePtr);
            if (_channelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(BatteryChannel_add_length_callback(_channelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out var listener));
            _listenerPtr = listener;
            SdkError.ThrowIfError(BatteryChannel_get_info(_channelPtr, out var info));
            Info = info;
        }

        ~BatteryChannel()
        {
            BatteryChannel_delete(_channelPtr);
            free_listener_handle(_listenerPtr);
        }

        public override event EventHandler<int> LengthChanged;
        public sealed override ChannelInfo Info { get; set; }

        public override int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(BatteryChannel_get_total_length(_channelPtr, out var length));
                return (int)length;
            }
        }

        public override int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(BatteryChannel_get_buffer_size(_channelPtr, out var size));
                return (int) size;
            }
        }

        public override float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(BatteryChannel_get_sampling_frequency(_channelPtr, out var frequency));
                return frequency;
            }
            set => SdkError.ThrowIfError(BatteryChannel_set_sampling_frequency(_channelPtr, value));
        }

        public override int[] ReadData(int offset, int length)
        {
            if (length <= 0)
            {
                return new int[0];
            }
            
            var bufferPtr = Marshal.AllocHGlobal(length * sizeof(int));
            try
            {
                SdkError.ThrowIfError(
                    BatteryChannel_read_data(_channelPtr, (IntPtr) offset, (IntPtr) length, bufferPtr));
                var buffer = new int[length];
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
        private static extern void free_listener_handle(IntPtr batteryChannelPtr);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr channelPtr, IntPtr length);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_BatteryChannel(IntPtr devicePtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void BatteryChannel_delete(IntPtr batteryChannelPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_get_info(IntPtr batteryChannelPtr, out ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_read_data(IntPtr batteryChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_get_sampling_frequency(IntPtr batteryChannelPtr, out float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_set_sampling_frequency(IntPtr batteryChannelPtr, float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_add_length_callback(IntPtr batteryChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_get_total_length(IntPtr batteryChannelPtr, out IntPtr totalLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_get_buffer_size(IntPtr batteryChannelPtr, out IntPtr bufferSize);
    }
}
