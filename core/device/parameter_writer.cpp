#include "device/parameter_writer.h"

namespace Neuro {

bool ParameterWriter::setName(typename ParamValue<Parameter::Name>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

bool ParameterWriter::setState(typename ParamValue<Parameter::State>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

bool ParameterWriter::setAddress(typename ParamValue<Parameter::Address>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

}
