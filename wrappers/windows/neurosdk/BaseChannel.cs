using System;

namespace Neuro
{
    public interface CommonChannelInterface
    {
        event EventHandler<int> LengthChanged;
        ChannelInfo Info { get; set; }
        int TotalLength { get; }
        int BufferSize { get; }
        float SamplingFrequency { get; set; }
    }

    public abstract class BaseChannel<T> : CommonChannelInterface
    {
        protected BaseChannel(ChannelInfo channelInfo)
        {
            Info = channelInfo;
        }

        public abstract event EventHandler<int> LengthChanged;
        public ChannelInfo Info { get; set; }
        public abstract int TotalLength { get; }
        public abstract int BufferSize { get; }
        public abstract float SamplingFrequency { get; set; }
    }
}
