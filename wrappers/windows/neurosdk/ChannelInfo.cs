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

    [StructLayout(LayoutKind.Sequential)]
    public struct ChannelInfo
    {
        [MarshalAs(UnmanagedType.LPStr)]
        public string Name;
        public ChannelType Type;
        public uint Index;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct ChannelInfoArray
    {
        public IntPtr InfoArray;
        public uint InfoCount;
    }
}
