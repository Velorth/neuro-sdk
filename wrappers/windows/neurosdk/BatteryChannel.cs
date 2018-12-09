using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class BatteryChannel : IBaseChannel<int>
    {
        private readonly IntPtr _listenerPtr;
        private readonly LengthChangedFunc _lengthChangedFunc;

        public BatteryChannel(Device device)
        {
            ChannelPtr = create_BatteryChannel(device.DevicePtr);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }

            _lengthChangedFunc = OnTotalLengthChanged;
            SdkError.ThrowIfError(BatteryChannel_add_length_callback(ChannelPtr, _lengthChangedFunc, out var listener));
            _listenerPtr = listener;
            SdkError.ThrowIfError(BatteryChannel_get_info(ChannelPtr, out var info));
            Info = info;
        }

        ~BatteryChannel()
        {
            BatteryChannel_delete(ChannelPtr);
            free_listener_handle(_listenerPtr);
        }

        public event EventHandler<int> LengthChanged;
        public ChannelInfo Info { get; set; }

        public int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(BatteryChannel_get_total_length(ChannelPtr, out var length));
                return (int)length;
            }
        }

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(BatteryChannel_get_buffer_size(ChannelPtr, out var size));
                return (int) size;
            }
        }

        public float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(BatteryChannel_get_sampling_frequency(ChannelPtr, out var frequency));
                return frequency;
            }
        }

        public IntPtr ChannelPtr { get; }

        public int[] ReadData(int offset, int length)
        {
            if (length <= 0)
            {
                return new int[0];
            }
            
            var bufferPtr = Marshal.AllocHGlobal(length * sizeof(int));
            try
            {
                SdkError.ThrowIfError(
                    BatteryChannel_read_data(ChannelPtr, (IntPtr) offset, (IntPtr) length, bufferPtr));
                var buffer = new int[length];
                Marshal.Copy(bufferPtr, buffer, 0, length);
                return buffer;
            }
            finally
            {
                Marshal.FreeHGlobal(bufferPtr);
            }
        }

        private void OnTotalLengthChanged(IntPtr channelPtr, IntPtr length)
        {
            LengthChanged?.Invoke(this, (int)length);
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
        private static extern int BatteryChannel_add_length_callback(IntPtr batteryChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_get_total_length(IntPtr batteryChannelPtr, out IntPtr totalLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryChannel_get_buffer_size(IntPtr batteryChannelPtr, out IntPtr bufferSize);
    }
}
