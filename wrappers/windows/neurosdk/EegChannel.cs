using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class EegChannel : IDataChannel<double>, IDisposable
    {
        private readonly Device _device; //store device reference to prevent its deletion
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;

        public EegChannel(Device device)
        {
            _device = device;
            _anyChannel = new AnyChannel(create_EegDoubleChannel(device.DevicePtr));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public EegChannel(Device device, ChannelInfo info)
        {
            _anyChannel = new AnyChannel(create_EegDoubleChannel_info(device.DevicePtr, info));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public void Dispose()
        {
            _anyChannel.Dispose();
        }

        public event EventHandler<int> LengthChanged;

        public ChannelInfo Info => _anyChannel.Info;

        public int TotalLength => _anyChannel.TotalLength;

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(EegDoubleChannel_get_buffer_size(ChannelPtr, out var size));
                return (int)size;
            }
        }

        public float SamplingFrequency => _anyChannel.SamplingFrequency;

        public IntPtr ChannelPtr => _anyChannel.ChannelPtr;

        public double[] ReadData(int offset, int length)
        {
            return _dataChannel.ReadData(offset, length);
        }

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_EegDoubleChannel(IntPtr devicePtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_EegDoubleChannel_info(IntPtr devicePtr, ChannelInfo info);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EegDoubleChannel_get_buffer_size(IntPtr eegChannelPtr, out IntPtr bufferSize);
    }
}
