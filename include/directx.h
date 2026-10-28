#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class DirectXDevice {
private:
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pImmediateContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_pRenderTargetView = nullptr;

public:
    // コンストラクタ / デストラクタ
    DirectXDevice() = default;
    ~DirectXDevice() {
        CleanupDirect3D();
    }

    HRESULT InitDirect3D(HWND hWnd, int width, int height);

    void Resize(int width, int height);

    // クリーンアップ関数
    void CleanupDirect3D();
    void Render();
};