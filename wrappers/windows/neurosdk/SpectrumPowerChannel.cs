using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class SpectrumPowerChannel : IDataChannel<double>, IDisposable
    {
        private readonly IntPtr[] _channelsPointers;
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;


        public SpectrumPowerChannel(IEnumerable<SpectrumChannel> channels, float lowFrequency, float highFrequency, string name, double windowDuration = 8.0, double overlappingCoeff = 0.9)
        {
            _channelsPointers = channels.Select(x => x.ChannelPtr).ToArray();

            _anyChannel = new AnyChannel(create_SpectrumPowerDoubleChannel(_channelsPointers, (IntPtr)_channelsPointers.Length, lowFrequency, highFrequency, name, windowDuration, overlappingCoeff));
            _dataChannel = new DataChannel<double>(_anyChannel);
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
            return _dataChannel.ReadData(offset, length);
        }

        public double WindowDuration
        {
            set => SdkError.ThrowIfError(SpectrumPowerDoubleChannel_set_window_duration(ChannelPtr, value));
        }

        public double OverlappingCoefficient
        {
            set => SdkError.ThrowIfError(SpectrumPowerDoubleChannel_set_overlapping_coefficient(ChannelPtr, value));
        }

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SpectrumPowerDoubleChannel(IntPtr[] channels, IntPtr channelsCount, float lowFreq, float highFreq, 
            [MarshalAs(UnmanagedType.LPStr)] string name, double windowDuration, double overlappingCoeff);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumPowerDoubleChannel_set_overlapping_coefficient(IntPtr channel, double overlap);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SpectrumPowerDoubleChannel_set_window_duration(IntPtr channel, double duration);
    }
}