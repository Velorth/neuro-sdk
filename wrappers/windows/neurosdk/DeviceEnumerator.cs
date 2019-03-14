using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Neuro
{
    [StructLayout(LayoutKind.Sequential)]
    public struct DeviceInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string Name;
        public ulong Address;
    }

    public enum DeviceType
    {
        Brainbit,
        Callibri,
        Any
    }

    public class DeviceEnumerator : IDisposable
    {
        private readonly IntPtr _enumeratorPtr;
        private readonly IntPtr _deviceListChangedListenerPtr;
        private readonly DeviceListChangedCallbackFunc _deviceListChangedCallbackFunc;

        public event EventHandler DeviceListChanged;

        public DeviceEnumerator(DeviceType deviceType)
        {
            _enumeratorPtr = create_device_enumerator(deviceType);
            if (_enumeratorPtr == null)
            {
                throw new InvalidOperationException(SdkError.LastErrorMessage);
            }

            _deviceListChangedCallbackFunc = OnDeviceListChanged;
            SdkError.ThrowIfError(enumerator_set_device_list_changed_callback(_enumeratorPtr, _deviceListChangedCallbackFunc, out _deviceListChangedListenerPtr, IntPtr.Zero));
        }

        private void ReleaseUnmanagedResources()
        {
            free_listener_handle(_deviceListChangedListenerPtr);
            enumerator_delete(_enumeratorPtr);
        }

        public void Dispose()
        {
            ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        ~DeviceEnumerator()
        {
            ReleaseUnmanagedResources();
        }

        public IList<DeviceInfo> Devices
        {
            get
            {
                SdkError.ThrowIfError(enumerator_get_device_list(_enumeratorPtr, out var nativeArray));
                var deviceInfoArray = new NativeArrayMarshaler<DeviceInfo>().MarshalArray(nativeArray.InfoArray, nativeArray.InfoCount);
                free_DeviceInfoArray(nativeArray);
                return deviceInfoArray;
            }
        }

        private void OnDeviceListChanged(IntPtr enumerator, IntPtr userData)
        {
            if (enumerator != _enumeratorPtr)
                return;

            DeviceListChanged?.Invoke(this, null);
        }

        [StructLayout(LayoutKind.Sequential)]
        internal struct DeviceInfoArray
        {
            public IntPtr InfoArray;
            public IntPtr InfoCount;
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceListChangedCallbackFunc(IntPtr enumerator, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_device_enumerator(DeviceType type);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void enumerator_delete(IntPtr enumeratorPtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int enumerator_set_device_list_changed_callback(IntPtr enumeratorPtr, DeviceListChangedCallbackFunc deviceFoundCallback, out IntPtr listenerHandle, IntPtr userData);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int enumerator_get_device_list(IntPtr enumeratorPtr, out DeviceInfoArray deviceInfoArray);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_listener_handle(IntPtr listenerHandle);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void free_DeviceInfoArray(DeviceInfoArray deviceInfoArray);
    }
}
