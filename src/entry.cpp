#include <window.h>
#include <directx.h>
int main()
{
    Window window(800, 600, L"My C++ Window");
    window.Show(1);

    DirectXDevice device;
    device.InitDirect3D(window.GetHandle(), 800, 600);
    while (window.ProcessMessages()) {
        device.Render();
        Sleep(10);
    }

    return 0;
}