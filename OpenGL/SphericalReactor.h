#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>

#include "ParticleSandbox.h"

class SphericalReactor
{
public:
    void AddReactor(const std::shared_ptr<SphericalReactor>& reactor);
    void ClearReactors();
    void SetRunning(bool running) { m_Running = running; }
    bool IsRunning() const { return m_Running; }
    void AddParticle(const glm::vec3& position, const glm::vec3& velocity);
    void EmitFromInlets(float dt, float particlesPerSecond, float speed);

private:
    void ResolveReactorCollisions(FluidParticle& particle);
    std::vector<std::shared_ptr<SphericalReactor>> m_Reactors;
    bool m_Running = false;
    float m_EmissionCarry = 0.0f;
};