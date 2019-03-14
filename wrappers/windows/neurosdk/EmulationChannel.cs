using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Neuro
{
    [StructLayout(LayoutKind.Sequential)]
    public struct EmulationSine
    {
        public double AmplitudeV;
        public double FrequencyHz;
        public double PhaseShiftRad;
    }

    public class EmulationChannel : IDataChannel<double>, IDisposable
    {
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;

        public EmulationChannel(IEnumerable<EmulationSine> components, float samplingFrequency)
        {
            var componentArray = components.ToArray();
            _anyChannel = new AnyChannel(create_EmulationDoubleChannel(componentArray, (IntPtr)componentArray.Length, samplingFrequency));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public void Dispose()
        {
            _anyChannel.Dispose();
        }

        public event EventHandler<int> LengthChanged;

        public ChannelInfo Info => _anyChannel.Info;

        public int TotalLength => _anyChannel.TotalLength;

        public void StartTimer()
        {
           SdkError.ThrowIfError(EmulationDoubleChannel_start_timer(ChannelPtr));
        }

        public void StopTimer()
        {
            SdkError.ThrowIfError(EmulationDoubleChannel_stop_timer(ChannelPtr));
        }

        public void Reset()
        {
            SdkError.ThrowIfError(EmulationDoubleChannel_reset(ChannelPtr));
        }

        public float SamplingFrequency => _anyChannel.SamplingFrequency;

        public IntPtr ChannelPtr => _anyChannel.ChannelPtr;

        public double[] ReadData(int offset, int length)
        {
            return _dataChannel.ReadData(offset, length);
        }

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_EmulationDoubleChannel(EmulationSine[] components, IntPtr componentsCount, float samplingFrequency);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EmulationDoubleChannel_start_timer(IntPtr emulationChannelPtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EmulationDoubleChannel_stop_timer(IntPtr emulationChannelPtr);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int EmulationDoubleChannel_reset(IntPtr emulationChannelPtr);
    }
}
