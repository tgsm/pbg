#include "entities/CEntityBhvTagCameraSystem.h"
#include "entities/CEntityBhvTagFinishMission.h"
#include "entities/CEntityBhvTagGuiCtrl.h"
#include "entities/CEntityBhvTagNarrator.h"
#include "entities/CEntityBhvTagResponse.h"
#include "entities/CEntityBhvTagSendMessage.h"
#include "entities/CEntityBhvTagSettings.h"
#include "entities/CEntityBhvTagTeleport.h"
#include <cstring>
#include <iostream>

CEntityBhvTagResponse::CEntityBhvTagResponse() {

}

CEntityBhvTagResponse::~CEntityBhvTagResponse() {

}

void CEntityBhvTagResponse::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "SendMessage") == 0) {
                CEntityBhvTagSendMessage* send_message = new CEntityBhvTagSendMessage;
                send_message->Parse(dest);
                AddData(send_message);
            } else if (strcmp(buf, "Teleport") == 0) {
                CEntityBhvTagTeleport* teleport = new CEntityBhvTagTeleport;
                teleport->Parse(dest);
                AddData(teleport);
            } else if (strcmp(buf, "Camera") == 0) {
                CEntityBhvTagCameraSystem* camera_system = new CEntityBhvTagCameraSystem;
                camera_system->Parse(dest);
                AddData(camera_system);
            } else if (strcmp(buf, "GuiControl") == 0) {
                CEntityBhvTagGuiCtrl* gui_control = new CEntityBhvTagGuiCtrl;
                gui_control->Parse(dest);
                AddData(gui_control);
            } else if (strcmp(buf, "Settings") == 0) {
                CEntityBhvTagSettings* settings = new CEntityBhvTagSettings;
                settings->Parse(dest);
                AddData(settings);
            } else if (strcmp(buf, "FinishMission") == 0) {
                CEntityBhvTagFinishMission* finish_mission = new CEntityBhvTagFinishMission;
                finish_mission->Parse(dest);
                AddData(finish_mission);
            } else if (strcmp(buf, "Narrator") == 0) {
                CEntityBhvTagNarrator* narrator = new CEntityBhvTagNarrator;
                narrator->Parse(dest);
                AddData(narrator);
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
