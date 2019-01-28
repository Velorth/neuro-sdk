using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public interface IChannel
    {
        event EventHandler<int> LengthChanged;
        IntPtr ChannelPtr { get; }
        ChannelInfo Info { get; }
        int TotalLength { get; }
        float SamplingFrequency { get; }
    }

    public interface IDataChannel<out T> : IChannel
    {
        T[] ReadData(int offset, int length);
    }
}
