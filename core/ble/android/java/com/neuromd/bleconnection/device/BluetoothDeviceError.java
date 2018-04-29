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

package com.neuromd.bleconnection.device;

public enum BluetoothDeviceError {
    UNKNOWN(0),
    TX_NOT_FOUND(1),
    RX_NOT_FOUND(2),
    STATUS_NOT_FOUND(3),
    SERVICE_NOT_FOUND(4),
    GENERAL_CONNECTION_ERROR(5);

    private final int mErrorCode;

    BluetoothDeviceError(int errorCode){
        mErrorCode = errorCode;
    }

    public int getIntCode(){
        return mErrorCode;
    }
}
