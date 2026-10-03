#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

struct ReactorPort
{
    glm::vec3 localDirection{ 0.0f, 1.0f, 0.0f };
    float diameter = 1.0f;
    float angle = 0.0f;
    float axialLength = 0.35f;
    bool inlet = true;
};

struct ReactorCollision
{
    bool collided = false;
    bool throughOpening = false;
    glm::vec3 normal{ 0.0f };
    float penetration = 0.0f;
};

class SphericalReactor
{
public:
    explicit SphericalReactor(float radius = 5.0f);

    glm::vec3 position{ 0.0f, 5.0f, 0.0f };
    glm::quat orientation{ 1.0f, 0.0f, 0.0f, 0.0f };
    float radius = 5.0f;
    bool showInside = false;
    std::string name = "Spherical reactor";
    std::vector<ReactorPort> inlets;
    std::vector<ReactorPort> outlets;

    glm::vec3 WorldDirection(const ReactorPort& port) const;
    glm::vec3 WorldPortCenter(const ReactorPort& port) const;
    ReactorCollision CollideInside(const glm::vec3& particlePosition,
        float particleRadius) const;
    glm::vec3 SpawnPosition(const ReactorPort& port, float particleRadius) const;
    glm::vec3 SpawnVelocity(const ReactorPort& port, float speed) const;
    bool IsInside(const glm::vec3& p, float margin = 0.0f) const;
    std::string Specifications() const;

private:
    bool IsOpening(const glm::vec3& localPoint, float particleRadius) const;
    static glm::vec3 SafeNormalize(const glm::vec3& v);
};