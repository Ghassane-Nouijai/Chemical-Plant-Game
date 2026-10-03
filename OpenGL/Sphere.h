#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "IRenderable.h"
#include "Mesh.h"
#include "Shader.h"
#include "InstancedMesh.h"

class Sphere : public IRenderable
{
private:
    float m_Radius;
    unsigned int m_Precision;
    std::unique_ptr<Mesh> m_Mesh;
    std::unique_ptr<InstancedMesh> m_InstancedMesh;
    unsigned int m_IndexCount = 0;
    std::size_t m_MaxInstances = 0;

public:
    Sphere(float radius, unsigned int precision);
    std::vector<float> CreateVertices(float radius, unsigned int precision);
    std::vector<unsigned int> CreateIndices(unsigned int precision);
    void Draw(Shader& shader, const glm::vec3& position, const glm::quat& orientation) override;
    void DrawScaled(Shader& shader, const glm::vec3& position, const glm::quat& orientation, float scale);
    void UpdateInstances(const std::vector<float>& instanceData) override;
    void DrawInstanced(Shader& shader) override;

private:
    void BuildInstancedMesh(std::size_t maxInstances);
};