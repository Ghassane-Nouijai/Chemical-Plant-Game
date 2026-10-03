#include "PanelGUI.h"

#include <algorithm>
#include <string>

#include "imgui.h"
#include "ParticleSandbox.h"

void PanelGUI::ApplyStyle()
{
    if (m_StyleApplied)
        return;

    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 0.0f;
    s.ChildRounding = 3.0f;
    s.FrameRounding = 3.0f;
    s.PopupRounding = 3.0f;
    s.ScrollbarRounding = 3.0f;
    s.GrabRounding = 3.0f;
    s.WindowBorderSize = 1.0f;
    s.FrameBorderSize = 1.0f;
    s.ItemSpacing = ImVec2(8.0f, 6.0f);
    s.FramePadding = ImVec2(8.0f, 5.0f);

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg] = ImVec4(0.075f, 0.085f, 0.105f, 0.98f);
    c[ImGuiCol_ChildBg] = ImVec4(0.055f, 0.062f, 0.078f, 1.0f);
    c[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.135f, 0.165f, 1.0f);
    c[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.25f, 0.34f, 1.0f);
    c[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.32f, 0.43f, 1.0f);
    c[ImGuiCol_Button] = ImVec4(0.13f, 0.19f, 0.27f, 1.0f);
    c[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.34f, 0.48f, 1.0f);
    c[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.43f, 0.58f, 1.0f);
    c[ImGuiCol_Header] = ImVec4(0.13f, 0.19f, 0.27f, 1.0f);
    c[ImGuiCol_HeaderHovered] = ImVec4(0.19f, 0.34f, 0.48f, 1.0f);
    c[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.43f, 0.58f, 1.0f);
    c[ImGuiCol_Tab] = ImVec4(0.10f, 0.14f, 0.19f, 1.0f);
    c[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.36f, 0.50f, 1.0f);
    c[ImGuiCol_TabActive] = ImVec4(0.16f, 0.28f, 0.39f, 1.0f);
    c[ImGuiCol_Border] = ImVec4(0.23f, 0.28f, 0.35f, 1.0f);
    m_StyleApplied = true;
}

void PanelGUI::Draw(ParticleSandbox& sandbox)
{
    ApplyStyle();
    DrawPartsPanel(sandbox);
    DrawReactorInspector();
}

void PanelGUI::DrawPartsPanel(ParticleSandbox& sandbox)
{
    const float width = 300.0f;
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("CHEMICAL PLANT##PartsPanel", nullptr, flags);
    ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.0f, 1.0f), "CHEMICAL PLANT");
    ImGui::TextDisabled("BUILD MODE / SIMULATION CONTROL");
    ImGui::Separator();

    if (ImGui::Button("+  Spherical reactor", ImVec2(-1.0f, 0.0f)))
        AddReactor(sandbox);

    if (ImGui::BeginDragDropSource())
    {
        constexpr int payload = 1;
        ImGui::SetDragDropPayload("SPHERICAL_REACTOR", &payload, sizeof(payload));
        ImGui::TextUnformatted("Place spherical reactor");
        ImGui::EndDragDropSource();
    }

    ImGui::Spacing();
    ImGui::Text("REACTORS  (%d)", static_cast<int>(m_Reactors.size()));
    ImGui::Separator();

    for (std::size_t i = 0; i < m_Reactors.size(); ++i)
    {
        const bool selected = m_SelectedIndex == static_cast<int>(i);
        std::string label = m_Reactors[i]->name + "##reactor_" + std::to_string(i);
        if (ImGui::Selectable(label.c_str(), selected, 0, ImVec2(0.0f, 30.0f)))
            m_SelectedIndex = static_cast<int>(i);
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 70.0f);
    ImGui::Separator();
    if (ImGui::Button(m_SimulationRunning ? "||  Pause simulation" : ">  Start simulation", ImVec2(-1.0f, 0.0f)))
    {
        m_SimulationRunning = !m_SimulationRunning;
        sandbox.SetRunning(m_SimulationRunning);
    }
    ImGui::End();
}

void PanelGUI::DrawReactorInspector()
{
    if (m_SelectedIndex < 0 || m_SelectedIndex >= static_cast<int>(m_Reactors.size()))
        return;

    SphericalReactor& reactor = *m_Reactors[m_SelectedIndex];
    ImGui::SetNextWindowPos(ImVec2(315.0f, 20.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(360.0f, 650.0f), ImGuiCond_FirstUseEver);
    ImGui::Begin("REACTOR INSPECTOR##Inspector");

    ImGui::TextColored(ImVec4(0.95f, 0.72f, 0.25f, 1.0f), "%s", reactor.name.c_str());
    ImGui::Separator();
    ImGui::DragFloat3("Position", &reactor.position.x, 0.05f);
    if (ImGui::DragFloat("Radius", &reactor.radius, 0.05f, 0.25f, 100.0f))
        reactor.position.y = std::max(reactor.position.y, reactor.radius);

    if (ImGui::Button(reactor.showInside ? "Show reactor shell" : "Hide reactor / show inside", ImVec2(-1.0f, 0.0f)))
        reactor.showInside = !reactor.showInside;

    if (ImGui::CollapsingHeader("INLETS", ImGuiTreeNodeFlags_DefaultOpen))
        DrawPortList(reactor.inlets, true);
    if (ImGui::CollapsingHeader("OUTLETS", ImGuiTreeNodeFlags_DefaultOpen))
        DrawPortList(reactor.outlets, false);

    if (ImGui::Button("Specifications", ImVec2(-1.0f, 0.0f)))
        reactor.showSpecifications = !reactor.showSpecifications;
    if (reactor.showSpecifications)
        ImGui::TextWrapped("%s", reactor.Specifications().c_str());

    ImGui::End();
}

void PanelGUI::DrawPortList(std::vector<ReactorPort>& ports, bool inlet)
{
    int count = static_cast<int>(ports.size());
    const char* id = inlet ? "Count##inlets" : "Count##outlets";
    if (ImGui::SliderInt(id, &count, 0, 8))
        ports.resize(static_cast<std::size_t>(count));

    for (int i = 0; i < count; ++i)
    {
        ReactorPort& port = ports[static_cast<std::size_t>(i)];
        ImGui::PushID((inlet ? 1000 : 2000) + i);
        ImGui::Text("%s %d", inlet ? "Inlet" : "Outlet", i + 1);
        ImGui::SameLine(ImGui::GetWindowWidth() - 100.0f);
        if (ImGui::SmallButton(port.open ? "Open" : "Closed"))
            port.open = !port.open;
        ImGui::DragFloat3("Direction", &port.localDirection.x, 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat("Diameter", &port.diameter, 0.01f, 0.05f, 20.0f);
        ImGui::DragFloat("Angle", &port.angle, 0.5f, -180.0f, 180.0f);
        ImGui::Separator();
        ImGui::PopID();
    }
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