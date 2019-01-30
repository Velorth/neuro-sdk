﻿using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    internal class NativeArrayMarshaler<T>
    {
        private readonly IMarshaler _marshaler;

        internal NativeArrayMarshaler()
        {
            _marshaler = typeof(T).IsPrimitive ? (IMarshaler) new PrimitiveMarshaller() : new StructEnumMarshaller();
        }

        public T[] MarshalArray(IntPtr firstElementPtr, UIntPtr elementCount)
        {
            return _marshaler.MarshalArray(firstElementPtr, elementCount);
        }

        private interface IMarshaler
        {
            T[] MarshalArray(IntPtr firstElementPtr, UIntPtr elementCount);
        }

        private class PrimitiveMarshaller : IMarshaler
        {
            private delegate T[] PrimitiveArrayCopyDelegate(IntPtr firstElementPtr, UIntPtr elementCount);

            private readonly PrimitiveArrayCopyDelegate _copyFunction;

            public PrimitiveMarshaller()
            {
                _copyFunction = GetCopyFunctionForType(typeof(T));
            }

            public T[] MarshalArray(IntPtr firstElementPtr, UIntPtr elementCount)
            {
                var resultArray = _copyFunction(firstElementPtr, elementCount);
                return resultArray ??
                       throw new ArrayTypeMismatchException("Wrong result buffer type after native buffer marshaling");
            }

            private static PrimitiveArrayCopyDelegate GetCopyFunctionForType(Type type)
            {
                if (type == typeof(int))
                {
                    return CopyInt;
                }
                if (type == typeof(double))
                {
                    return CopyDouble;
                }

                throw new ArgumentException($"There is no reader function for type {type.Name}");
            }

            private static T[] CopyDouble(IntPtr firstElementPtr, UIntPtr elementCount)
            {
                var buffer = new double[elementCount.ToUInt32()];
                Marshal.Copy(firstElementPtr, buffer, 0, (int)elementCount);
                return buffer as T[];
            }

            private static T[] CopyInt(IntPtr firstElementPtr, UIntPtr elementCount)
            {
                var buffer = new int[elementCount.ToUInt32()];
                Marshal.Copy(firstElementPtr, buffer, 0, (int)elementCount);
                return buffer as T[];
            }
            
        }

        private class StructEnumMarshaller : IMarshaler
        {
            private delegate T NativePtrReader(IntPtr nativePtr);
            private readonly NativePtrReader _ptrReaderFunc;

            public StructEnumMarshaller()
            {
                _ptrReaderFunc = typeof(T).IsEnum ? (NativePtrReader)NativeEnumPtrReader : NativeStructPtrReader;
            }

            public T[] MarshalArray(IntPtr firstElementPtr, UIntPtr elementCount)
            {
                var result = new T[elementCount.ToUInt32()];

                var type = typeof(T).IsEnum ? typeof(T).GetEnumUnderlyingType() : typeof(T);
                for (uint i = 0; i < elementCount.ToUInt32(); i++)
                {
                    result[i] = _ptrReaderFunc(firstElementPtr);
                    firstElementPtr = IntPtr.Add(firstElementPtr, Marshal.SizeOf(type));
                }

                return result;
            }

            private static T NativeStructPtrReader(IntPtr nativePtr)
            {
                return Marshal.PtrToStructure<T>(nativePtr);
            }

            private static T NativeEnumPtrReader(IntPtr nativePtr)
            {
                return (T)(object)Marshal.ReadInt32(nativePtr);
            }
        }
    }
}