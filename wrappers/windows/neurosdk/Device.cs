using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;

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
            device_subscribe_param_changed(_devicePtr, (Parameter param) => { ParameterChanged?.Invoke(this, param); });
        }

        public void Connect()
        {
            var result = device_connect(_devicePtr);
            switch (result)
            {
                case SdkError.ERROR_EXCEPTION_WITH_MESSAGE:
                    throw new InvalidOperationException(SdkError.sdk_last_error_msg());
                case SdkError.ERROR_UNHANDLED_EXCEPTION:
                    throw new InvalidOperationException("Unknown error");
                default:
                    break;
            }
        }

        public void Disconnect()
        {
            var result = device_disconnect(_devicePtr);
            switch (result)
            {
                case SdkError.ERROR_EXCEPTION_WITH_MESSAGE:
                    throw new InvalidOperationException(SdkError.sdk_last_error_msg());
                case SdkError.ERROR_UNHANDLED_EXCEPTION:
                    throw new InvalidOperationException("Unknown error");
                default:
                    break;
            }
        }

        public IEnumerable<ChannelInfo> Channels()
        {

        }

        public IEnumerable<Command> Commands()
        {

        }

        public IEnumerable<ParamInfo> Parameters()
        {

        }

        public void Execute(Command command)
        {
            var result = device_execute(_devicePtr, command);
            switch (result)
            {
                case SdkError.ERROR_EXCEPTION_WITH_MESSAGE:
                    throw new InvalidOperationException(SdkError.sdk_last_error_msg());
                case SdkError.ERROR_UNHANDLED_EXCEPTION:
                    throw new InvalidOperationException("Unknown error");
                default:
                    break;
            }
        }

        public T ReadParam<T>(Parameter parameter)
        {

        }

        public void SetParam(object value)
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
        private static extern int device_available_channels(IntPtr devicePtr, ChannelInfoArray*);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_commands(IntPtr devicePtr, CommandArray*);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_available_parameters(IntPtr devicePtr, ParamInfoArray*);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_execute(IntPtr devicePtr, Command command);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_subscribe_param_changed(IntPtr devicePtr, DeviceParamChangedFunc callback);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Name(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] out string outName);

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
