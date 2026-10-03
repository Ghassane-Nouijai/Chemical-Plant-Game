#include "Reactor.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

SphericalReactor::SphericalReactor(float r) : radius(std::max(r, 0.1f))
{
    inlets.push_back(ReactorPort{ glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f, 0.35f, true });
    outlets.push_back(ReactorPort{ glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f, 0.35f, false });
}

glm::vec3 SphericalReactor::SafeNormalize(const glm::vec3& v)
{
    const float len2 = glm::dot(v, v);
    return len2 > 1e-8f ? v * (1.0f / std::sqrt(len2)) : glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 SphericalReactor::WorldDirection(const ReactorPort& port) const
{
    return SafeNormalize(orientation * SafeNormalize(port.localDirection));
}

glm::vec3 SphericalReactor::WorldPortCenter(const ReactorPort& port) const
{
    return position + orientation * (SafeNormalize(port.localDirection) * radius);
}

bool SphericalReactor::IsOpening(const glm::vec3& localPoint, float particleRadius) const
{
    const float distance = glm::length(localPoint);
    if (distance < 1e-6f)
        return false;

    const glm::vec3 radial = localPoint / distance;
    const float rimTolerance = particleRadius + 0.08f;

    auto matches = [&](const ReactorPort& port)
        {
            const glm::vec3 d = SafeNormalize(port.localDirection);
            const float angularDistance = std::acos(std::clamp(glm::dot(radial, d), -1.0f, 1.0f));
            const float openingAngle = std::atan2(port.diameter * 0.5f, std::max(radius, 0.001f));
            return angularDistance <= openingAngle + rimTolerance / std::max(radius, 0.001f);
        };

    for (const auto& port : inlets)
        if (matches(port)) return true;
    for (const auto& port : outlets)
        if (matches(port)) return true;
    return false;
}

ReactorCollision SphericalReactor::CollideInside(const glm::vec3& particlePosition,
    float particleRadius) const
{
    const glm::vec3 local = glm::inverse(orientation) * (particlePosition - position);
    const float distance = glm::length(local);
    const float limit = radius - particleRadius;

    if (distance <= limit)
        return {};

    ReactorCollision result;
    result.collided = true;
    result.throughOpening = IsOpening(local, particleRadius);
    result.normal = distance > 1e-6f ? glm::normalize(particlePosition - position) : glm::vec3(0.0f, 1.0f, 0.0f);
    result.penetration = distance - limit;

    if (result.throughOpening)
        result.collided = false;
    return result;
}

glm::vec3 SphericalReactor::SpawnPosition(const ReactorPort& port, float particleRadius) const
{
    const glm::vec3 d = WorldDirection(port);
    return WorldPortCenter(port) - d * (particleRadius + port.axialLength * 0.5f);
}

glm::vec3 SphericalReactor::SpawnVelocity(const ReactorPort& port, float speed) const
{
    return WorldDirection(port) * std::max(speed, 0.0f);
}

bool SphericalReactor::IsInside(const glm::vec3& p, float margin) const
{
    return glm::length(p - position) <= radius + margin;
}

std::string SphericalReactor::Specifications() const
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(2);
    out << name << "\n";
    out << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
    out << "Radius: " << radius << "\n";
    out << "Inlets: " << inlets.size() << "  Outlets: " << outlets.size() << "\n";
    for (std::size_t i = 0; i < inlets.size(); ++i)
        out << "Inlet " << i << " diameter=" << inlets[i].diameter << " direction=("
        << inlets[i].localDirection.x << ", " << inlets[i].localDirection.y << ", "
        << inlets[i].localDirection.z << ")\n";
    for (std::size_t i = 0; i < outlets.size(); ++i)
        out << "Outlet " << i << " diameter=" << outlets[i].diameter << " direction=("
        << outlets[i].localDirection.x << ", " << outlets[i].localDirection.y << ", "
        << outlets[i].localDirection.z << ")\n";
    return out.str();
}