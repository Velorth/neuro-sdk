using System;

namespace Neuro
{
    public class BatteryChannel : BaseChannel<int>
    {
        public BatteryChannel(ChannelInfo channelInfo) : base(channelInfo)
        {
        }

        public override event EventHandler<int> LengthChanged;
        public override int TotalLength { get; }
        public override int BufferSize { get; }
        public override float SamplingFrequency { get; set; }
    }
}
