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

package ru.neurotech.neurosdk;

import ru.neurotech.neurosdk.channels.ChannelInfo;
import ru.neurotech.neurosdk.parameters.Command;
import ru.neurotech.neurosdk.parameters.Parameter;
import ru.neurotech.neurosdk.parameters.ParameterName;

public class Device {
    static {
        System.loadLibrary("neurosdk");
    }

    private long mNativeObjPtr = 0;

    private Device(long nativeObjPtr) {
        mNativeObjPtr = nativeObjPtr;
    }

    public void finalize() throws Throwable {
        if (mNativeObjPtr != 0) {
            deleteDevice(mNativeObjPtr);
            mNativeObjPtr = 0;
        }
        super.finalize();
    }

    public native void connect();

    public native void disconnect();

    public native ChannelInfo[] channels();

    public native Command[] commands();

    public native Parameter[] parameters();

    public native boolean execute(Command cmd);

    public native Object readParam(ParameterName param);

    public native boolean setParam(ParameterName param, Object value);

    private native void deleteDevice(long objPtr);
}
