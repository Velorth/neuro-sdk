using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public interface ICommonChannelInterface
    {
        event EventHandler<int> LengthChanged;
        ChannelInfo Info { get; set; }
        int TotalLength { get; }
        int BufferSize { get; }
        float SamplingFrequency { get; set; }
        Device UnderlyingDevice { get; }
    }

    public abstract class BaseChannel<T> : ICommonChannelInterface
    {
        public IntPtr ChannelPtr { get; protected set; }

        protected BaseChannel(Device devcie)
        {
            UnderlyingDevice = devcie;
        }

        public abstract T[] ReadData(int offset, int length);

        public abstract event EventHandler<int> LengthChanged;
        public abstract ChannelInfo Info { get; set; }
        public abstract int TotalLength { get; }
        public abstract int BufferSize { get; }
        public abstract float SamplingFrequency { get; set; }
        public Device UnderlyingDevice { get; }
    }
}
