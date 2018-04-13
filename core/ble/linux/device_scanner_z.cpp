#include "device_scanner_z.h"

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "logger.h"

#include "ble_device_z.h"
#include <bluetooth/hci_lib.h>

#include <signal.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>


#define FLAGS_AD_TYPE 0x01
#define FLAGS_LIMITED_MODE_BIT 0x01
#define FLAGS_GENERAL_MODE_BIT 0x02

#define EIR_NAME_SHORT              0x08  /* shortened local name */
#define EIR_NAME_COMPLETE           0x09  /* complete local name */

static volatile int signalReceived = 0; // TODO

using std::string;
using std::vector;
using std::shared_ptr;
using std::function;


static int check_report_filter(uint8_t procedure, le_advertising_info *info);
static int read_flags(uint8_t *flags, const uint8_t *data, size_t size);
static bool eirFillName(uint8_t *eir, size_t eir_len,
                        char *buf, size_t buf_len);


DeviceScannerZ::DeviceScannerZ():
    scanning(false),
    subscribeDeviceFoundProcessor(nullptr)
{
}

DeviceScannerZ::~DeviceScannerZ()
{
    try {
        stopScan();
    } catch (...) {
        // ignore
    }
    bleLocalClose();
}

void DeviceScannerZ::startScan()
{
    emulator.startScan();
    if(this->isScanning()) {
        return;
    }
    scanCV.notify_all();
    struct hci_filter of = initScan();
    devices.clear();

    scanning.store(true);
    std::thread threadScan([this, &of] {
        std::lock_guard<std::mutex> lk(scanMutex);
        scanProcess();
        deinitScan(of);
        bleLocalClose();
        scanCV.notify_all();
    });
    threadScan.detach();
}

void DeviceScannerZ::stopScan()
{
    emulator.stopScan();
    if(!isScanning()) {
        return;
    }
    scanCV.notify_one();
    scanning.store(false);
    std::unique_lock<std::mutex> lk(scanMutex);
    scanCV.wait_for(lk, std::chrono::milliseconds(500));
//    scanCV.wait(lk);
}
auto findeByAddressInV(const std::vector<std::pair<std::string, std::string>> &devices, const string &address)->typename std::remove_reference<decltype(devices)>::type::const_iterator
{

        auto pred = [&](const std::pair<std::string, std::string> & item) {
            return item.second == address;
        };
        return std::find_if(devices.begin(), devices.end(), pred);
}

std::unique_ptr<BleDevice> DeviceScannerZ::getDeviceByAddress(string address)
{
    auto emulatedDevice = emulator.getDeviceByAddress(address);
    if (emulatedDevice)
        return emulatedDevice;

    if(address.empty())
        return nullptr;
    // search local
    auto devItm = findeByAddressInV(devices, address);
    if(devItm != devices.end())
        return std::unique_ptr<BleDevice>(new BleDeviceZ(devItm->first, devItm->second));
    // search
    if(this->isScanning()) // break
        return std::unique_ptr<BleDevice>();

    struct hci_filter of = initScan();
    scanning.store(true);
    std::future<std::unique_ptr<BleDevice>> asyncSearch = std::async(std::launch::async, [this, &of, &address](){
        std::unique_ptr<BleDevice> devRes = findDeviceByAddress(address);
        deinitScan(of);
        bleLocalClose();
        return devRes;
    });
    int tryCount = 5;
    while(asyncSearch.wait_for(std::chrono::milliseconds(250)) != std::future_status::ready
          && tryCount != 0)
    {
        scanning.store(false);
        tryCount--;
    }
    scanning.store(false);
    return asyncSearch.get();
}

void DeviceScannerZ::setFilter(vector<string> filterNames)
{
    emulator.setFilter(filterNames);

    this->filter = filterNames;
}

void DeviceScannerZ::subscribeDeviceFound(function<void (std::unique_ptr<BleDevice>)> processor)
{
    emulator.subscribeDeviceFound(processor);
    subscribeDeviceFoundProcessor = processor;
}

void DeviceScannerZ::releaseDevice(std::string name, std::string address){
    emulator.releaseDevice(name, address);
    auto devItm = findeByAddressInV(devices, address);
    if(devItm != devices.end()){
        devices.erase(devItm);
    }
}

bool DeviceScannerZ::isScanning()
{
    return scanning.load() || emulator.isScanning();
}

hci_filter DeviceScannerZ::initScan()
{
    if(!isBleLocalOpen())
    {
        bleLocalOpen();
    }
    if(hci_le_set_scan_parameters(devDescriptor,
                                  0x01, /*scan_type : Active*/
                                  htobs(0x0010), /* interval */
                                  htobs(0x0010), /* window */
                                  LE_PUBLIC_ADDRESS, /* own_type */
                                  0x00, /* filter_policy */
                                  1000) < 0)
    {
        throw BleZErrorScan(strerror(errno)); // Could not set param
    }

    struct hci_filter nf, of;
    socklen_t olen;
    if(hci_le_set_scan_enable(devDescriptor, 0x01, 0x01, 1000) < 0)
    {
        throw BleZErrorScan(strerror(errno)); // Could not enable scan
    }

    if (getsockopt(devDescriptor, SOL_HCI, HCI_FILTER, &of, &olen) < 0) {
        throw BleZErrorScan(strerror(errno)); // Could not get socket options
    }

    hci_filter_clear(&nf);
    hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
    hci_filter_set_event(EVT_LE_META_EVENT, &nf);

    if (setsockopt(devDescriptor, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0) {
        throw BleZErrorScan(strerror(errno)); // Could not set socket options
    }
    return of;
}

void DeviceScannerZ::deinitScan(struct hci_filter &of)
{
    if(hci_le_set_scan_enable(devDescriptor, 0x00, 0x01, 1000) < 0)
    {
//        TODO: Пока игнорируем
//        std::string err = strerror(errno);
//        throw BleZErrorScan(err); // Could not stop scan
    }
    setsockopt(devDescriptor, SOL_HCI, HCI_FILTER, &of, sizeof(of));
}

void DeviceScannerZ::bleLocalOpen()
{
    bleLocalClose();
    devId = hci_get_route(nullptr);
    devDescriptor = hci_open_dev(devId);
    if(devDescriptor < 0)
    {
        throw BleZErrorOpen(strerror(errno)); // Could not open local ble device
    }
    // Set fd non-blocking
    int on = 1;
    if(ioctl(devDescriptor, FIONBIO, (char *)&on) < 0)
    {
        throw BleZErrorNonBlocking(strerror(errno)); // Could not set fd non-blocking
    }
}

void DeviceScannerZ::bleLocalClose()
{
    if(!isBleLocalOpen()) {
        return;
    }
    hci_close_dev(devDescriptor);
    devId = -1;
    devDescriptor = -1;
}

bool DeviceScannerZ::isBleLocalOpen()
{
    return devDescriptor >= 0;
}

bool DeviceScannerZ::scanProcess()
{
    unsigned char buf[HCI_MAX_EVENT_SIZE];

    struct sigaction sa;
    int len;
    signalReceived = 0;
    string devName = string();
    string devAddr = string();

    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_NOCLDSTOP;
    sa.sa_handler = [](int sig) {
        signalReceived = sig;
    };
    sigaction(SIGINT, &sa, nullptr);

    while (scanning.load())
    {
        if(!scanProcessDataRead(len, buf, sizeof(buf))
                || !scanProcessDataParse(len, buf, devName, devAddr))
            return false;

        if(devName.empty() || devAddr.empty()) // skip empty
            continue;
        if(filter.size() > 0 &&
                std::find(filter.begin(), filter.end(), devName) == filter.end()) { // filtered if need
            continue;
        }
        if(pushDevIfNotPresent(devName, devAddr) && subscribeDeviceFoundProcessor != nullptr) {
            subscribeDeviceFoundProcessor(std::unique_ptr<BleDevice>(new BleDeviceZ(devices.back().first, devices.back().second))); // invoke event found
        }
    }
    return true;
}

std::unique_ptr<BleDevice> DeviceScannerZ::findDeviceByAddress(const std::string &addr)
{
    if(addr.empty())
        return nullptr;

    unsigned char buf[HCI_MAX_EVENT_SIZE];
    struct sigaction sa;
    int len;
    signalReceived = 0;
    string devName = string();
    string devAddr = string();

    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_NOCLDSTOP;
    sa.sa_handler = [](int sig) {
        signalReceived = sig;
    };
    sigaction(SIGINT, &sa, nullptr);

    while (scanning.load())
    {
        if(!scanProcessDataRead(len, buf, sizeof(buf))
                || !scanProcessDataParse(len, buf, devName, devAddr))
            break;
        if(devName.empty() || devAddr.empty()) // skip empty
            continue;
        if(pushDevIfNotPresent(devName, devAddr) && (devAddr.compare(addr) == 0))
            return std::unique_ptr<BleDevice>(new BleDeviceZ(devices.back().first, devices.back().second));
    }
    return nullptr;
}

bool DeviceScannerZ::scanProcessDataRead(int &len, unsigned char * buf, size_t bufSize)
{
    // Чтение данных из потока ввода.
    while ((len = read(devDescriptor, buf, bufSize)) < 0)
    {
        if (errno == EINTR && signalReceived == SIGINT)
        {
            len = 0;
            return false;
        }

        if (errno == EAGAIN || errno == EINTR)
            continue;
        return false;
    }
    return true;
}

bool DeviceScannerZ::scanProcessDataParse(int &len, unsigned char * buf, string &retName, string &retAddr)
{
    retName.clear();
    retAddr.clear();

    unsigned char *ptr;
    ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
    len -= (1 + HCI_EVENT_HDR_SIZE);
//    evt_le_meta_event *meta = reinterpret_cast<evt_le_meta_event*>(ptr);
    evt_le_meta_event *meta = (evt_le_meta_event *)(void *) ptr;
    if (meta->subevent != 0x02)
        return false;
    /* Ignoring multiple reports */
//    le_advertising_info *info = reinterpret_cast<le_advertising_info*>((meta->data + 1));
    le_advertising_info *info = (le_advertising_info *) (meta->data + 1);
    if (check_report_filter(0x0, info))
    {
        char addr[18];
        ba2str(&info->bdaddr, addr);

        char name[64];
        std::fill_n(name, sizeof(name), 0);
        if(!eirFillName(info->data, info->length, name, sizeof(name) - 1)) // skip device without name
            return true;
        retName.append(name);
        retAddr.append(addr);
    }
    return true;
}

bool DeviceScannerZ::pushDevIfNotPresent(const std::string &devName, const std::string &devAddr)
{
    auto devItm = findeByAddressInV(devices, devAddr);
    if(devItm != devices.end())
        return false;
    devices.push_back(std::make_pair(devName, devAddr));
    return true;
}

static int check_report_filter(uint8_t procedure, le_advertising_info *info)
{
    uint8_t flags;

    /* If no discovery procedure is set, all reports are treat as valid */
    if (procedure == 0)
        return 1;

    /* Read flags AD type value from the advertising report if it exists */
    if (read_flags(&flags, info->data, info->length))
        return 0;

    switch (procedure) {
    case 'l': /* Limited Discovery Procedure */
        if (flags & FLAGS_LIMITED_MODE_BIT)
            return 1;
        break;
    case 'g': /* General Discovery Procedure */
        if (flags & (FLAGS_LIMITED_MODE_BIT | FLAGS_GENERAL_MODE_BIT))
            return 1;
        break;
    default:
        fprintf(stderr, "Unknown discovery procedure\n");
    }

    return 0;
}

static bool eirFillName(uint8_t *eir, size_t eir_len,
                        char *buf, size_t buf_len)
{
    size_t offset;

    offset = 0;
    while (offset < eir_len) {
        uint8_t field_len = eir[0];
        size_t name_len;

        /* Check for the end of EIR */
        if (field_len == 0)
            break;

        if (offset + field_len > eir_len)
            return false;

        switch (eir[1]) {
        case EIR_NAME_SHORT:
        case EIR_NAME_COMPLETE:
            name_len = field_len - 1;
            if (name_len > buf_len)
                return false;

            memcpy(buf, &eir[2], name_len);
            return true;
        }

        offset += field_len + 1;
        eir += field_len + 1;
    }
    return false;
}

static int read_flags(uint8_t *flags, const uint8_t *data, size_t size)
{
    size_t offset;

    if (!flags || !data)
        return -EINVAL;

    offset = 0;
    while (offset < size) {
        uint8_t len = data[offset];
        uint8_t type;

        /* Check if it is the end of the significant part */
        if (len == 0)
            break;

        if (len + offset > size)
            break;

        type = data[offset + 1];

        if (type == FLAGS_AD_TYPE) {
            *flags = data[offset + 2];
            return 0;
        }

        offset += 1 + len;
    }

    return -ENOENT;
}
