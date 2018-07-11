using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    internal class NativeAdapter
    {
        public delegate T NativePtrReader<T>(IntPtr nativePtr);

        public static T NativeStructPtrReader<T>(IntPtr nativePtr) where T : struct
        {
            return Marshal.PtrToStructure<T>(nativePtr);
        }

        public static T NativeEnumPtrReader<T>(IntPtr nativePtr) where T : IConvertible
        {
            return (T)(object)Marshal.ReadInt32(nativePtr);
        }

        public static T[] MarshalArray<T>(IntPtr structArrayPtr, uint length, NativePtrReader<T> reader)
        {
            var result = new T[length];
            for (var i = 0; i < length; i++)
            {
                structArrayPtr = IntPtr.Add(structArrayPtr, Marshal.SizeOf<T>());
                result[i] = reader(structArrayPtr);
            }

            return result;
        }
    }
}
