using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class Device : IDisposable
    {
        private readonly IntPtr _paramChangedListenerPtr;
        private readonly DeviceParamChangedFunc _paramChangedFunc;
        private readonly Dictionary<Action<object, ChannelData<double>>, IntPtr> _doubleChannelListenerHandles = new Dictionary<Action<object, ChannelData<double>>, IntPtr>();
        private readonly Dictionary<Action<object, ChannelData<int>>, IntPtr> _intChannelListenerHandles = new Dictionary<Action<object, ChannelData<int>>, IntPtr>();
        private readonly DeviceDoubleDataReceivedFunc _doubleDataReceivedFunc;
        private readonly DeviceIntDataReceivedFunc _intDataReceivedFunc;

        private event Action<object, ChannelData<double>> DoubleChannelDataReceived;
        private event Action<object, ChannelData<int>> IntChannelDataReceived;

        public readonly IntPtr DevicePtr;
        public event EventHandler<Parameter> ParameterChanged;

        public Device(DeviceInfo info)
        {
            DevicePtr = create_Device(info);
            if (DevicePtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }

            _paramChangedFunc = OnParameterChanged;
            var result = device_subscribe_param_changed(DevicePtr, _paramChangedFunc, out _paramChangedListenerPtr, IntPtr.Zero);
            SdkError.ThrowIfError(result);

            _doubleDataReceivedFunc = OnDoubleDataReceived;
            _intDataReceivedFunc = OnIntDataReceived;
        }

        internal Device(IntPtr devicePtr)
        {
            DevicePtr = devicePtr;
            if (DevicePtr == null)
            {
                throw new ArgumentNullException(nameof(devicePtr), "Device pointer is null");
            }

            _paramChangedFunc = OnParameterChanged;
            var result = device_subscribe_param_changed(DevicePtr, _paramChangedFunc, out _paramChangedListenerPtr, IntPtr.Zero);
            SdkError.ThrowIfError(result);

            _doubleDataReceivedFunc = OnDoubleDataReceived;
            _intDataReceivedFunc = OnIntDataReceived;
        }

        private void ReleaseUnmanagedResources()
        {
            RemoveAllCahnnelListeners();
            free_listener_handle(_paramChangedListenerPtr);
            device_delete(DevicePtr);
        }

        public void Dispose()
        {
            ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        ~Device()
        {
            ReleaseUnmanagedResources();
        }

        public IEnumerable<ChannelInfo> Channels
        {
            get
            {
                SdkError.ThrowIfError(device_available_channels(DevicePtr, out var nativeArray));
                var channels = new NativeArrayMarshaler<ChannelInfo>().MarshalArray(nativeArray.InfoArray, nativeArray.InfoCount);
                free_ChannelInfoArray(nativeArray);
                return channels;
            }
        }

        public IEnumerable<Command> Commands
        {
            get
            {
                SdkError.ThrowIfError(device_available_commands(DevicePtr, out var nativeArray));
                var commands = new NativeArrayMarshaler<Command>().MarshalArray(nativeArray.CmdArray, nativeArray.CmdArraySize);
                free_CommandArray(nativeArray);
                return commands;
            }
        }

        public IEnumerable<ParamInfo> Parameters
        {
            get
            {
                SdkError.ThrowIfError(device_available_parameters(DevicePtr, out var nativeArray));
                var parameters = new NativeArrayMarshaler<ParamInfo>().MarshalArray(nativeArray.InfoArray, nativeArray.InfoCount);
                free_ParamInfoArray(nativeArray);
                return parameters;
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

        public void AddDoubleChannelDataListener(
            Action<object, ChannelData<double>> callback,
            ChannelInfo channelInfo)
        {
            if (_doubleChannelListenerHandles.ContainsKey(callback))
            {
                free_listener_handle(_doubleChannelListenerHandles[callback]);
            }

            SdkError.ThrowIfError(device_subscribe_double_channel_data_received(DevicePtr, channelInfo,
                _doubleDataReceivedFunc, out var dataReceivedListenerPtr, IntPtr.Zero));

            _doubleChannelListenerHandles[callback] = dataReceivedListenerPtr;
            DoubleChannelDataReceived += callback;
        }

        public void AddIntChannelDataListener(
            Action<object, ChannelData<int>> callback,
            ChannelInfo channelInfo)
        {
            if (_intChannelListenerHandles.ContainsKey(callback))
            {
                free_listener_handle(_intChannelListenerHandles[callback]);
            }

            SdkError.ThrowIfError(device_subscribe_int_channel_data_received(DevicePtr, channelInfo,
                _intDataReceivedFunc, out var dataReceivedListenerPtr, IntPtr.Zero));

            _intChannelListenerHandles[callback] = dataReceivedListenerPtr;
            IntChannelDataReceived += callback;
        }

        public void RemoveIntChannelDataListener(Action<object, ChannelData<int>> callback)
        {
            if (_intChannelListenerHandles.ContainsKey(callback))
            {
                free_listener_handle(_intChannelListenerHandles[callback]);
                _intChannelListenerHandles.Remove(callback);
            }
        }

        public void RemoveDoubleChannelDataListener(Action<object, ChannelData<double>> callback)
        {
            if (_doubleChannelListenerHandles.ContainsKey(callback))
            {
                free_listener_handle(_doubleChannelListenerHandles[callback]);
                _doubleChannelListenerHandles.Remove(callback);
            }
        }

        private void RemoveAllCahnnelListeners()
        {
            foreach (var listener in _intChannelListenerHandles.Values)
            {
                free_listener_handle(listener);
            }
            _intChannelListenerHandles.Clear();
            foreach (var listener in _doubleChannelListenerHandles.Values)
            {
                free_listener_handle(listener);
            }
            _doubleChannelListenerHandles.Clear();
        }

        private void OnParameterChanged(IntPtr devicePtr, Parameter parameter, IntPtr userData)
        {
            if (DevicePtr != devicePtr) return;
            ParameterChanged?.Invoke(this, parameter);
        }

        private void OnDoubleDataReceived(IntPtr devicePtr, ChannelInfo info, DoubleDataArray dataArray, IntPtr userData)
        {
            if (DevicePtr != devicePtr) return;
            var data = new NativeArrayMarshaler<double>().MarshalArray(dataArray.DoubleArray, dataArray.SamplesCount);
            free_DoubleDataArray(dataArray);
            DoubleChannelDataReceived?.Invoke(this, new ChannelData<double>(data, info));
        }

        private void OnIntDataReceived(IntPtr devicePtr, ChannelInfo info, IntDataArray dataArray, IntPtr userData)
        {
            if (DevicePtr != devicePtr) return;
            var data = new NativeArrayMarshaler<int>().MarshalArray(dataArray.IntArray, dataArray.SamplesCount);
            free_IntDataArray(dataArray);
            IntChannelDataReceived?.Invoke(this, new ChannelData<int>(data, info));
        }

        public class ChannelData<T>
        {
            public ChannelInfo ChannelInfo { get; }
            public T[] DataArray { get; }

            public ChannelData(T[] array, ChannelInfo info)
            {
                DataArray = array;
                ChannelInfo = info;
            }
        }

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_listener_handle(IntPtr listenerHandle);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_ChannelInfoArray(ChannelInfoArray channelInfoArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_ParamInfoArray(ParamInfoArray paramInfoArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_CommandArray(CommandArray commandArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_DoubleDataArray(DoubleDataArray doubleArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_IntDataArray(IntDataArray intArray);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceParamChangedFunc(IntPtr devicePtr, Parameter parameter, IntPtr userData);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceDoubleDataReceivedFunc(IntPtr devicePtr, ChannelInfo info, DoubleDataArray dataArray, IntPtr userData);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceIntDataReceivedFunc(IntPtr devicePtr, ChannelInfo info, IntDataArray dataArray, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_Device(DeviceInfo info);

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
        private static extern int device_subscribe_param_changed(IntPtr devicePtr, DeviceParamChangedFunc callback, out IntPtr outListenerPtr, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_subscribe_double_channel_data_received(IntPtr devicePtr, ChannelInfo info, DeviceDoubleDataReceivedFunc callback, out IntPtr outListenerPtr, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_subscribe_int_channel_data_received(IntPtr devicePtr, ChannelInfo info, DeviceIntDataReceivedFunc callback, out IntPtr outListenerPtr, IntPtr userData);

        [StructLayout(LayoutKind.Sequential)]
        private struct DoubleDataArray
        {
            public IntPtr DoubleArray;
            public IntPtr SamplesCount;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct IntDataArray
        {
            public IntPtr IntArray;
            public IntPtr SamplesCount;
        }
    }
}
