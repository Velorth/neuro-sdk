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
#include "subsystem/signal_subsystem.h"

extern "C"
{

JNIEXPORT jintArray JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_getSupportedFrequencies(JNIEnv *env,
                                                                                jobject instance,
                                                                                jlong objPtr) {
    auto signalSubsystem = (SignalSubsystem *) objPtr;
    auto frequencies = signalSubsystem->getSupportedFrequencies();

    jintArray frequenciesArray = env->NewIntArray(frequencies.size());
    if (frequenciesArray == NULL) {
        return NULL;
    }

    env->SetIntArrayRegion(frequenciesArray, 0, frequencies.size(), &frequencies[0]);
    return frequenciesArray;
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_getSamplingFrequency(JNIEnv *env,
                                                                             jobject instance,
                                                                             jlong objPtr) {
    auto signalSubsystem = (SignalSubsystem *) objPtr;
    return signalSubsystem->getSamplingFrequency();
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_setSamplingFrequency(JNIEnv *env,
                                                                             jobject instance,
                                                                             jlong objPtr,
                                                                             jint frequency) {
    auto signalSubsystem = (SignalSubsystem *) objPtr;
    signalSubsystem->setSamplingFrequency(frequency);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_stopReceive(JNIEnv *env, jobject instance,
                                                                    jlong objPtr) {
    auto signalSubsystem = (SignalSubsystem *) objPtr;
    signalSubsystem->stopReceive();
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_startReceive(JNIEnv *env, jobject instance,
                                                                     jlong objPtr) {
    auto signalSubsystem = (SignalSubsystem *) objPtr;
    signalSubsystem->startReceive();
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_getChannels(JNIEnv *env,
                                                                    jobject instance,
                                                                    jlong objPtr) {
    auto signalSubsystem = (SignalSubsystem *) objPtr;
    auto channels = signalSubsystem->getChannels();

    auto channelClass = jni::getChannelClass();
    auto constructor = env->GetMethodID(channelClass, "<init>", "(J)V");

    jobjectArray channelsArray = env->NewObjectArray((jsize)channels.size(), channelClass, NULL);

    for (auto channel = channels.begin(); channel<channels.end(); ++channel) {

        auto channelObj = env->NewObject(channelClass, constructor,
                                         (jlong) new std::weak_ptr<Channel>(*channel));
        env->SetObjectArrayElement(channelsArray, channel - channels.begin(), channelObj);
    }

    return channelsArray;
}


JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_getGain__J(JNIEnv *env, jobject instance,
                                                                   jlong objPtr) {

    auto signalSubsystem = (SignalSubsystem *) objPtr;
    return signalSubsystem->getGain();

}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_setGain__JI(JNIEnv *env, jobject instance,
                                                                    jlong objPtr, jint gain) {

    auto signalSubsystem = (SignalSubsystem *) objPtr;
    signalSubsystem->setGain(gain);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_setSignalOffset__JI(JNIEnv *env,
                                                                            jobject instance,
                                                                            jlong objPtr,
                                                                            jint offset) {

    auto signalSubsystem = (SignalSubsystem *) objPtr;
    signalSubsystem->setOffset(offset);
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_features_SignalSubsystem_getSignalOffset__J(JNIEnv *env,
                                                                           jobject instance,
                                                                           jlong objPtr) {

    auto signalSubsystem = (SignalSubsystem *) objPtr;
    return signalSubsystem->getOffset();
}

}
