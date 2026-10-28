#pragma once
#include "component.h"
#include "imgui.h"

class ImGuiDemoComponent : public Component {
private:
    bool m_showDemoWindow = true; // デモウィンドウの表示・非表示を管理するフラグ

public:
    void Render() override {
        if (m_showDemoWindow) {
            ImGui::ShowDemoWindow(&m_showDemoWindow);
        }
    }
};