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

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, NCDeviceState){Unknown = 0, Error = 1, Disconnected = 2, Connected = 3, Ready = 4, Working = 5, Closed = 6};

typedef NS_ENUM(NSUInteger, NCDeviceFeature) {Signal = 0, Mems = 1, Spyro = 2, Stimul = 3};
typedef NS_ENUM(NSUInteger, NCMotionAssistantLimb) {RightLeg = 0, LeftLeg = 1, RightArm = 2, LeftArm = 3};
typedef NS_ENUM(NSUInteger, NCStimulatorImpulseDuration) {us60 = 6, us100 = 10, us200 = 20};


/**
 * Represents stimulation module of NeuroDevice
 * Provides common methods for miostimulation and motion assistance
 */
@interface NCStimulationSubsystem : NSObject

-(id)initWithNativeSubsystem: (void*)subsystem;

-(void)stimulStart;
-(void)stimulStop;
-(bool)getStimulatorIsActive;
-(void)setStimulParams:(int) current pulse_duration:(int)pulse_duration frequency:(int)frequency stimulus_duration:(int)stimulus_duration;
-(int)getCurrentAmplitude;
-(int)getPulseDuration;
-(int)getPulseFrequency;
-(int)getStimulationDuration;
-(void)shStart;
-(void)shStop;
-(bool)getShIsActive;
-(void)setShParams:(int) gyroStart gyroStop:(int)gyroStop limb:(NCMotionAssistantLimb)limb stimulationPause:(int)stimulationPause maxStimulationDuration:(int)maxStimulationDuration;
-(int)getGyroStart;
-(int)getGyroStop;
-(NCMotionAssistantLimb)getSelectedLimb;
-(int)getStimulationPause;
-(int)getMaxStimulationDuration;
-(void)doCalibration;

@end


/**
 * Represents all Neurotec devices
 * Provides methods for managing Bluetooth connection, list availible features of device
 * and access device modules
 */
@interface NCNeuroDevice : NSObject

-(id)initWithNeuroDevice: (void*)device;
-(void)connect;
-(void)disconnect;
-(void)close;
-(void)subscribeStateChanged:(void(^)(NCDeviceState))onStatusChangedCallback;
-(const NSString*)getName;
-(const NSString*)getAddress;
-(int)getBatteryLevel;
-(NCStimulationSubsystem*)getStimulationSubsystem;
-(NSArray<NSNumber *>*)getFeatures;

@end

/**
 * Provides basic connection methods for all Neurotech devices
 */
@interface NCNeuroConnection : NSObject

-(void)startScan:(int)msTimeout;
-(void)stopScan;
-(void)subscribeDeviceFound:(void (^)(NCNeuroDevice*))deviceFoundCallback;
-(void)subscribeScanStateChanged: (void (^)(bool)) scanStateChangedCallback;
-(NCNeuroDevice*)findDeviceByAddress:(const NSString*)address;

@end


@interface NCMotionAssistantDevice : NSObject

-(id) initWithNativeDevice: (void*) device;

-(void) motionAssistantStart;
-(void) motionAssistantStop;
-(bool) getMotionAssistantState;
-(void) setMotionAssistantParams:(int) gyroStart gyroStop:(int)gyroStop limb:(NCMotionAssistantLimb)limb minStimulationPause:(int)minStimulationPause maxStimulationDuration:(int)maxStimulationDuration;
-(void) setGyroStartThreshold:(int) gyroStart;
-(void) setGyroStopThreshold:(int) gyroStop;
-(void) setLimbForStimulation:(NCMotionAssistantLimb) limb;
-(void) setMinAssistantStimulationPause:(int) stimulationPause;
-(void) setMaxAssistantStimulusDuration:(int) maxStimulDuration;
-(int) getGyroStartThreshold;
-(int) getGyroStopThreshold;
-(NCMotionAssistantLimb) getLimbForStimulation;
-(int) getMinAssistantStimulationPause;
-(int) getMaxAssistantStimulusDuration;
-(void) stimulationStart;
-(void) stimulationStop;
-(bool) getStimulatorState;
-(void) doCalibration;
-(void) setStimulatorParams:(int) amplitude pulse_duration:(NCStimulatorImpulseDuration) pulse_duration frequency:(int) frequency stim_duration:(int) stim_duration;
-(void) setCurrentAmplitude:(int) current;
-(void) setPulseDuration:(NCStimulatorImpulseDuration) pulseDuration;
-(void) setPulseFrequency:(int) pulseFrequency;
-(void) setStimulationDuration:(int) stimulationDuration;
-(int) getCurrentAmplitude;
-(NCStimulatorImpulseDuration) getPulseDuration;
-(int) getPulseFrequency;
-(int) getStimulationDuration;
-(void) setAssistantParamsChangedCallback:(void(^)(int gyroStart, int gyroStop, NCMotionAssistantLimb limb, int minStimulationPause, int maxStimulusDuration)) assistantParamsChangedCallback;
-(void) setStimulationParamsChangedCallback:(void(^)(int amplitude, NCStimulatorImpulseDuration pulse_duration, int frequency, int stimul_duration)) stimulParamsChangedCallback;
-(void) setAssistantStateChangedCallback:(void(^)(bool isOn)) assistantStateChangedCallback;
-(void) setStimulatorStateChangedCallback:(void(^)(bool isOn)) stimulatorStateChangedCallback;
-(NCNeuroDevice *) getNeuroDevice;

@end

@interface NCMotionAssistantConnection : NSObject

-(void)startScan:(int)timeout;
-(void)stopScan;
-(NCMotionAssistantDevice *)getDeviceByAddress:(const NSString*)address;
-(void)setScanStateChangedCallback:(void(^)(bool isScanning)) scanStateChangedCallback;
-(void)setDeviceFoundCallback:(void(^)(NCMotionAssistantDevice *)) deviceFoundCallback;

@end
