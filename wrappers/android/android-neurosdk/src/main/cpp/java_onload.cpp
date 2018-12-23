#include <wrappers/channels/jni_resistance_channel_wrap.h>
#include <wrappers/jni_device_scanner_wrap.h>
#include "wrappers/channels/jni_electrodes_state_channel_wrap.h"
#include "wrappers/channels/jni_spectrum_channel_wrap.h"
#include "wrappers/channels/jni_connection_stats_channel_wrap.h"
#include "wrappers/channels/jni_angle_channel_wrap.h"
#include "wrappers/channels/jni_respiration_channel_wrap.h"
#include "wrappers/channels/jni_mems_channel_wrap.h"
#include "wrappers/device/jni_param_types_wrap.h"
#include "wrappers/channels/jni_battery_channel_wrap.h"
#include "wrappers/channels/jni_signal_channel_wrap.h"
#include "wrappers/channels/jni_channel_info_wrap.h"
#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/device/jni_device_param_wrap.h"
#include "jni_logger.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jni::on_vm_load(vm);
    JNIEnv *env;
    if (jni::get_env(&env)!=0)
        std::terminate();

    jni::initJavaObjClass<int>(env);
    jni::initJavaObjClass<long>(env);
    jni::initJavaObjClass<double>(env);
    jni::initJavaObjClass<bool>(env);
    jni::initJavaObjClass<unsigned char>(env);
    jni::initJavaObjClass<std::string>(env);
    jni::initJavaObjClass<JniDeviceScannerWrap *>(env);
    jni::initJavaObjClass<JniDeviceWrap *>(env);
    jni::initJavaObjClass<JniBatteryChannelWrap *>(env);
    jni::initJavaObjClass<JniSignalChannelWrap *>(env);
    jni::initJavaObjClass<JniRespirationChannelWrap *>(env);
    jni::initJavaObjClass<JniMEMSChannelWrap *>(env);
    jni::initJavaObjClass<JniOrientationChannelWrap *>(env);
    jni::initJavaObjClass<JniConnectionStatsChannelWrap *>(env);
    jni::initJavaObjClass<JniElectrodesStateChannelWrap *>(env);
    jni::initJavaObjClass<JniSpectrumChannelWrap *>(env);
    jni::initJavaObjClass<JniResistanceChannelWrap *>(env);
    jni::initJavaObjClass<Neuro::ChannelInfo *>(env);
    jni::initJavaObjClass<Neuro::ChannelInfo>(env);
    jni::initJavaObjClass<Neuro::ChannelInfo::Type>(env);
    jni::initJavaObjClass<Neuro::DeviceState>(env);
    jni::initJavaObjClass<Neuro::Command>(env);
    jni::initJavaObjClass<Neuro::Parameter>(env);
    jni::initJavaObjClass<Neuro::ParamAccess>(env);
    jni::initJavaObjClass<Neuro::ParamPair>(env);
    jni::initJavaObjClass<ParameterType>(env);
    jni::initJavaObjClass<Neuro::ADCInput>(env);
    jni::initJavaObjClass<Neuro::AccelerometerSensitivity >(env);
    jni::initJavaObjClass<Neuro::GyroscopeSensitivity>(env);
    jni::initJavaObjClass<Neuro::ExternalSwitchInput>(env);
    jni::initJavaObjClass<Neuro::FirmwareMode>(env);
    jni::initJavaObjClass<Neuro::Gain>(env);
    jni::initJavaObjClass<Neuro::MotionAssistantLimb>(env);
    jni::initJavaObjClass<Neuro::SamplingFrequency>(env);
    jni::initJavaObjClass<Neuro::MEMS>(env);
    jni::initJavaObjClass<Neuro::Quaternion>(env);
    jni::initJavaObjClass<Neuro::ElectrodeState>(env);
    jni::initJavaObjClass<Neuro::MotionAssistantParams>(env);
    jni::initJavaObjClass<Neuro::MotionAssistantLimb >(env);
    jni::initJavaObjClass<Neuro::StimulationParams>(env);
    jni::initJavaObjClass<Neuro::StimulatorImpulseDuration>(env);
    jni::initJavaObjClass<Neuro::StimulatorDeviceState>(env);
    jni::initJavaObjClass<Neuro::StimulatorDeviceState::State>(env);
    jni::initJavaObjClass<Neuro::FirmwareVersion>(env);

    auto logFactory = LoggerFactory::getInstance();
    logFactory->setLogger(new JniLogger());
#ifdef NDEBUG
    logFactory->setLogLevel(LogLevel::Info);
#else
    logFactory->setLogLevel(LogLevel::Trace);
#endif
    return JNI_VERSION_1_6;
}