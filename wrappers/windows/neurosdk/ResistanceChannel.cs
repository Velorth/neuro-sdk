using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class ResistanceChannel : IDataChannel<double>
    {
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;

        public ResistanceChannel(Device device, ChannelInfo info)
        {
            _anyChannel = new AnyChannel(create_ResistanceDoubleChannel_info(device.DevicePtr, info));
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
                SdkError.ThrowIfError(ResistanceDoubleChannel_get_buffer_size(ChannelPtr, out var size));
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
        private static extern IntPtr create_ResistanceDoubleChannel_info(IntPtr devicePtr, ChannelInfo info);
       
        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int ResistanceDoubleChannel_get_buffer_size(IntPtr resistanceChannelPtr, out IntPtr bufferSize);
    }
}
