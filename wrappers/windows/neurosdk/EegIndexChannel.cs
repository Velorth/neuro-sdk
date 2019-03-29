using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Neuro
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct EegIndexValues
    {
        public double AlphaRate;
        public double BetaRate;
        public double DeltaRate;
        public double ThetaRate;
    }

    public class EegIndexChannel : IDataChannel<EegIndexValues>, IDisposable
    {
        private readonly AnyChannel _anyChannel;
        private readonly NativeArrayMarshaler<EegIndexValues> _arrayMarshaler = new NativeArrayMarshaler<EegIndexValues>();

        //store source channels reference to prevent their deletion
        private readonly EegChannel _t3;
        private readonly EegChannel _t4;
        private readonly EegChannel _o1;
        private readonly EegChannel _o2;

        public EegIndexChannel(EegChannel t3, EegChannel t4, EegChannel o1, EegChannel o2)
        {
            _t3 = t3;
            _t4 = t4;
            _o1 = o1;
            _o2 = o2;
            _anyChannel = new AnyChannel(create_EegIndexChannel(_t3.ChannelPtr, _t4.ChannelPtr, _o1.ChannelPtr, _o2.ChannelPtr));
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public void Dispose()
        {
            _anyChannel.Dispose();
        }

        public event EventHandler<int> LengthChanged;

        public ChannelInfo Info => _anyChannel.Info;

        public int TotalLength => _anyChannel.TotalLength;

        public float SamplingFrequency => _anyChannel.SamplingFrequency;

        public IntPtr ChannelPtr => _anyChannel.ChannelPtr;

        public EegIndexValues[] ReadData(int offset, int length)
        {
            var bufferPtr = Marshal.AllocHGlobal(length * Marshal.SizeOf<ArtifactZone>());
            try
            {
                SdkError.ThrowIfError(EegIndexChannel_read_data(ChannelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr));
                return _arrayMarshaler.MarshalArray(bufferPtr, (IntPtr)length);
            }
            finally
            {
                Marshal.FreeHGlobal(bufferPtr);
            }
        }

        public int BufferSize
        {
            get
            {
                SdkError.ThrowIfError(EegIndexChannel_get_buffer_size(ChannelPtr, out var size));
                return (int)size;
            }
        }


        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_EegIndexChannel(IntPtr t3, IntPtr t4, IntPtr o1, IntPtr o2);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EegIndexChannel_read_data(IntPtr indexChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EegIndexChannel_get_buffer_size(IntPtr indexChannelPtr, out IntPtr bufferSize);
    }
}
