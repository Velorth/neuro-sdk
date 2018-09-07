using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Neuro
{
    public static class SdkError
    {
        public const int SdkNoError = 0;
        public const int ErrorUnhandledException = 1;
        public const int ErrorExceptionWithMessage = 2;

        public static string LastErrorMessage
        {
            get
            {
                var messageBuffer = new StringBuilder(256);
                var result = sdk_last_error_msg(messageBuffer, (uint)messageBuffer.Capacity);
                if (result != SdkNoError)
                {
                    throw new InvalidOperationException("Failed to read last error message");
                }
                return messageBuffer.ToString();
            }
        }

        /// <summary>
        /// Checks result code of native operation and throws in case operation failed
        /// </summary>
        /// <param name="resultCode">Native result code</param>
        public static void ThrowIfError(int resultCode)
        {
            switch (resultCode)
            {
                case ErrorExceptionWithMessage:
                    throw new InvalidOperationException(LastErrorMessage);
                case ErrorUnhandledException:
                    throw new InvalidOperationException("Unhandled exception in native code");
                case SdkNoError:
                    break;
                default:
                    throw new InvalidOperationException("Uknown error code or unknown exception");
            }
        }

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif
        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern int sdk_last_error_msg(StringBuilder msgBuffer, uint bufferLength);
    }
}
