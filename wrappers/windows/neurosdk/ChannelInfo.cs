using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Neuro
{
    public enum ChannelType
    {
        Signal,
        Battery,
        ElectrodesState,
        Respiration,
        MEMS,
        Orientation,
        ConnectionStats,
        Resistance,
        Pedometer,
        Custom
    }

    public class ChannelInfo
    {
        public string Name { get; set; }
        public ChannelType Type { get; private set; }
        public int Index { get; private set; }
    }
}
