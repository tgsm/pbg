#include "input/CInput.h"
#include <cstring>

namespace DKI {

CInput::CInput(char* name, IDevice* device, int a3) : IInput(name) {
    m_device = device;
    m_unkC = a3;
}

}
