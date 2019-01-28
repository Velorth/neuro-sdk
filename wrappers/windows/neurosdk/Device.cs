using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace Neuro
{
    public class Device
    {
        public readonly IntPtr DevicePtr;
        public event EventHandler<Parameter> ParameterChanged;

        internal Device(IntPtr devicePtr)
        {
            DevicePtr = devicePtr;
            Debug.Assert(DevicePtr != null);
            if (DevicePtr == null)
            {
                throw new ArgumentNullException(nameof(devicePtr), "Device pointer is null");
            }

            var result = device_subscribe_param_changed(DevicePtr, OnParameterChanged);
            Debug.Assert(result == SdkError.SdkNoError);
            SdkError.ThrowIfError(result);
        }

        ~Device()
        {
            device_delete(DevicePtr);
        }

        public IEnumerable<ChannelInfo> Channels
        {
            get
            {
                SdkError.ThrowIfError(device_available_channels(DevicePtr, out var nativeArray));
                return new NativeArrayMarshaler<ChannelInfo>().MarshalArray(nativeArray.InfoArray, nativeArray.InfoCount);
            }
        }

        public IEnumerable<Command> Commands
        {
            get
            {
                SdkError.ThrowIfError(device_available_commands(DevicePtr, out var nativeArray));
                return new NativeArrayMarshaler<Command>().MarshalArray(nativeArray.CmdArray, nativeArray.CmdArraySize);
            }
        }

        public IEnumerable<ParamInfo> Parameters
        {
            get
            {
                SdkError.ThrowIfError(device_available_parameters(DevicePtr, out var nativeArray));
                return new NativeArrayMarshaler<ParamInfo>().MarshalArray(nativeArray.InfoArray, nativeArray.InfoCount);
            }
        }

        public void Connect()
        {
            SdkError.ThrowIfError(device_connect(DevicePtr));
        }

        public void Disconnect()
        {
            SdkError.ThrowIfError(device_disconnect(DevicePtr));
        }

        public void Execute(Command command)
        {
            SdkError.ThrowIfError(device_execute(DevicePtr, command));
        }

        public T ReadParam<T>(Parameter parameter)
        {
            var paramTypeInfo = new ParameterTypeInfo(parameter);
            if (paramTypeInfo.Type != typeof(T))
            {
                throw new ArgumentException($"Wrong return generic type argument. Must be {paramTypeInfo.Type.Name}");
            }

            var paramValue = paramTypeInfo.ReadParamValue(this);
            if (!(paramValue is T))
            {
                throw new ArgumentException("Return type of native readParam method is not equal to generic type parameter");
            }

            return (T)paramValue;
        }

        public void SetParam<T>(Parameter parameter, T value)
        {
            var paramTypeInfo = new ParameterTypeInfo(parameter);
            if (paramTypeInfo.Type != typeof(T))
            {
                throw new ArgumentException($"Wrong return generic type argument. Must be {paramTypeInfo.Type.Name}");
            }

            paramTypeInfo.SetParamValue(this, value);
        }

        private void OnParameterChanged(IntPtr devicePtr, Parameter parameter)
        {
            ParameterChanged?.Invoke(this, parameter);
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceParamChangedFunc(IntPtr devicePtr, Parameter parameter);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_connect(IntPtr devicePtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_disconnect(IntPtr devicePtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void device_delete(IntPtr devicePtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_channels(IntPtr devicePtr, out ChannelInfoArray infoArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_commands(IntPtr devicePtr, out CommandArray cmdArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_parameters(IntPtr devicePtr, out ParamInfoArray infoArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_execute(IntPtr devicePtr, Command command);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_subscribe_param_changed(IntPtr devicePtr, DeviceParamChangedFunc callback);
    }
}
