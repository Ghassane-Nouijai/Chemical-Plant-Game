#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Reactor.h"

class ParticleSandbox;

class PanelGUI
{
public:
    PanelGUI() = default;

    void Draw(ParticleSandbox& sandbox);

    std::vector<std::shared_ptr<SphericalReactor>>& GetReactors() { return m_Reactors; }
    const std::vector<std::shared_ptr<SphericalReactor>>& GetReactors() const { return m_Reactors; }

    int GetSelectedIndex() const { return m_SelectedIndex; }
    bool IsSimulationRunning() const { return m_SimulationRunning; }

private:
    void DrawPartsPanel(ParticleSandbox& sandbox);
    void DrawReactorInspector();
    void DrawPortList(std::vector<ReactorPort>& ports, bool inlet);
    void AddReactor(ParticleSandbox& sandbox);
    void ApplyStyle();

    std::vector<std::shared_ptr<SphericalReactor>> m_Reactors;
    int m_SelectedIndex = -1;
    bool m_SimulationRunning = false;
    bool m_StyleApplied = false;
};