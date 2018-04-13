#include <wrappers/jni_motion_assistant_device_wrap.h>

template<>
template<>
jni::java_object<MotionAssistantLimb>::java_object(const MotionAssistantLimb &limb):nativeObj(limb) {
    JNIEnv *env;
    auto resCode = get_env(&env);
    if (resCode == 2) return;

    auto fromIntCodeMethod = env->GetStaticMethodID(object_class, "fromIntCode",
                                                    jni::constructor_signature<MotionAssistantLimb>());
    javaObj = make_global_ref_ptr(env->CallStaticObjectMethod(object_class, fromIntCodeMethod,
                                                              static_cast<int>(nativeObj)));

    if (resCode == 1) detach_thread();
}

template<>
template<>
jni::java_object<MotionAssistantParams>::java_object(const MotionAssistantParams &maParams):nativeObj(maParams){
    JNIEnv *env;
    auto resCode = get_env(&env);
    if (resCode == 2) return;

    auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                   constructor_signature<MotionAssistantParams>());
    javaObj = make_global_ref_ptr(env->NewObject(object_class, objectClassConstructor));
    auto gyroStartFieldID = env->GetFieldID(object_class, "gyroStart", "I");
    auto gyroStopFieldID = env->GetFieldID(object_class, "gyroStop", "I");
    auto limbFieldID = env->GetFieldID(object_class, "limb", "Lru/neurotech/neurodevices/stimulation/MotionAssistantLimb;");
    auto minPauseFieldID = env->GetFieldID(object_class, "minPause", "I");
    auto maxDurationFieldID = env->GetFieldID(object_class, "maxDuration", "I");

    env->SetIntField(javaObj.get(), gyroStartFieldID, nativeObj.gyroStart);
    env->SetIntField(javaObj.get(), gyroStopFieldID, nativeObj.gyroStop);
    env->SetObjectField(javaObj.get(), limbFieldID, java_object<MotionAssistantLimb>(nativeObj.limb));
    env->SetIntField(javaObj.get(), minPauseFieldID, nativeObj.minPause);
    env->SetIntField(javaObj.get(), maxDurationFieldID, nativeObj.maxDuration);

    if (resCode == 1) detach_thread();
}

template<>
template<>
jni::java_object<StimulationParams>::java_object(const StimulationParams &sParams):nativeObj(sParams){
    JNIEnv *env;
    auto resCode = get_env(&env);
    if (resCode == 2) return;

    auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                   constructor_signature<StimulationParams>());
    javaObj = make_global_ref_ptr(env->NewObject(object_class, objectClassConstructor));
    auto amplitudeFieldID = env->GetFieldID(object_class, "amplitude", "I");
    auto pulseDurationFieldID = env->GetFieldID(object_class, "pulse_duration", "I");
    auto frequencyFieldID = env->GetFieldID(object_class, "frequency", "I");
    auto stimulDurationFieldID = env->GetFieldID(object_class, "stimul_duration", "I");

    env->SetIntField(javaObj.get(), amplitudeFieldID, nativeObj.current);
    env->SetIntField(javaObj.get(), pulseDurationFieldID, nativeObj.pulse_duration);
    env->SetIntField(javaObj.get(), frequencyFieldID, nativeObj.frequency);
    env->SetIntField(javaObj.get(), stimulDurationFieldID, nativeObj.stimulus_duration);

    if (resCode == 1) detach_thread();
}

void JniMotionAssistantDevice::subscribeMotionAssistantParamsChanged(jobject instance) {

    motionAssistantParamsChangedSubscriberRef = jni::make_global_ref_ptr(instance);
    std::weak_ptr<jni::jobject_t> weakReference = motionAssistantParamsChangedSubscriberRef;

    auto onMotionAssistantParamsChangedFunc = [weakReference](const MotionAssistantDevice &,
                                                              int gyroStart, int gyroStop,
                                                              MotionAssistantLimb limb,
                                                              int minStimulationPause,
                                                              int maxStimulusDuration) {
        sendNotification(weakReference,
                         MotionAssistantParams{gyroStart, gyroStop, limb, minStimulationPause,
                                               maxStimulusDuration});
    };
    motionAssistantParamsChangedHandler = MakeHandler(MotionAssistantDevice,
                                                      motionAssistantParamsChanged,
                                                      onMotionAssistantParamsChangedFunc);
    object->motionAssistantParamsChanged += motionAssistantParamsChangedHandler;
}

void JniMotionAssistantDevice::subscribeMotionAssistantStateChanged(jobject instance){

    motionAssistantStateChangedSubscriberRef = jni::make_global_ref_ptr(instance);
    std::weak_ptr<jni::jobject_t> weakReference = motionAssistantStateChangedSubscriberRef;

    using namespace std::placeholders;
    auto onMotionAssistantStateChangedFunc = std::bind(sendNotification<bool>, weakReference, _2);
    motionAssistantStateChangedHandler = MakeHandler(MotionAssistantDevice, motionAssistantStateChanged, onMotionAssistantStateChangedFunc);
    object->motionAssistantStateChanged+=motionAssistantStateChangedHandler;
}

void JniMotionAssistantDevice::subscribeStimulationParamsChanged(jobject instance) {
    stimulationParamsChangedSubscriberRef = jni::make_global_ref_ptr(instance);
    std::weak_ptr<jni::jobject_t> weakReference = stimulationParamsChangedSubscriberRef;

    using namespace std::placeholders;
    auto onStimulationParamsChangedFunc = [weakReference](const MotionAssistantDevice &,
                                                          int amplitude, int pulse_duration,
                                                          int frequency, int stimul_duration) {
        sendNotification(weakReference,
                         StimulationParams{amplitude, pulse_duration, frequency, stimul_duration});
    };
    stimulationParamsChangedHandler = MakeHandler(MotionAssistantDevice, stimulationParamsChanged,
                                                  onStimulationParamsChangedFunc);
    object->stimulationParamsChanged += stimulationParamsChangedHandler;
}

void JniMotionAssistantDevice::subscribeStimulatorStateChanged(jobject instance){

    stimulatorStateChangedSubscriberRef = jni::make_global_ref_ptr(instance);
    std::weak_ptr<jni::jobject_t> weakReference = stimulatorStateChangedSubscriberRef;

    using namespace std::placeholders;
    auto onStimulatorStateChangedFunc = std::bind(sendNotification<bool>, weakReference, _2);
    stimulatorStateChangedHandler = MakeHandler(MotionAssistantDevice, stimulatorStateChanged, onStimulatorStateChangedFunc);
    object->stimulatorStateChanged+=stimulatorStateChangedHandler;
}