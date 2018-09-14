using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public class NativeAdapter
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

        public static T[] MarshalArray<T>(IntPtr firstElemPtr, UIntPtr length, NativePtrReader<T> reader)
        {
            var result = new T[length.ToUInt32()];
            var type = typeof(T).IsEnum ? typeof(T).GetEnumUnderlyingType() : typeof(T);
            for (uint i = 0; i < length.ToUInt32(); i++)
            {
                result[i] = reader(firstElemPtr);
                firstElemPtr = IntPtr.Add(firstElemPtr, Marshal.SizeOf(type));
            }

            return result;
        }
    }
}
