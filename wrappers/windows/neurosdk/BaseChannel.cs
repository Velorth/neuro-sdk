using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public interface ICommonChannelInterface
    {
        event EventHandler<int> LengthChanged;
        ChannelInfo Info { get; set; }
        int TotalLength { get; }
        float SamplingFrequency { get; }
    }

    public interface IBaseChannel<out T> : ICommonChannelInterface
    {
        IntPtr ChannelPtr { get; }
        T[] ReadData(int offset, int length);
    }
}
