using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    class DeviceScanner
    {
        private readonly IntPtr _scannerPtr = create_device_scanner();

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif
        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_device_scanner();
    }
}
