using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class SpectrumChannel : IDataChannel<double>
    {
        private readonly IDataChannel<double> _sourceChannel; //store source channel reference to prevent its deletion
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;

        public SpectrumChannel(IDataChannel<double> channel)
        {
            _sourceChannel = channel;
            _anyChannel = new AnyChannel(create_SpectrumDoubleChannel(channel.ChannelPtr));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public event EventHandler<int> LengthChanged;

        public ChannelInfo Info => _anyChannel.Info;

        public int TotalLength => _anyChannel.TotalLength;

        public float SamplingFrequency => _anyChannel.SamplingFrequency;

        public IntPtr ChannelPtr => _anyChannel.ChannelPtr;

        public double[] ReadData(int offset, int length)
        {
            return _dataChannel.ReadData(offset, length);
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
        private static extern int SpectrumDoubleChannel_get_hz_per_spectrum_sample(IntPtr spectrumChannelPtr, out double hzPerSample);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumDoubleChannel_get_spectrum_length(IntPtr spectrumChannelPtr, out IntPtr spectrumLength);
    }
}
