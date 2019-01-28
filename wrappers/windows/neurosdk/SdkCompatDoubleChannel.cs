using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public abstract class SdkCompatDoubleChannel : IDataChannel<double>
    {
        private LengthChangedFunc _lengthCallback;
        private readonly ReadDataFunc _readDataFunc;
        private readonly GetFrequencyFunc _getFrequencyFunc;
        private readonly AddLengthCallbackFunc _addLengthCallbackFunc;
        private readonly GetTotalLengthFunc _getTotalLengthFunc;

        protected SdkCompatDoubleChannel(Device device, ChannelInfo info)
        {
            _readDataFunc = ReadDataForNative;
            _getFrequencyFunc = GetFrequencyForNative;
            _addLengthCallbackFunc = AddLengthCallbackForNative;
            _getTotalLengthFunc = GetTotalLengthForNative;
            ChannelPtr = create_BridgeDoubleChannel_info(info, _readDataFunc, _getFrequencyFunc,
                _addLengthCallbackFunc, _getTotalLengthFunc);
            Info = info;
        }

        ~SdkCompatDoubleChannel()
        {
            AnyChannel_delete(ChannelPtr);
        }

        public event EventHandler<int> LengthChanged;
        public ChannelInfo Info { get; set; }
        public abstract int TotalLength { get; }
        public abstract float SamplingFrequency { get; set; }
        public IntPtr ChannelPtr { get; }
        public abstract double[] ReadData(int offset, int length);

        private int ReadDataForNative(IntPtr offset, IntPtr length, IntPtr buffer)
        {
            try
            {
                var data = ReadData((int)offset, (int)length);
                if (data.Length != (int)length)
                {
                    return SdkError.ErrorUnhandledException;
                }
                Marshal.Copy(data, 0, buffer, (int)length);
            }
            catch (Exception e)
            {
                return SdkError.ErrorUnhandledException;
            }

            return SdkError.SdkNoError;
        }

        private int GetFrequencyForNative(out float outFrequency)
        {
            try
            {
                outFrequency = SamplingFrequency;
            }
            catch (Exception)
            {
                outFrequency = 0f;
                return SdkError.ErrorUnhandledException;
            }

            return SdkError.SdkNoError;
        }

        private int AddLengthCallbackForNative(LengthChangedFunc lengthChangedCallback, out IntPtr handle)
        {
            _lengthCallback = lengthChangedCallback;
            LengthChanged += SdkCompatDoubleChannel_LengthChanged;
            handle = (IntPtr)0;
            return SdkError.SdkNoError;
        }

        private void SdkCompatDoubleChannel_LengthChanged(object sender, int length)
        {
            _lengthCallback(ChannelPtr, (IntPtr)length);
        }

        private int GetTotalLengthForNative(out IntPtr outLength)
        {
            try
            {
                outLength = (IntPtr) TotalLength;
            }
            catch (Exception)
            {
                outLength = (IntPtr) 0;
                return SdkError.ErrorUnhandledException;
            }

            return SdkError.SdkNoError;
        }


        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int ReadDataFunc(IntPtr offset, IntPtr length, IntPtr buffer);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int GetFrequencyFunc(out float outFrequency);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr channelPtr, IntPtr length);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int AddLengthCallbackFunc(LengthChangedFunc func, out IntPtr handle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int GetTotalLengthFunc(out IntPtr outLength);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_BridgeDoubleChannel_info(ChannelInfo info, ReadDataFunc readDataFunc, GetFrequencyFunc getFrequencyFunc,
            AddLengthCallbackFunc addLengthCallbackFunc, GetTotalLengthFunc getTotalLengthFunc);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void AnyChannel_delete(IntPtr baseDoubleChannel);
    }
}
