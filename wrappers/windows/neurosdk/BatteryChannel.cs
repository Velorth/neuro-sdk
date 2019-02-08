using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class BatteryChannel : IDataChannel<int>
    {
        private readonly Device _device; //store device reference to prevent its deletion
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<int> _dataChannel;

        public BatteryChannel(Device device)
        {
            _device = device;
            _anyChannel = new AnyChannel(create_BatteryIntChannel(device.DevicePtr));
            _dataChannel = new DataChannel<int>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public event EventHandler<int> LengthChanged;

        public ChannelInfo Info => _anyChannel.Info;

        public int TotalLength => _anyChannel.TotalLength;

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(BatteryIntChannel_get_buffer_size(ChannelPtr, out var size));
                return (int)size;
            }
        }

        public float SamplingFrequency => _anyChannel.SamplingFrequency;

        public IntPtr ChannelPtr => _anyChannel.ChannelPtr;

        public int[] ReadData(int offset, int length)
        {
            return _dataChannel.ReadData(offset, length);
        }
        
        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_BatteryIntChannel(IntPtr devicePtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int BatteryIntChannel_get_buffer_size(IntPtr batteryChannelPtr, out IntPtr bufferSize);
    }
}
