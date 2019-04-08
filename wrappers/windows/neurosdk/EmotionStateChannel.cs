using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct EmotionalState
    {
        public int State;
        public int Stress;
        public int Attention;
        public int Relax;
        public int Meditation;
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct StateCoefficients
    {
        public double PX1;
        public double PX2;
        public double PX3;
        public double PX4;
        public double NX1;
        public double NX2;
        public double NX3;
        public double NX4;
        public double PY1;
        public double PY2;
        public double PY3;
        public double PY4;
        public double NY1;
        public double NY2;
        public double NY3;
        public double NY4;
    }

    public enum EmotionalStateName
    {
        Neutral,
        Relax,
        DeepRelax,
        Sleep,
        NormalActivation,
        Excitement,
        DeepExcitement
    }

    public class EmotionStateChannel : IDataChannel<EmotionalState>, IDisposable
    {
        private readonly AnyChannel _anyChannel;
        private readonly NativeArrayMarshaler<EmotionalState> _arrayMarshaler = new NativeArrayMarshaler<EmotionalState>();

        //store source channels reference to prevent their deletion
        private readonly EegIndexChannel _indexChannel;

        public EmotionStateChannel(EegIndexChannel indexChannel)
        {
            _indexChannel = indexChannel;
            _anyChannel = new AnyChannel(create_EmotionalStateChannel(_indexChannel.ChannelPtr));
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

        public EmotionalState[] ReadData(int offset, int length)
        {
            var bufferPtr = Marshal.AllocHGlobal(length * Marshal.SizeOf<EmotionalState>());
            try
            {
                SdkError.ThrowIfError(EmotionalStateChannel_read_data(ChannelPtr, (IntPtr)offset, (IntPtr)length, bufferPtr, (IntPtr)length, out var dataRead));
                return _arrayMarshaler.MarshalArray(bufferPtr, dataRead);
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
                SdkError.ThrowIfError(EmotionalStateChannel_get_buffer_size(ChannelPtr, out var size));
                return (int)size;
            }
        }

        public StateCoefficients StateCoefficients
        {
            get
            {
                SdkError.ThrowIfError(EmotionalStateChannel_get_state_coefficients(ChannelPtr, out var coefficients));
                return coefficients;
            }
            set => SdkError.ThrowIfError(EmotionalStateChannel_set_state_coefficients(ChannelPtr, value));
        }

        public static EmotionalStateName ValueToName(int value)
        {
            if (value == 0)
            {
                return EmotionalStateName.Neutral;
            }
            else if (value > 0 && value <= 4)
            {
                return EmotionalStateName.Relax;
            }
            else if (value > 4 && value <= 8)
            {
                return EmotionalStateName.DeepRelax;
            }
            else if (value > 8)
            {
                return EmotionalStateName.Sleep;
            }
            else if (value < 0 && value >= -4)
            {
                return EmotionalStateName.NormalActivation;
            }
            else if (value < -4 && value >= -8)
            {
                return EmotionalStateName.Excitement;
            }
            else if (value < -8)
            {
                return EmotionalStateName.DeepExcitement;
            }
            else
            {
                throw new ArgumentOutOfRangeException("Wrong emotional state value");
            }
        }

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_EmotionalStateChannel(IntPtr indexChannel);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EmotionalStateChannel_read_data(IntPtr emotionalChannelPtr, IntPtr offset, IntPtr length,
            IntPtr buffer, IntPtr bufferSize, out IntPtr dataRead);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EmotionalStateChannel_get_buffer_size(IntPtr emotionalChannelPtr, out IntPtr bufferSize);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EmotionalStateChannel_get_state_coefficients(IntPtr emotionalChannelPtr, out StateCoefficients outStateCoeffs);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EmotionalStateChannel_set_state_coefficients(IntPtr emotionalChannelPtr, StateCoefficients stateCoeffs);
    }
}