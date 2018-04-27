package ru.neurotech.neurosdk.channels;

/**
 * Represents information about channel
 * Channel info classes could be embedded and custom
 */
public interface ChannelInfo {

    /**
     * Returns name of a channel
     * @return Channel name
     */
    String getName();

    /**
     * Sets custom name for channel
     * @param name New channel name
     */
    void setName(String name);

    /**
     * Returns type of channel
     * @return Channel type
     */
    ChannelType getType();
    
    /**
     * Returns index number of channel for devices with multiple channels
     * @return channel index
     */
    long getIndex();
}
