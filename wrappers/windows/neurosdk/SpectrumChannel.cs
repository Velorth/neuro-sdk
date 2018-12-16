using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class SpectrumChannel : IBaseChannel<double>
    {
        private readonly IntPtr _listenerPtr;
        private readonly LengthChangedFunc _lengthChangedFunc;

        public SpectrumChannel(IBaseChannel<double> channel)
        {
            ChannelPtr = create_SpectrumChannel(channel.ChannelPtr);
            if (ChannelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }

            _lengthChangedFunc = OnTotalLengthChanged;
            SdkError.ThrowIfError(SpectrumChannel_add_length_callback(ChannelPtr, _lengthChangedFunc, out _listenerPtr));
            SdkError.ThrowIfError(SpectrumChannel_get_info(ChannelPtr, out var info));
            Info = info;
        }

        ~SpectrumChannel()
        {
            SpectrumChannel_delete(ChannelPtr);
            free_listener_handle(_listenerPtr);
        }

        public event EventHandler<int> LengthChanged;
        public ChannelInfo Info { get; set; }

        public int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(SpectrumChannel_get_total_length(ChannelPtr, out var length));
                return (int)length;
            }
        }

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(SpectrumChannel_get_buffer_size(ChannelPtr, out var size));
                return (int)size;
            }
        }

        public float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(SpectrumChannel_get_sampling_frequency(ChannelPtr, out var frequency));
                return frequency;
            }
        }

        public double HzPerSpectrumSample
        {
            get
            {
                SdkError.ThrowIfError(SpectrumChannel_get_hz_per_spectrum_sample(ChannelPtr, out var frequencyStep));
                return frequencyStep;
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
                    SpectrumChannel_read_data(ChannelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr));
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
        private static extern void free_listener_handle(IntPtr spectrumChannelPtr);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr channelPtr, IntPtr length);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SpectrumChannel(IntPtr channelPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void SpectrumChannel_delete(IntPtr spectrumChannelPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumChannel_get_info(IntPtr spectrumChannelPtr, out ChannelInfo info);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumChannel_read_data(IntPtr spectrumChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumChannel_get_sampling_frequency(IntPtr spectrumChannelPtr, out float samplingFrequency);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumChannel_get_hz_per_spectrum_sample(IntPtr spectrumChannelPtr, out double hzPerSample);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumChannel_add_length_callback(IntPtr spectrumChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumChannel_get_total_length(IntPtr spectrumChannelPtr, out IntPtr totalLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumChannel_get_buffer_size(IntPtr spectrumChannelPtr, out IntPtr bufferSize);
    }
}
