#include "PanelGUI.h"

#include <algorithm>
#include <string>

#include "imgui.h"
#include "ParticleSandbox.h"

void PanelGUI::Draw(ParticleSandbox& sandbox)
{
    DrawPartsPanel(sandbox);
    DrawReactorInspector();
}

void PanelGUI::DrawPartsPanel(ParticleSandbox& sandbox)
{
    ImGui::Begin("Chemical Plant");

    ImGui::TextUnformatted("Parts");
    ImGui::Separator();

    if (ImGui::Button("Add spherical reactor", ImVec2(-1.0f, 0.0f)))
        AddReactor(sandbox);

    if (ImGui::BeginDragDropSource())
    {
        constexpr int payload = 1;
        ImGui::SetDragDropPayload("SPHERICAL_REACTOR", &payload, sizeof(payload));
        ImGui::TextUnformatted("Place spherical reactor");
        ImGui::EndDragDropSource();
    }

    ImGui::Separator();
    ImGui::Text("Reactors: %d", static_cast<int>(m_Reactors.size()));

    for (std::size_t i = 0; i < m_Reactors.size(); ++i)
    {
        const bool selected = m_SelectedIndex == static_cast<int>(i);
        std::string label = m_Reactors[i]->name + "##reactor_" + std::to_string(i);
        if (ImGui::Selectable(label.c_str(), selected))
            m_SelectedIndex = static_cast<int>(i);
    }

    ImGui::Separator();
    const char* simulationLabel = m_SimulationRunning
        ? "Pause simulation"
        : "Start simulation";

    if (ImGui::Button(simulationLabel, ImVec2(-1.0f, 0.0f)))
    {
        m_SimulationRunning = !m_SimulationRunning;
        sandbox.SetRunning(m_SimulationRunning);
    }

    ImGui::End();
}

void PanelGUI::DrawReactorInspector()
{
    if (m_SelectedIndex < 0 ||
        m_SelectedIndex >= static_cast<int>(m_Reactors.size()))
        return;

    SphericalReactor& reactor = *m_Reactors[m_SelectedIndex];

    ImGui::Begin("Reactor Inspector");

    ImGui::TextUnformatted(reactor.name.c_str());
    ImGui::Separator();

    ImGui::DragFloat3("Position", &reactor.position.x, 0.05f);
    ImGui::DragFloat("Radius", &reactor.radius, 0.05f, 0.25f, 100.0f);

    if (ImGui::CollapsingHeader("Inlets", ImGuiTreeNodeFlags_DefaultOpen))
    {
        int count = static_cast<int>(reactor.inlets.size());
        if (ImGui::SliderInt("Count##inlets", &count, 0, 8))
            reactor.inlets.resize(static_cast<std::size_t>(count));

        for (int i = 0; i < count; ++i)
        {
            ImGui::PushID(100 + i);
            ImGui::Text("Inlet %d", i);
            ImGui::DragFloat3("Direction", &reactor.inlets[i].localDirection.x,
                0.01f, -1.0f, 1.0f);
            ImGui::DragFloat("Diameter", &reactor.inlets[i].diameter,
                0.01f, 0.05f, 20.0f);
            ImGui::DragFloat("Angle", &reactor.inlets[i].angle,
                0.5f, -180.0f, 180.0f);
            ImGui::PopID();
        }
    }

    if (ImGui::CollapsingHeader("Outlets", ImGuiTreeNodeFlags_DefaultOpen))
    {
        int count = static_cast<int>(reactor.outlets.size());
        if (ImGui::SliderInt("Count##outlets", &count, 0, 8))
            reactor.outlets.resize(static_cast<std::size_t>(count));

        for (int i = 0; i < count; ++i)
        {
            ImGui::PushID(200 + i);
            ImGui::Text("Outlet %d", i);
            ImGui::DragFloat3("Direction", &reactor.outlets[i].localDirection.x,
                0.01f, -1.0f, 1.0f);
            ImGui::DragFloat("Diameter", &reactor.outlets[i].diameter,
                0.01f, 0.05f, 20.0f);
            ImGui::DragFloat("Angle", &reactor.outlets[i].angle,
                0.5f, -180.0f, 180.0f);
            ImGui::PopID();
        }
    }

    if (ImGui::Button(reactor.showInside ? "Hide inside" : "Show inside"))
        reactor.showInside = !reactor.showInside;

    if (reactor.showInside)
    {
        ImGui::Separator();
        ImGui::TextWrapped("%s", reactor.Specifications().c_str());
    }

    ImGui::End();
}

void PanelGUI::AddReactor(ParticleSandbox& sandbox)
{
    auto reactor = std::make_shared<SphericalReactor>(5.0f);
    reactor->position = glm::vec3(0.0f, reactor->radius, 0.0f);
    reactor->name = "Spherical reactor " + std::to_string(m_Reactors.size() + 1);

    m_Reactors.push_back(reactor);
    m_SelectedIndex = static_cast<int>(m_Reactors.size()) - 1;
    sandbox.AddReactor(reactor);
}