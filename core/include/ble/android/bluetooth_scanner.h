/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neuromd.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_BLUETOOTH_SCANNER_H
#define ANDROID_BLUETOOTH_SCANNER_H

#include <atomic>
#include "ble/ble_scanner.h"
#include "java_environment.h"

namespace Neuro {

    class BluetoothScannerJni : public BleScanner {
    public:
        BluetoothScannerJni(jobject);

        ~BluetoothScannerJni();

        void startScan() override;

        void stopScan() override;

        std::unique_ptr<BleDevice> getDeviceByAddress(std::string) override;

        void setFilter(std::vector<std::string>) override;

        void subscribeDeviceFound(std::function<void(std::unique_ptr<BleDevice>)>) override;

        void releaseDevice(std::string name, std::string address) override;

        void onDeviceFound(jobject);

        bool isScanning() override {
            return isScanProcessing.load();// || emulator.isScanning();
        }

    private:
        static constexpr const char *class_name = "BluetoothScannerJni";

        jobject appContext;
        jobject javaScannerInstance;

        std::atomic<bool> isScanProcessing;
        std::function<void(std::unique_ptr<BleDevice>)> deviceFoundCallback;
    };

}
#endif //ANDROID_BLUETOOTH_SCANNER_H
