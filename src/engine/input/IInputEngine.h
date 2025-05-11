#pragma once on

#include "engine/input/IInput.h"

namespace DKI {

// TODO
class IInputEngine {
public:
    static IDevice* GetDevice(int);
};

}
