using System.Collections.Generic;
using System.Linq;

namespace Neuro
{
    public static class DeviceTraits
    {
        public static bool HasChannelsWithType(Device device, ChannelType type)
        {
            return device.Channels.Any(deviceChannel => deviceChannel.Type == type);
        }

        public static IList<ChannelInfo> GetChannelsWithType(Device device, ChannelType type)
        {
            return device.Channels.Where(channelInfo => channelInfo.Type == type).ToList();
        }
    }
}
