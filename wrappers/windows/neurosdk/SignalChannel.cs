using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class SignalChannel : IDataChannel<double>
    {
        private readonly Device _device; //store device reference to prevent its deletion
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;

        public SignalChannel(Device device)
        {
            _device = device;
            _anyChannel = new AnyChannel(create_SignalDoubleChannel(device.DevicePtr));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public SignalChannel(Device device, ChannelInfo info)
        {
            _device = device;
            _anyChannel = new AnyChannel(create_SignalDoubleChannel_info(device.DevicePtr, info));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public SignalChannel(Device device, ChannelInfo info, Filter[] filters)
        {
            _device = device;
            _anyChannel = new AnyChannel(create_SignalDoubleChannel_info_filters(device.DevicePtr, info, filters, (IntPtr)filters.Length));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public SignalChannel(Device device, Filter[] filters)
        {
            _device = device;
            _anyChannel = new AnyChannel(create_SignalDoubleChannel_filters(device.DevicePtr, filters, (IntPtr)filters.Length));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public event EventHandler<int> LengthChanged;

        public ChannelInfo Info => _anyChannel.Info;

        public int TotalLength => _anyChannel.TotalLength;

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(SignalDoubleChannel_get_buffer_size(ChannelPtr, out var size));
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
        private static extern IntPtr create_SignalDoubleChannel(IntPtr devicePtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SignalDoubleChannel_info(IntPtr devicePtr, ChannelInfo info);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SignalDoubleChannel_info_filters(IntPtr devicePtr, ChannelInfo info, Filter[] filtersArray, IntPtr filterCount);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_SignalDoubleChannel_filters(IntPtr devicePtr, Filter[] filtersArray, IntPtr filterCount);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int SignalDoubleChannel_get_buffer_size(IntPtr signalChannelPtr, out IntPtr bufferSize);
    }
}
