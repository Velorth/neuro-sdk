/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
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

package ru.neurotech.bleconnection.scanner;

import android.util.Log;

/**
 * Factory for BleDeviceScanner creation
 */
public class BleDeviceScannerFactory {
    /**
     * Creates new instance of BleDeviceScanner depending on API version
     *
     * @return BleDeviceScanner instance
     */
    public static BleDeviceScanner getBleDeviceScanner() {
        Log.v("BleDeviceScannerFactory", "Instantiating scanner");
        if (android.os.Build.VERSION.SDK_INT >= 21) {
            return new BleDeviceScanner21Api();
        } else {
            return new BleDeviceScannerOldApi();
        }
    }
}
