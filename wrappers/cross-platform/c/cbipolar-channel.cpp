extern "C"
{
#include "cbipolar-channel.h"
}

#include "cchannel-helper.h"
#include "channels/bipolar_channel.h"

using DoubleChannelWrap = DataChannelWrapper<double>;
using DoubleChannelWrapPtr = std::shared_ptr<DoubleChannelWrap>;

using BipolarChannelType = Neuro::BipolarChannel<DoubleChannelWrap>;
using BipolarChannelWrap = SpecificChannelWrapper<BipolarChannelType>;
using BipolarWrapPtr = std::shared_ptr<BipolarChannelWrap>;

BipolarDoubleChannel* create_BipolarDoubleChannel(BridgeDoubleChannel *first, BridgeDoubleChannel *second) {
	auto& firstShared = *reinterpret_cast<DoubleChannelWrapPtr *>(first);
	auto& secondShared = *reinterpret_cast<DoubleChannelWrapPtr *>(second);
	try {
		auto bipolarChannel = Neuro::make_bipolar_from_ptrs(firstShared, secondShared);
		const auto bipolarChannelPtr = std::make_shared<BipolarChannelType>(std::move(bipolarChannel));
		return getCObjectPtr<BipolarDoubleChannel>(bipolarChannelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}