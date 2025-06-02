#ifndef ENGINE_GUI_CGUIENGINE_H
#define ENGINE_GUI_CGUIENGINE_H

#include "engine/gui/IGUIEngine.h"

namespace DKGUI {

class CGUIEngine : public IGUIEngine {
public:
    virtual ~CGUIEngine();

    virtual void UpdateAndRenderOnlyTexts(RwCamera* camera);
};

}

#endif
