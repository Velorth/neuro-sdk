#include "device/brainbit_device.h"
/*
BrainbitSignalSubsystem::BrainbitSignalSubsystem(BrainbitDevice *ds):
    brainbitSource(ds),
    signalReaderT3(std::unique_ptr<SignalBuffer>(new SignalBuffer(BRAINBIT_BUFFER_DURATION*BRAINBIT_SAMPLE_FREQ))),
    signalReaderT4(std::unique_ptr<SignalBuffer>(new SignalBuffer(BRAINBIT_BUFFER_DURATION*BRAINBIT_SAMPLE_FREQ))),
    signalReaderO1(std::unique_ptr<SignalBuffer>(new SignalBuffer(BRAINBIT_BUFFER_DURATION*BRAINBIT_SAMPLE_FREQ))),
    signalReaderO2(std::unique_ptr<SignalBuffer>(new SignalBuffer(BRAINBIT_BUFFER_DURATION*BRAINBIT_SAMPLE_FREQ))){

    channels.push_back(std::make_shared<Channel>(0, "T3", signalReaderT3));
    channels.push_back(std::make_shared<Channel>(1, "T4", signalReaderT4));
    channels.push_back(std::make_shared<Channel>(2, "O1", signalReaderO1));
    channels.push_back(std::make_shared<Channel>(3, "O2", signalReaderO2));
}

void BrainbitSignalSubsystem::startReceive(){

    brainbitSource->sendStartReceive();
}

void BrainbitSignalSubsystem::stopReceive(){
    brainbitSource->stopAll();
}

void BrainbitSignalSubsystem::setSamplingFrequency(int){

}

int BrainbitSignalSubsystem::getSamplingFrequency(){
    return BRAINBIT_SAMPLE_FREQ;
}

std::vector<int> BrainbitSignalSubsystem::getSupportedFrequencies() const {
    return {BRAINBIT_SAMPLE_FREQ};
}

int BrainbitSignalSubsystem::getOffset() const {
    return 3;
}

int BrainbitSignalSubsystem::getGain() const {
    return 6;
}

void BrainbitSignalSubsystem::setOffset(int) {

}

void BrainbitSignalSubsystem::setGain(int){

}

void BrainbitSignalSubsystem::onDataReceived(std::vector<SignalSample> &data){

    signalReaderT4.onDataReceived({data[0], data[4]});
    signalReaderO2.onDataReceived({data[1], data[5]});
    signalReaderT3.onDataReceived({data[2], data[6]});
    signalReaderO1.onDataReceived({data[3], data[7]});

}

bool BrainbitSignalSubsystem::getElectrodeState() {

    return true;
}
*/
