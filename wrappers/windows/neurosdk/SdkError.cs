using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Neuro
{
    public static class SdkError
    {
        public const int SDK_NO_ERROR = 0;
        public const int ERROR_UNHANDLED_EXCEPTION = 1;
        public const int ERROR_EXCEPTION_WITH_MESSAGE = 2;

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif
        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        public static extern string sdk_last_error_msg();
    }
}
