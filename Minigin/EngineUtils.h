#pragma once

#include "EngineStructs.h"


namespace Engine
{
  
    static constexpr glm::vec2 VectorEast = glm::vec2(1.f, 0.f);
    static constexpr glm::vec2 VectorWest = glm::vec2(-1.f, 0.f);
    static constexpr glm::vec2 VectorNorth = glm::vec2(0.f, 1.f);
    static constexpr glm::vec2 VectorSouth = glm::vec2(0.f, -1.f);

    static constexpr glm::vec2 cardinals[] = {
            VectorEast,
            VectorWest,
            VectorNorth,
            VectorSouth
    };

    inline bool GetIsDirectionCardinal(const glm::vec2& direction, float angleThresholdDegrees = 5.f)
    {
        glm::vec2 normalizedDir = glm::normalize(direction);
        float thresholdCos = glm::cos(glm::radians(angleThresholdDegrees));

        for (const auto& cardinal : cardinals)
        {
            float dot = glm::dot(normalizedDir, cardinal);
            if (dot >= thresholdCos)
                return true;
        }

        return false;
    }

    inline glm::vec2 GetClosestCardinal(const glm::vec2& direction)
    {
        glm::vec2 normalizedDir = glm::normalize(direction);
        float maxDot = -1.f;
        glm::vec2 closestCardinal = cardinals[0];

        for (const auto& cardinal : cardinals)
        {
            float dot = glm::dot(normalizedDir, cardinal);
            if (dot > maxDot)
            {
                maxDot = dot;
                closestCardinal = cardinal;
            }
        }

        return closestCardinal;
    }

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

    
    // Credit to Dylan Falconer: https://www.youtube.com/watch?v=3dIiTo7mlnU
    //useful for static vs dynamic objects. 
    // Changing the problem from does it collide to will it collide
    inline SweepResult SweptAABB(const Engine::Rect& moving, const glm::vec2& velocity, const Engine::Rect& target)
    {
        // Expand the static box so that the problem becomes ray vs AABB
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