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

#include "java_environment.h"
#include "signal/channel.h"

extern "C"
{

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_features_Channel_deleteChannelObj(JNIEnv *env, jobject instance,
                                                                 jlong objPtr) {

    auto channel = (std::weak_ptr<Channel> *) objPtr;
    delete channel;
}


JNIEXPORT jdoubleArray JNICALL
Java_ru_neurotech_neurodevices_features_Channel_getSignalData__JII(JNIEnv *env, jobject instance,
                                                                   jlong objPtr, jint offset,
                                                                   jint length) {
    auto channelPtr = *(std::weak_ptr<Channel> *) objPtr;
    auto channel = channelPtr.lock();
    if (!channel)
        return NULL;
    if (length < 0) return NULL;


    auto data = channel->getSignalData(offset, (size_t) length);
    if (data.size() == 0) return NULL;

    jdoubleArray dataArray = env->NewDoubleArray(data.size());
    if (dataArray == NULL) {
        return NULL;
    }

    env->SetDoubleArrayRegion(dataArray, 0, data.size(), &data[0]);
    return dataArray;
}

JNIEXPORT jdoubleArray JNICALL
Java_ru_neurotech_neurodevices_features_Channel_getRawData__JII(JNIEnv *env, jobject instance,
                                                                jlong objPtr, jint offset,
                                                                jint length) {
;
    auto channelPtr = *(std::weak_ptr<Channel> *) objPtr;
    auto channel = channelPtr.lock();
    if (!channel)
        return NULL;
    if (length < 0) return NULL;



    auto data = channel->getRawData(offset, (size_t) length);
    if (data.size() == 0) return NULL;

    jdoubleArray dataArray = env->NewDoubleArray(data.size());
    if (dataArray == NULL) {
        return NULL;
    }

    env->SetDoubleArrayRegion(dataArray, 0, data.size(), &data[0]);
    return dataArray;
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurodevices_features_Channel_getDataLength(JNIEnv *env, jobject instance,
                                                              jlong objPtr) {
    auto channelPtr = *(std::weak_ptr<Channel> *) objPtr;
    auto channel = channelPtr.lock();
    if (!channel)
        return 0;
    return (long) channel->getDataLength();
}

JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_features_Channel_getName(JNIEnv *env, jobject instance,
                                                        jlong objPtr) {
    auto channelPtr = *(std::weak_ptr<Channel> *) objPtr;
    auto channel = channelPtr.lock();
    if (!channel)
        return NULL;

    const char *cName = channel->getName().c_str();
    return env->NewStringUTF(cName);
}

}
