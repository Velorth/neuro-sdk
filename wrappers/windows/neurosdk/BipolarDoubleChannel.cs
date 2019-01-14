using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class BipolarDoubleChannel : DataChannel<double>
    {
        private readonly IntPtr _listenerPtr;
        private readonly LengthChangedFunc _lengthChangedFunc;

        public BipolarDoubleChannel(DataChannel<double> first, DataChannel<double> second)
        {
            ChannelPtr = create_BipolarDoubleChannel(first.ChannelPtr, second.ChannelPtr);
            if (ChannelPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }

            _lengthChangedFunc = OnTotalLengthChanged;
            SdkError.ThrowIfError(BipolarDoubleChannel_add_length_callback(ChannelPtr, _lengthChangedFunc, out _listenerPtr));
            SdkError.ThrowIfError(BipolarDoubleChannel_get_info(ChannelPtr, out var info));
            Info = info;
        }

        ~BipolarDoubleChannel()
        {
            free_listener_handle(_listenerPtr);
            BipolarDoubleChannel_delete(ChannelPtr);
        }

        public event EventHandler<int> LengthChanged;
        public ChannelInfo Info { get; set; }

        public int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(BipolarDoubleChannel_get_total_length(ChannelPtr, out var length));
                return (int)length;
            }
        }

        public float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(BipolarDoubleChannel_get_sampling_frequency(ChannelPtr, out var frequency));
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
                    BipolarDoubleChannel_read_data(ChannelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr));
                var buffer = new double[length];
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
        private static extern void free_listener_handle(IntPtr listenerPtr);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr channelPtr, IntPtr length);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_BipolarDoubleChannel(IntPtr firstPtr, IntPtr secondPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void BipolarDoubleChannel_delete(IntPtr bipolarChannelPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BipolarDoubleChannel_get_info(IntPtr bipolarChannelPtr, out ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BipolarDoubleChannel_read_data(IntPtr bipolarChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BipolarDoubleChannel_get_sampling_frequency(IntPtr bipolarChannelPtr, out float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BipolarDoubleChannel_add_length_callback(IntPtr bipolarChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BipolarDoubleChannel_get_total_length(IntPtr bipolarChannelPtr, out IntPtr totalLength);
    }
}
