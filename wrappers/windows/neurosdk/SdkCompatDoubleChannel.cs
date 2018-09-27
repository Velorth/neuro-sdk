using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public abstract class SdkCompatDoubleChannel : BaseChannel<double>
    {
        private LengthChangedFunc _lengthCallback;
        private readonly ReadDataFunc _readDataFunc;
        private readonly GetFrequencyFunc _getFrequencyFunc;
        private readonly SetFrequencyFunc _setFrequencyFunc;
        private readonly AddLengthCallbackFunc _addLengthCallbackFunc;
        private readonly GetTotalLengthFunc _getTotalLengthFunc;
        private readonly GetBufferSizeFunc _getBufferSizeFunc;
        private readonly GetDeviceFunc _getDeviceFunc;

        protected SdkCompatDoubleChannel(Device device, ChannelInfo info) : base(device)
        {
            _readDataFunc = ReadDataForNative;
            _getFrequencyFunc = GetFrequencyForNative;
            _setFrequencyFunc = SetFrequencyForNative;
            _addLengthCallbackFunc = AddLengthCallbackForNative;
            _getTotalLengthFunc = GetTotalLengthForNative;
            _getBufferSizeFunc = GetBufferSizeForNative;
            _getDeviceFunc = GetDeviceForNative;
            ChannelPtr = create_BridgeDoubleChannel_info(info, _readDataFunc, _getFrequencyFunc, _setFrequencyFunc,
                _addLengthCallbackFunc, _getTotalLengthFunc, _getBufferSizeFunc, _getDeviceFunc);
            Info = info;
        }

        public sealed override ChannelInfo Info { get; set; }

        ~SdkCompatDoubleChannel()
        {
            BridgeDoubleChannel_delete(ChannelPtr);
        }

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

        private int SetFrequencyForNative(float frequency)
        {
            try
            {
                SamplingFrequency = frequency;
            }
            catch (Exception)
            {
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

        private int GetBufferSizeForNative(out IntPtr outBufferSize)
        {
            try
            {
                outBufferSize = (IntPtr) BufferSize;
            }
            catch (Exception)
            {
                outBufferSize = (IntPtr) 0;
                return SdkError.ErrorUnhandledException;
            }

            return SdkError.SdkNoError;
        }

        private IntPtr GetDeviceForNative()
        {
            return UnderlyingDevice?.DevicePtr ?? (IntPtr)0;
        }

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int ReadDataFunc(IntPtr offset, IntPtr length, IntPtr buffer);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int GetFrequencyFunc(out float outFrequency);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int SetFrequencyFunc(float frequency);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void LengthChangedFunc(IntPtr channelPtr, IntPtr length);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int AddLengthCallbackFunc(LengthChangedFunc func, out IntPtr handle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int GetTotalLengthFunc(out IntPtr outLength);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate int GetBufferSizeFunc(out IntPtr outBufferSize);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate IntPtr GetDeviceFunc();

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_BridgeDoubleChannel_info(ChannelInfo info, ReadDataFunc readDataFunc, GetFrequencyFunc getFrequencyFunc, 
            SetFrequencyFunc setFrequencyFunc, AddLengthCallbackFunc addLengthCallbackFunc, GetTotalLengthFunc getTotalLengthFunc, 
            GetBufferSizeFunc getBufferSizeFunc, GetDeviceFunc getDeviceFunc);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void BridgeDoubleChannel_delete(IntPtr baseDoubleChannel);

    }
}
