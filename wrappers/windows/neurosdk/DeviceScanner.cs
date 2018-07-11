using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class DeviceScanner
    {
        private readonly IntPtr _scannerPtr = create_device_scanner();

        public event EventHandler<bool> ScanStateChanged;
        public event EventHandler<Device> DeviceFound; 

        public DeviceScanner()
        {
            Debug.Assert(_scannerPtr != null);
            if (_scannerPtr == null)
            {
                throw new SystemException("Unable to retreive BLE device scanner instance");
            }

            SdkError.ThrowIfError(scanner_set_scan_state_callback(_scannerPtr, isScanning => ScanStateChanged?.Invoke(this, isScanning)));
            SdkError.ThrowIfError(scanner_set_device_found_callback(_scannerPtr, OnDeviceFound));
        }

        ~DeviceScanner()
        {
            scanner_delete(_scannerPtr);
        }

        public void StartScan(int timeoutMs = 0)
        {
            SdkError.ThrowIfError(scanner_start_scan(_scannerPtr, timeoutMs));
        }

        public void StopScan()
        {
            SdkError.ThrowIfError(scanner_stop_scan(_scannerPtr));
        }

        public Device GetDeviceByAddress(string address)
        {
            return new Device(scanner_get_device_by_address(_scannerPtr, address));
        }

        public void ReleaseDevice(string name, string address)
        {
            SdkError.ThrowIfError(scanner_release_device(_scannerPtr, name, address));
        }

        private void OnDeviceFound(IntPtr devicePtr)
        {
            try
            {
                var device = new Device(devicePtr);
                DeviceFound?.Invoke(this, device);
            }
            catch (Exception e)
            {
                //TODO log or something else...
            }
        }

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DeviceFoundCallbackFunc(IntPtr parameter);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void ScanStateCallbackFunc(bool isScanning);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_device_scanner();

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void scanner_delete(IntPtr scannerPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int scanner_start_scan(IntPtr scannerPtr, int timeoutMs);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int scanner_stop_scan(IntPtr scannerPtr);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int scanner_set_device_found_callback(IntPtr scannerPtr, DeviceFoundCallbackFunc deviceFoundCallback);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int scanner_set_scan_state_callback(IntPtr scannerPtr, ScanStateCallbackFunc scanStateCallback);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr scanner_get_device_by_address(IntPtr scannerPtr, [MarshalAs(UnmanagedType.LPStr)] string address);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int scanner_release_device(IntPtr scannerPtr, [MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.LPStr)] string address);
    }
}
