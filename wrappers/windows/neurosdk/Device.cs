using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace Neuro
{
    public class Device
    {
        private readonly IntPtr _devicePtr;
        public event EventHandler<Parameter> ParameterChanged;

        internal Device(IntPtr devicePtr)
        {
            _devicePtr = devicePtr;
            Debug.Assert(_devicePtr != null);
            if (_devicePtr == null)
            {
                throw new ArgumentNullException(nameof(devicePtr), "Device pointer is null");
            }

            var result = device_subscribe_param_changed(_devicePtr, OnParameterChanged);
            Debug.Assert(result == SdkError.SdkNoError);
            SdkError.ThrowIfError(result);
        }

        ~Device()
        {
            device_delete(_devicePtr);
        }

        public void Connect()
        {
            SdkError.ThrowIfError(device_connect(_devicePtr));
        }

        public void Disconnect()
        {
            SdkError.ThrowIfError(device_disconnect(_devicePtr));
        }

        public IEnumerable<ChannelInfo> Channels()
        {
            SdkError.ThrowIfError(device_available_channels(_devicePtr, out var nativeArray));
            return NativeAdapter.MarshalArray(nativeArray.InfoArray, nativeArray.InfoCount, NativeAdapter.NativeStructPtrReader<ChannelInfo>);
        }

        public IEnumerable<Command> Commands()
        {
            SdkError.ThrowIfError(device_available_commands(_devicePtr, out var nativeArray));
            return NativeAdapter.MarshalArray(nativeArray.CmdArray, nativeArray.CmdArraySize, NativeAdapter.NativeEnumPtrReader<Command>);
        }

        public IEnumerable<ParamInfo> Parameters()
        {
            SdkError.ThrowIfError(device_available_parameters(_devicePtr, out var nativeArray));
            return NativeAdapter.MarshalArray(nativeArray.InfoArray, nativeArray.InfoCount, NativeAdapter.NativeStructPtrReader<ParamInfo>);
        }

        public void Execute(Command command)
        {
            SdkError.ThrowIfError(device_execute(_devicePtr, command));
        }

        public T ReadParam<T>(Parameter parameter)
        {
            var paramTypeInfo = new ParameterTypeInfo(parameter);
            if (paramTypeInfo.Type != typeof(T))
            {
                throw new ArgumentException($"Wrong return generic type argument. Must be {paramTypeInfo.Type.Name}");
            }
            return (T) new object();
        }

        public void SetParam<T>(Parameter parameter, T value)
        {

        }

        private void OnParameterChanged(Parameter parameter)
        {

        }

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceParamChangedFunc(Parameter parameter);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_connect(IntPtr devicePtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_disconnect(IntPtr devicePtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void device_delete(IntPtr devicePtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_channels(IntPtr devicePtr, out ChannelInfoArray infoArray);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_commands(IntPtr devicePtr, out CommandArray cmdArray);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_parameters(IntPtr devicePtr, out ParamInfoArray infoArray);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_execute(IntPtr devicePtr, Command command);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_subscribe_param_changed(IntPtr devicePtr, DeviceParamChangedFunc callback);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Name(IntPtr devicePtr, StringBuilder outName, uint bufferLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_State(IntPtr devicePtr, out DeviceState outState);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Address(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] out string outAddress);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_SerialNumber(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] out string outSerial);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_HardwareFilterState(IntPtr devicePtr, out bool outIsEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_FirmwareMode(IntPtr devicePtr, out FirmwareMode outMode);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_SamplingFrequency(IntPtr devicePtr, out SamplingFrequency outFreq);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Gain(IntPtr devicePtr, out Gain outGain);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Offset(IntPtr devicePtr, out byte outOffset);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_ExternalSwitchState(IntPtr devicePtr, out ExternalSwitchInput outExtSwitch);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_ADCInputState(IntPtr devicePtr, out ADCInput outAdcInput);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_AccelerometerSens(IntPtr devicePtr, out AccelerometerSensitivity outAccelSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_GyroscopeSens(IntPtr devicePtr, out GyroscopeSensitivity outGuroSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_StimulatorState(IntPtr devicePtr, out bool outIsEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_MotionAssistantState(IntPtr devicePtr, out bool outIsEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_StimulatorParamPack(IntPtr devicePtr, out StimulationParams outStimulParams);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_MotionAssistantParamPack(IntPtr devicePtr, out MotionAssistantParams outMaParams);



        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Name(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] string name);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_State(IntPtr devicePtr, DeviceState state);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Address(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] string address);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_SerialNumber(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] string serial);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_HardwareFilterState(IntPtr devicePtr, bool isEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_FirmwareMode(IntPtr devicePtr, FirmwareMode mode);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_SamplingFrequency(IntPtr devicePtr, SamplingFrequency freq);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Gain(IntPtr devicePtr, Gain gain);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Offset(IntPtr devicePtr, byte offset);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_ExternalSwitchState(IntPtr devicePtr, ExternalSwitchInput extSwitch);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_ADCInputState(IntPtr devicePtr, ADCInput adcInput);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_AccelerometerSens(IntPtr devicePtr, AccelerometerSensitivity accelSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_GyroscopeSens(IntPtr devicePtr, GyroscopeSensitivity gyroSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_StimulatorState(IntPtr devicePtr, bool isEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_MotionAssistantState(IntPtr devicePtr, bool isEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_StimulatorParamPack(IntPtr devicePtr, StimulationParams stimulParams);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_MotionAssistantParamPack(IntPtr devicePtr, MotionAssistantParams maParams);
    }
}
