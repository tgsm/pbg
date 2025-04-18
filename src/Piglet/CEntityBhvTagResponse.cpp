#include "CEntityBhvTagNarrator.h"
#include "CEntityBhvTagResponse.h"
#include "CEntityBhvTagSendMessage.h"
#include <cstring>
#include <iostream>

CEntityBhvTagResponse::CEntityBhvTagResponse() {

}

CEntityBhvTagResponse::~CEntityBhvTagResponse() {

}

void CEntityBhvTagResponse::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) != TRUE) {
        return;
    }

    while (TRUE) {
        strcpy(buf, dest.GetName());

        if (strcmp(buf, "SendMessage") == 0) {
            CEntityBhvTagSendMessage* send_message = new CEntityBhvTagSendMessage;
            send_message->Parse(dest);
            AddData(send_message);
        } else if (strcmp(buf, "Teleport") == 0) {
            // FIXME: CEntityBhvTagTeleport
        } else if (strcmp(buf, "Camera") == 0) {
            // FIXME: CEntityBhvTagCameraSystem
        } else if (strcmp(buf, "GuiControl") == 0) {
            // FIXME: CEntityBhvGuiCtrl
        } else if (strcmp(buf, "Settings") == 0) {
            // FIXME: CEntityBhvTagSettings
        } else if (strcmp(buf, "FinishMission") == 0) {
            // FIXME: CEntityBhvTagFinishMission
        } else if (strcmp(buf, "Narrator") == 0) {
            CEntityBhvTagNarrator* narrator = new CEntityBhvTagNarrator;
            narrator->Parse(dest);
            AddData(narrator);
        }

        if (dest.GetNextSiblingChunk(dest) != TRUE) {
            break;
        }
    }
}
