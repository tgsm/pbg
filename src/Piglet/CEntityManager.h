#pragma once

#include <string>
#include "CGame.h"
#include "types.h"

class CEntityManager {
private:
    CGame* m_game;

public:
    // FIXME: Make an enum for entity types

    CGame* GetGame() { return m_game; }

    u32 GetTypeFromString(std::string type_str);
    u32 CreateEntity(std::string str1, std::string type, std::string str3); // unknown return type
    u32 CreateEntity(std::string str1, u32 type, std::string str3); // unknown return type

    u32 GetEntity(std::string name); // unknown return type

    u32 GetHero(); // unknown return type
};
