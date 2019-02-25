package com.neuromd.neurosdk.channels;

import com.neuromd.common.SubscribersNotifier;
import com.neuromd.neurosdk.Device;


/**
 * Base channel class is intended for creating various channels of device and for build flexible
 * system with different signal processing classes representing their data in same way.
 * Inherit from this class to create you own channel
 * @param <SampleType> Specifies type of data samples stored in channel buffer
 */
public abstract class BaseChannel {

    /**
     * Subscribe this event to receive notifications about changes of total length of data
     */
    public final SubscribersNotifier<Long> dataLengthChanged = new SubscribersNotifier<>();


    public abstract long channelPtr();

    /**
     * Returns channel information
     * @return Channel info
     */
    public abstract ChannelInfo info();

    /**
     * Returns total length of data been added to channel buffer. This value indicates how much data
     * SHOULD be in buffer, but does not guarantee that all of these data samples are actually
     * in buffer. How many data samples are stored in buffer, whether they saved on hard disc or not
     * is depends on implementation of channel class. Basically channel data samples are stored in
     * RAM and after totalLength exceeds buffer size, old data are discarded and cannot be restored.
     * In this case actual data length stored in buffer could be clarified by calling
     * bufferSize() method. For channels which do not have internal buffer or save data on disc
     * return values of this method and bufferSize method will be equal and no data samples are
     * discarded. To check whether channel has buffer overflow or not call bufferSize() method
     * @return Total data length been added to channel buffer
     */
    public abstract long totalLength();

    /**
     * Returns sampling frequency of data returned by readData() method
     * @return Sampling frequency of data
     */
    public abstract float samplingFrequency();
}
