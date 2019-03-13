using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Neuro
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct EegIndex
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
        public string Name;
        public float FrequencyBottom;
        public float FrequencyTop;
    }

    public static class EegStandardIndices
    {
        public static EegIndex Alpha => new EegIndex(){Name = "Alpha", FrequencyBottom = 8.0f, FrequencyTop = 14.0f};
        public static EegIndex Beta => new EegIndex() { Name = "Beta", FrequencyBottom = 14.0f, FrequencyTop = 34.0f };
        public static EegIndex Delta => new EegIndex() { Name = "Delta", FrequencyBottom = .5f, FrequencyTop = 4.0f };
        public static EegIndex Theta => new EegIndex() { Name = "Theta", FrequencyBottom = 4.0f, FrequencyTop = 8.0f };
    }

    public class EegIndexChannel : IDataChannel<double>, IDisposable
    {
        private readonly IntPtr[] _channelsPointers;
        private EegArtifactChannel _artifactChannel;
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;


        public EegIndexChannel(EegIndex index, IEnumerable<IDataChannel<double>> channels, double windowDuration = 8.0, double overlappingCoeff = 0.9)
        {
            _channelsPointers = channels.Select(x => x.ChannelPtr).ToArray();

            _anyChannel = new AnyChannel(create_EegIndexDoubleChannel_channels(index, _channelsPointers, (IntPtr)_channelsPointers.Length, windowDuration, overlappingCoeff));
            _dataChannel = new DataChannel<double>(_anyChannel);
            _anyChannel.LengthChanged += (sender, length) => LengthChanged?.Invoke(sender, length);
        }

        public EegIndexChannel(EegIndex index, IEnumerable<IDataChannel<double>> channels, EegArtifactChannel artifactChannel, double windowDuration = 8.0, double overlappingCoeff = 0.9)
        {
            _channelsPointers = channels.Select(x => x.ChannelPtr).ToArray();
            _artifactChannel = artifactChannel;
            _anyChannel = new AnyChannel(create_EegIndexDoubleChannel_channels_artifacts(index, _channelsPointers, (IntPtr)_channelsPointers.Length, _artifactChannel.ChannelPtr, windowDuration, overlappingCoeff));
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

        public float SamplingFrequency => _anyChannel.SamplingFrequency;

        public IntPtr ChannelPtr => _anyChannel.ChannelPtr;

        public double[] ReadData(int offset, int length)
        {
            return _dataChannel.ReadData(offset, length);
        }


        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_EegIndexDoubleChannel_channels(EegIndex eegIndex, IntPtr[] channels, IntPtr channelsCount, double windowDuration, double overlappingCoeff);

        [DllImport(SdkLib.LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_EegIndexDoubleChannel_channels_artifacts(EegIndex eegIndex, IntPtr[] channels, IntPtr channelsCount, IntPtr artifactChannel, double windowDuration, double overlappingCoeff);
       
    }
}
