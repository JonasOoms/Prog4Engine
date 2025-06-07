#pragma once

#include "EngineStructs.h"

namespace Engine
{
  
    constexpr bool CheckAABBCollision(const Engine::Rect& a, const Engine::Rect& b) {
        return (a.x < b.x + b.width &&
            a.x + a.width > b.x && 
            a.y < b.y + b.height &&
            a.y + a.height > b.y);  
    }

    inline bool RayAABBIntersect(
        const glm::vec2& rayStart,
        const glm::vec2& rayDir,
        const Engine::Rect& rect,
        float& tNear,
        float& tFar)
    {
        glm::vec2 rectMin = { rect.x, rect.y };
        glm::vec2 rectMax = { rect.x + rect.width, rect.y + rect.height };
        glm::vec2 invDir = 1.0f / rayDir;

        glm::vec2 t1 = (rectMin - rayStart) * invDir;
        glm::vec2 t2 = (rectMax - rayStart) * invDir;

        glm::vec2 tMin = glm::min(t1, t2);
        glm::vec2 tMax = glm::max(t1, t2);

        tNear = std::max(tMin.x, tMin.y);
        tFar = std::min(tMax.x, tMax.y);

        return (tNear <= tFar) && (tFar >= 0);
    }

    inline SweepResult SweptAABB(const Engine::Rect& moving, const glm::vec2& velocity, const Engine::Rect& target)
    {
        Engine::Rect expandedTarget;
        expandedTarget.x = target.x - moving.width;
        expandedTarget.y = target.y - moving.height;
        expandedTarget.width = target.width + moving.width;
        expandedTarget.height = target.height + moving.height;

        glm::vec2 invDir = {
            velocity.x != 0 ? 1.0f / velocity.x : std::numeric_limits<float>::infinity(),
            velocity.y != 0 ? 1.0f / velocity.y : std::numeric_limits<float>::infinity()
        };

        glm::vec2 from = { moving.x, moving.y };
        glm::vec2 to = from + velocity;

        glm::vec2 t1 = (glm::vec2(expandedTarget.x, expandedTarget.y) - from) * invDir;
        glm::vec2 t2 = (glm::vec2(expandedTarget.x + expandedTarget.width, expandedTarget.y + expandedTarget.height) - from) * invDir;

        glm::vec2 tmin = glm::min(t1, t2);
        glm::vec2 tmax = glm::max(t1, t2);

        float tEnter = std::max(tmin.x, tmin.y);
        float tExit = std::min(tmax.x, tmax.y);

        SweepResult result;
        if (tEnter < tExit && tEnter >= 0.0f && tEnter <= 1.0f)
        {
            result.hit = true;
            result.time = tEnter;
            if (tmin.x > tmin.y)
                result.normal = { velocity.x < 0 ? 1.0f : -1.0f, 0.0f };
            else
                result.normal = { 0.0f, velocity.y < 0 ? 1.0f : -1.0f };
        }

        return result;
    }
}