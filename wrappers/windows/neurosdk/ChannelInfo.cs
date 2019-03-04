using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct ChannelInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
        public string Name;
        public ChannelType Type;
        public IntPtr Index;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct ChannelInfoArray
    {
        public IntPtr InfoArray;
        public IntPtr InfoCount;
    }
}
