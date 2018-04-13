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

package ru.neurotech.bleconnection.device;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

/**
 * Provides filtering list and check methods for ble devices
 */
public class DeviceFilter {
    private List<String> mFilterList = new ArrayList<>();

    /**
     * Adds device name to filtering list
     *
     * @param name Device name
     */
    public void addFilter(String name) {
        Log.v("Device filter", String.format("Add filter \"%s\"", name));
        mFilterList.add(name);
    }

    /**
     * Clears filter list
     */
    public void resetFilter() {
        mFilterList.clear();
    }

    /**
     * Checks if device with @name passes filters
     *
     * @param name Device name
     * @return True if device presents in filter list
     */
    public boolean filter(String name) {
        if (mFilterList.isEmpty()) return true;
        return mFilterList.contains(name);
    }
}
