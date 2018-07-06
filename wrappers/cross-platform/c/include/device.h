#ifndef DEVICE_H
#define DEVICE_H

typedef struct _Device Device;

typedef enum _Command {
	CommandStartSignal,
	CommandStopSignal,
	CommandStartResist,
	CommandStopResist,
	CommandStartMEMS,
	CommandStopMEMS,
	CommandStartRespiration,
	CommandStopRespiration,
	CommandStartStimulation,
	CommandEnableMotionAssistant,
	CommandFindMe
}
Command;

int device_connect(Device *);
int device_disconnect(Device *);
int device_available_channels(const Device *);
int device_available_commands(const Device *);
int device_available_parameters(const Device *);
int device_execute(Device *, Command);
void* device_subscribe_param_changed(Device *, void(*)(Parameter));
int device_read_param(Device *, Parameter);
int device_set_param(Device *, const ParamValue *);

#endif // DEVICE_H
