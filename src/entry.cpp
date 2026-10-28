#include <window.h>
#include <directx.h>
#include <gameobject.h>
#include <imgui_demo_component.h>
#include <scene.h>
int main()
{
    Window window(800, 600, L"My C++ Window");
    window.Show(1);

    DirectXDevice device;
    device.InitDirect3D(window.GetHandle(), 800, 600);

    GameObject* demoObj = Scene::GetInstance().CreateGameObject();
    demoObj->AddComponent<ImGuiDemoComponent>();

    while (window.ProcessMessages()) {
        device.Render();
        Sleep(10);
    }

    return 0;
}