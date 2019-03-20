using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class Device : IDisposable
    {
        private readonly IntPtr _paramChangedListenerPtr;
        private readonly DeviceParamChangedFunc _paramChangedFunc;
        private readonly Dictionary<Action<object, ChannelData<double>>, List<IntPtr>> _doubleChannelListenerHandles = new Dictionary<Action<object, ChannelData<double>>, List<IntPtr>>();
        private readonly Dictionary<Action<object, SignalChannelData>, List<IntPtr>> _signalChannelListenerHandles = new Dictionary<Action<object, SignalChannelData>, List<IntPtr>>();
        private readonly Dictionary<Action<object, ChannelData<int>>, List<IntPtr>> _intChannelListenerHandles = new Dictionary<Action<object, ChannelData<int>>, List<IntPtr>>();
        private readonly DeviceDoubleDataReceivedFunc _doubleDataReceivedFunc;
        private readonly DeviceSignalDataReceivedFunc _signalDataReceivedFunc;
        private readonly DeviceIntDataReceivedFunc _intDataReceivedFunc;

        private event Action<object, ChannelData<double>> DoubleChannelDataReceived;
        private event Action<object, SignalChannelData> SignalChannelDataReceived;
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
            _signalDataReceivedFunc = OnSignalDataReceived;
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
            _signalDataReceivedFunc = OnSignalDataReceived;
            _intDataReceivedFunc = OnIntDataReceived;
        }

        private void ReleaseUnmanagedResources()
        {
            RemoveAllChannelListeners();
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
            if (!_doubleChannelListenerHandles.ContainsKey(callback))
            {
                _doubleChannelListenerHandles[callback] = new List<IntPtr>();
            }

            SdkError.ThrowIfError(device_subscribe_double_channel_data_received(DevicePtr, channelInfo,
                _doubleDataReceivedFunc, out var dataReceivedListenerPtr, IntPtr.Zero));

            _doubleChannelListenerHandles[callback].Add(dataReceivedListenerPtr);
            DoubleChannelDataReceived += callback;
        }

        public void AddSignalChannelDataListener(
            Action<object, SignalChannelData> callback,
            ChannelInfo channelInfo)
        {
            if (!_signalChannelListenerHandles.ContainsKey(callback))
            {
                _signalChannelListenerHandles[callback] = new List<IntPtr>();
            }

            SdkError.ThrowIfError(device_subscribe_signal_channel_data_received(DevicePtr, channelInfo,
                _signalDataReceivedFunc, out var dataReceivedListenerPtr, IntPtr.Zero));

            _signalChannelListenerHandles[callback].Add(dataReceivedListenerPtr);
            SignalChannelDataReceived += callback;
        }

        public void AddIntChannelDataListener(
            Action<object, ChannelData<int>> callback,
            ChannelInfo channelInfo)
        {
            if (!_intChannelListenerHandles.ContainsKey(callback))
            {
                _intChannelListenerHandles[callback] = new List<IntPtr>();
            }

            SdkError.ThrowIfError(device_subscribe_int_channel_data_received(DevicePtr, channelInfo,
                _intDataReceivedFunc, out var dataReceivedListenerPtr, IntPtr.Zero));

            _intChannelListenerHandles[callback].Add(dataReceivedListenerPtr);
            IntChannelDataReceived += callback;
        }

        public void RemoveIntChannelDataListener(Action<object, ChannelData<int>> callback)
        {
            if (_intChannelListenerHandles.ContainsKey(callback))
            {
                foreach (var handle in _intChannelListenerHandles[callback])
                {
                    free_listener_handle(handle);
                }
                _intChannelListenerHandles.Remove(callback);
            }
        }

        public void RemoveDoubleChannelDataListener(Action<object, ChannelData<double>> callback)
        {
            if (_doubleChannelListenerHandles.ContainsKey(callback))
            {
                foreach (var handle in _doubleChannelListenerHandles[callback])
                {
                    free_listener_handle(handle);
                }
                _doubleChannelListenerHandles.Remove(callback);
            }
        }

        public void RemoveSignalChannelDataListener(Action<object, SignalChannelData> callback)
        {
            if (_signalChannelListenerHandles.ContainsKey(callback))
            {
                foreach (var handle in _signalChannelListenerHandles[callback])
                {
                    free_listener_handle(handle);
                }
                _signalChannelListenerHandles.Remove(callback);
            }
        }

        private void RemoveAllChannelListeners()
        {
            foreach (var listener in _intChannelListenerHandles.Values)
            {
                foreach (var handle in listener)
                {
                    free_listener_handle(handle);
                }
            }

            _intChannelListenerHandles.Clear();
            foreach (var listener in _doubleChannelListenerHandles.Values)
            {
                foreach (var handle in listener)
                {
                    free_listener_handle(handle);
                }
            }
            _doubleChannelListenerHandles.Clear();

            foreach (var listener in _signalChannelListenerHandles.Values)
            {
                foreach (var handle in listener)
                {
                    free_listener_handle(handle);
                }
            }
            _signalChannelListenerHandles.Clear();
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

        private void OnSignalDataReceived(IntPtr devicePtr, ChannelInfo info, SignalDataArray dataArray, IntPtr userData)
        {
            if (DevicePtr != devicePtr) return;
            var data = new NativeArrayMarshaler<double>().MarshalArray(dataArray.DoubleArray, dataArray.SamplesCount);
            free_SignalDataArray(dataArray);
            SignalChannelDataReceived?.Invoke(this, new SignalChannelData(data, info, dataArray.FirstSampleCount));
        }

        private void OnIntDataReceived(IntPtr devicePtr, ChannelInfo info, IntDataArray dataArray, IntPtr userData)
        {
            if (DevicePtr != devicePtr) return;
            var data = new NativeArrayMarshaler<int>().MarshalArray(dataArray.IntArray, dataArray.SamplesCount);
            free_IntDataArray(dataArray);
            IntChannelDataReceived?.Invoke(this, new ChannelData<int>(data, info));
        }

        public class SignalChannelData
        {
            public ChannelInfo ChannelInfo { get; }
            public double[] DataArray { get; }
            public ushort FirstSampleNumber { get; }

            public SignalChannelData(double[] array, ChannelInfo info, ushort firstSampleNumber)
            {
                DataArray = array;
                ChannelInfo = info;
                FirstSampleNumber = firstSampleNumber;
            }
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
        private static extern void free_SignalDataArray(SignalDataArray signalArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_IntDataArray(IntDataArray intArray);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceParamChangedFunc(IntPtr devicePtr, Parameter parameter, IntPtr userData);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceDoubleDataReceivedFunc(IntPtr devicePtr, ChannelInfo info, DoubleDataArray dataArray, IntPtr userData);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceSignalDataReceivedFunc(IntPtr devicePtr, ChannelInfo info, SignalDataArray dataArray, IntPtr userData);

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
        private static extern int device_subscribe_signal_channel_data_received(IntPtr devicePtr, ChannelInfo info, DeviceSignalDataReceivedFunc callback, out IntPtr outListenerPtr, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_subscribe_int_channel_data_received(IntPtr devicePtr, ChannelInfo info, DeviceIntDataReceivedFunc callback, out IntPtr outListenerPtr, IntPtr userData);

        [StructLayout(LayoutKind.Sequential)]
        private struct DoubleDataArray
        {
            public IntPtr DoubleArray;
            public IntPtr SamplesCount;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct SignalDataArray
        {
            public IntPtr DoubleArray;
            public IntPtr SamplesCount;
            public ushort FirstSampleCount;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct IntDataArray
        {
            public IntPtr IntArray;
            public IntPtr SamplesCount;
        }
    }
}
