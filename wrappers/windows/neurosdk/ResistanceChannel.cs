using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class ResistanceChannel : IBaseChannel<double>
    {
        private readonly IntPtr _listenerPtr;

        public ResistanceChannel(Device device)
        {
            ChannelPtr = create_ResistanceChannel(device.DevicePtr);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(ResistanceChannel_add_length_callback(ChannelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            SdkError.ThrowIfError(ResistanceChannel_get_info(ChannelPtr, out var info));
            Info = info;
        }

        public ResistanceChannel(Device device, ChannelInfo info)
        {
            ChannelPtr = create_ResistanceChannel_info(device.DevicePtr, info);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }
            SdkError.ThrowIfError(ResistanceChannel_add_length_callback(ChannelPtr, (channelPtr, length) => { LengthChanged?.Invoke(this, (int)length); }, out _listenerPtr));
            Info = info;
        }

        ~ResistanceChannel()
        {
            ResistanceChannel_delete(ChannelPtr);
            free_listener_handle(_listenerPtr);
        }

        public event EventHandler<int> LengthChanged;
        public ChannelInfo Info { get; set; }

        public int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(ResistanceChannel_get_total_length(ChannelPtr, out var length));
                return (int)length;
            }
        }

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(ResistanceChannel_get_buffer_size(ChannelPtr, out var size));
                return (int)size;
            }
        }

        public float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(ResistanceChannel_get_sampling_frequency(ChannelPtr, out var frequency));
                return frequency;
            }
        }

        public IntPtr ChannelPtr { get; }

        public double[] ReadData(int offset, int length)
        {
            if (length <= 0)
            {
                return new double[0];
            }

            var bufferPtr = Marshal.AllocHGlobal(length * sizeof(double));
            try
            {
                SdkError.ThrowIfError(
                    ResistanceChannel_read_data(ChannelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr));
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
        private static extern void ResistanceChannel_delete(IntPtr resistanceChannelPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_info(IntPtr resistanceChannelPtr, out ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_read_data(IntPtr resistanceChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_sampling_frequency(IntPtr resistanceChannelPtr, out float samplingFrequency);
        
        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_add_length_callback(IntPtr resistanceChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_total_length(IntPtr resistanceChannelPtr, out IntPtr totalLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceChannel_get_buffer_size(IntPtr resistanceChannelPtr, out IntPtr bufferSize);
    }
}
