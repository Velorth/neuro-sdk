﻿using System;
using System.Runtime.InteropServices;

namespace Neuro
{
    public sealed class BipolarDoubleChannel : IDataChannel<double>, IDisposable
    {
        private readonly IDataChannel<double> _firstSourceChannel; //store source channels reference to prevent its deletion
        private readonly IDataChannel<double> _secondSourceChannel;
        private readonly AnyChannel _anyChannel;
        private readonly DataChannel<double> _dataChannel;

        public BipolarDoubleChannel(IDataChannel<double> first, IDataChannel<double> second)
        {
            _firstSourceChannel = first;
            _secondSourceChannel = second;
            _anyChannel = new AnyChannel(create_BipolarDoubleChannel(first.ChannelPtr, second.ChannelPtr));
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
        private static extern IntPtr create_BipolarDoubleChannel(IntPtr firstPtr, IntPtr secondPtr);
        
    }
}
