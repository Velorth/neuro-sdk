using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public enum SpectrumWindow
    {
        Rectangular,
        Sine,
        Hamming,
        Blackman
    }

    public class SpectrumChannel : IDataChannel<double>, IDisposable
    {
        private readonly IDataChannel<double> _sourceChannel; //store source channel reference to prevent its deletion
        private readonly AnyChannel _anyChannel;

        public SpectrumChannel(IDataChannel<double> channel)
        {
            _sourceChannel = channel;
            _anyChannel = new AnyChannel(create_SpectrumDoubleChannel(channel.ChannelPtr));
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public void Dispose()
        {
            _anyChannel.Dispose();
        }

        public event EventHandler<int> LengthChanged;

        public ChannelInfo Info => _anyChannel.Info;

        public int TotalLength => _anyChannel.TotalLength;

        public float SamplingFrequency => _anyChannel.SamplingFrequency;

        public IntPtr ChannelPtr => _anyChannel.ChannelPtr;

        public double[] ReadData(int offset, int length)
        {
            return ReadData(offset, length, SpectrumWindow.Rectangular);
        }

        public double[] ReadData(int offset, int length, SpectrumWindow windowType)
        {
            var bufferSize = SpectrumLength * Marshal.SizeOf<double>();
            var bufferPtr = Marshal.AllocHGlobal(bufferSize);
            try
            {
                SdkError.ThrowIfError(SpectrumDoubleChannel_read_data(_anyChannel.ChannelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr, (IntPtr)bufferSize, out var dataRead, windowType));
                return new NativeArrayMarshaler<double>().MarshalArray(bufferPtr, dataRead);
            }
            finally
            {
                Marshal.FreeHGlobal(bufferPtr);
            }
        }

        public double HzPerSpectrumSample
        {
            get
            {
                SdkError.ThrowIfError(SpectrumDoubleChannel_get_hz_per_spectrum_sample(ChannelPtr, out var frequencyStep));
                return frequencyStep;
            }
        }

        public int SpectrumLength
        {
            get
            {
                SdkError.ThrowIfError(SpectrumDoubleChannel_get_spectrum_length(ChannelPtr, out var spectrumLength));
                return (int)spectrumLength;
            }
        }
      
        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SpectrumDoubleChannel(IntPtr channelPtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumDoubleChannel_read_data(IntPtr channelPtr, IntPtr offset, IntPtr length,
            IntPtr outBuffer, IntPtr bufferSize, out IntPtr samplesRead, SpectrumWindow windowType);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumDoubleChannel_get_hz_per_spectrum_sample(IntPtr spectrumChannelPtr, out double hzPerSample);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumDoubleChannel_get_spectrum_length(IntPtr spectrumChannelPtr, out IntPtr spectrumLength);
    }
}
