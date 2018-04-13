#ifndef BLUETOOTH_SCANNER_Z_H
#define BLUETOOTH_SCANNER_Z_H

#include "ble_scanner.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>

#include <condition_variable>
#include <mutex>
#include <atomic>


//#include <bluetooth/hci.h>

class DeviceScannerZ:public BleScanner
{
public:
    DeviceScannerZ();
    ~DeviceScannerZ();
    void startScan();
    void stopScan();
    std::unique_ptr<BleDevice> getDeviceByAddress(std::string);
    void setFilter(std::vector<std::string>);
    void subscribeDeviceFound(std::function<void(std::unique_ptr<BleDevice>)>);
    void releaseDevice(std::string name, std::string address);
    bool isScanning();
private:
    using device_info = std::pair<std::string, std::string>;
    std::mutex scanMutex;
    std::condition_variable scanCV;
    std::atomic<bool> scanning;
    std::vector<device_info> devices;
    std::vector<std::string> filter;

    int devId = -1;
    int devDescriptor = -1;
    std::function<void(std::unique_ptr<BleDevice>)> subscribeDeviceFoundProcessor;

    hci_filter initScan();
    void deinitScan(struct hci_filter &);
    void bleLocalOpen();
    void bleLocalClose();
    bool isBleLocalOpen();

    bool scanProcess();
    bool scanProcessDataRead(int&, unsigned char*, size_t);
    bool scanProcessDataParse(int&, unsigned char*, std::string &, std::string &);
    std::unique_ptr<BleDevice> findDeviceByAddress(const std::string &);

    bool pushDevIfNotPresent(const std::string &, const std::string &);
};

class BleZErrorOpen : public std::exception
{
public:
    explicit BleZErrorOpen(const char* message):
        msgError(message) {}
    explicit BleZErrorOpen(const std::string& message):
        msgError(message) {}
    virtual ~BleZErrorOpen() throw (){}

    virtual const char* what() const throw (){
        return msgError.c_str();
    }

private:
    std::string msgError;
};

class BleZErrorNonBlocking : public std::exception
{
public:
    explicit BleZErrorNonBlocking(const char* message):
        msgError(message) {}
    explicit BleZErrorNonBlocking(const std::string& message):
        msgError(message) {}
    virtual ~BleZErrorNonBlocking() throw (){}

    virtual const char* what() const throw (){
        return msgError.c_str();
    }

private:
    std::string msgError;
};

class BleZErrorScan : public std::exception
{
public:
    explicit BleZErrorScan(const char* message):
        msgError(message) {}
    explicit BleZErrorScan(const std::string& message):
        msgError(message) {}
    virtual ~BleZErrorScan() throw (){}

    virtual const char* what() const throw (){
        return msgError.c_str();
    }

private:
    std::string msgError;
};

#endif // BLUETOOTH_SCANNER_Z_H
