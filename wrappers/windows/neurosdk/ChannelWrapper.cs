using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class AnyChannel
    {
        private readonly IntPtr _listenerPtr;
        private readonly LengthChangedFunc _lengthChangedFunc;

        public event EventHandler<int> LengthChanged;

        public IntPtr ChannelPtr { get; }

        public ChannelInfo Info
        {
            get
            {
                SdkError.ThrowIfError(AnyChannel_get_info(ChannelPtr, out var info));
                return info;
            }
        }

        public int TotalLength
        {
            get
            {
                SdkError.ThrowIfError(AnyChannel_get_total_length(ChannelPtr, out var length));
                return (int)length;
            }
        }

        public float SamplingFrequency
        {
            get
            {
                SdkError.ThrowIfError(AnyChannel_get_sampling_frequency(ChannelPtr, out var frequency));
                return frequency;
            }
        }

        public AnyChannel(IntPtr channelPtr)
        {
            if (channelPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }

            ChannelPtr = channelPtr;
            _lengthChangedFunc = OnTotalLengthChanged;
            SdkError.ThrowIfError(AnyChannel_add_length_callback(ChannelPtr, _lengthChangedFunc, out _listenerPtr, IntPtr.Zero));
        }

        ~AnyChannel()
        {
            free_listener_handle(_listenerPtr);
            AnyChannel_delete(ChannelPtr);
        }

        private void OnTotalLengthChanged(IntPtr channelPtr, IntPtr length, IntPtr userData)
        {
            if (channelPtr != ChannelPtr) return;

            LengthChanged?.Invoke(this, (int)length);
        }

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_listener_handle(IntPtr listenerHandle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr anyChannelPtr, IntPtr length, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void AnyChannel_delete(IntPtr anyChannelPtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int AnyChannel_get_info(IntPtr anyChannelPtr, out ChannelInfo info);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int AnyChannel_get_sampling_frequency(IntPtr anyChannelPtr, out float samplingFrequency);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int AnyChannel_add_length_callback(IntPtr anyChannelPtr, LengthChangedFunc callback, out IntPtr listenerHandle, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int AnyChannel_get_total_length(IntPtr anyChannelPtr, out IntPtr totalLength);
    }

    public sealed class DataChannel<T>
    {
        private readonly NativeChannelDataReader<T> _channelDataReader;
        private readonly AnyChannel _anyChannel;

        public DataChannel(AnyChannel channel)
        {
            _anyChannel = channel;
            _channelDataReader = new NativeChannelDataReader<T>();
        }

        public T[] ReadData(int offset, int length)
        {
            return length > 0 ? _channelDataReader.ReadChannelData(_anyChannel.ChannelPtr, offset, length) : new T[0];
        }
    }

    internal class NativeChannelDataReader<T>
    {
        private delegate int ReaderFuncDelegate(IntPtr signalChannelPtr, IntPtr offset, IntPtr length, IntPtr buffer, IntPtr bufferSize, out IntPtr dataRead);

        private readonly ReaderFuncDelegate _readerFunc;
        private readonly NativeArrayMarshaler<T> _arrayMarshaler;

        internal NativeChannelDataReader()
        {
            _readerFunc = GetFunctionForType(typeof(T));
            _arrayMarshaler = new NativeArrayMarshaler<T>();
        }

        internal T[] ReadChannelData(IntPtr channelPtr, int offset, int length)
        {
            var bufferSize = length * Marshal.SizeOf<T>();
            var bufferPtr = Marshal.AllocHGlobal(bufferSize);
            try
            {
                SdkError.ThrowIfError(_readerFunc(channelPtr, (IntPtr) offset, (IntPtr) length, bufferPtr, (IntPtr)bufferSize, out var dataRead));
                return _arrayMarshaler.MarshalArray(bufferPtr, dataRead);
            }
            finally
            {
                Marshal.FreeHGlobal(bufferPtr);
            }
        }

        private static ReaderFuncDelegate GetFunctionForType(Type type)
        {
            if (type == typeof(int))
            {
                return NativeDataReadFunction.IntChannel_read_data;
            }

            if (type == typeof(double))
            {
                return NativeDataReadFunction.DoubleChannel_read_data;
            }

            throw new ArgumentException($"There is no reader function for type {type.Name}");
        }
    }

    internal class NativeDataReadFunction
    {
        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int IntChannel_read_data(IntPtr signalChannelPtr, IntPtr offset, IntPtr length,
            IntPtr buffer, IntPtr bufferSize, out IntPtr dataRead);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DoubleChannel_read_data(IntPtr signalChannelPtr, IntPtr offset, IntPtr length,
            IntPtr buffer, IntPtr bufferSize, out IntPtr dataRead);
    }
}
