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

package ru.neurotech.common;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Create instance of this class to use notification mechanism for your subscribers
 * Usage:
 * public SubscribersNotifier<myParamType> myEvent = new SubscribersNotifier<myParamType>();
 * ...
 * myEvent.sendNotification(param);
 * ...
 * <p>
 * To subscribe do the following:
 * private INotificationCallback<myParamType> _myScanCallback = new INotificationCallback<myParamType>()
 * {
 *
 * @Override public void onNotify(myParamType nParam)
 * {
 * //Handle notification
 * }
 * };
 * ...
 * _myObjectWithNotifications.myEvent.subscribe(_myScanCallback);
 * ...
 */
public class SubscribersNotifier<T> {
    /**
     * List of subscribers of your class
     * Enumerate it and call notification methods
     */
    private List<INotificationCallback<T>> mSubscribers = Collections.synchronizedList(new ArrayList<INotificationCallback<T>>());

    /**
     * Appends notifications subscriber
     *
     * @param callback subscriber callback class
     */
    public void subscribe(INotificationCallback<T> callback) {
        if (mSubscribers.contains(callback)) return;
        mSubscribers.add(callback);
    }

    /**
     * Removes subscriber from notification list
     *
     * @param callback
     */
    public void unsubscribe(INotificationCallback<T> callback) {
        mSubscribers.remove(callback);
    }

    public void unsubscribe() {
        mSubscribers.clear();
    }

    /**
     * Sends notification to all subscribers
     *
     * @param param notification data
     */
    public void sendNotification(Object sender, T param) {
        for (INotificationCallback<T> subscriber : mSubscribers) {
            if (subscriber == null) continue;
            subscriber.onNotify(sender, param);
        }
    }
}
