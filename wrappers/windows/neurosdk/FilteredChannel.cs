using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class FilteredChannel : IDataChannel<double>, IDisposable
    {
        private readonly IDataChannel<double> _sourceChannel; //store source channel reference to prevent its deletion
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;

        public FilteredChannel(IDataChannel<double> channel, Filter[] filters)
        {
            _sourceChannel = channel;
            _anyChannel = new AnyChannel(create_FilteredDoubleChannel(channel.ChannelPtr, filters, (IntPtr)filters.Length));
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

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(FilteredDoubleChannel_get_buffer_size(ChannelPtr, out var size));
                return (int) size;
            }
        }

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_FilteredDoubleChannel(IntPtr channelPtr, Filter[] filtersArray, IntPtr filterCount);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FilteredDoubleChannel_get_buffer_size(IntPtr spectrumChannelPtr, out double outBufferSize);
    }

}