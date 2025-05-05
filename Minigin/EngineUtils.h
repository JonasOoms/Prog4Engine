#pragma once

#include "EngineStructs.h"

namespace Engine
{
    constexpr bool CheckAABBCollision(const Engine::Rect& a, const Engine::Rect& b) {
        return (a.x < b.x + b.width &&
            a.x + a.width > b.width &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
    }
}