extern "C"
{
#include "clistener.h"
}

#include "event_listener.h"

void free_listener_handle(ListenerHandle handle) {
	auto handlePtr = reinterpret_cast<Neuro::ListenerPtr<void, size_t> *>(handle);
	delete handlePtr;
}