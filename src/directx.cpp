#include "directx.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

HRESULT DirectXDevice::InitDirect3D(HWND hWnd, int width, int height) {
    HRESULT hr = S_OK;

    // 1. スワップチェインの設定
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    // 2. 対応する機能レベル（Feature Level）の定義
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    UINT numLevels = 3;
    D3D_FEATURE_LEVEL featureLevel;

    // デバッグフラグ（必要に応じて）
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // 3. デバイスとスワップチェインの作成
    hr = D3D11CreateDeviceAndSwapChain(
        NULL,                     // ビデオアダプター（NULLはデフォルト）
        D3D_DRIVER_TYPE_HARDWARE, // ドライバータイプ
        NULL,                     // ソフトウェアパッチ用（通常NULL）
        createDeviceFlags,        // フラグ
        levels,                   // 機能レベルの配列
        numLevels,                // 機能レベルの数
        D3D11_SDK_VERSION,        // SDKバージョン
        &sd,                      // スワップチェイン設定
        &g_pSwapChain,            // [出力] スワップチェイン
        &g_pd3dDevice,            // [出力] デバイス
        &featureLevel,            // [出力] 決定された機能レベル
        &g_pImmediateContext      // [出力] デバイスコンテキスト
    );
    if (FAILED(hr)) return hr;

    // 4. レンダーターゲットビューの作成
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (FAILED(hr)) return hr;

    hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);

    // バックバッファの参照カウンタを解放（RTVが保持するため）
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    // 5. レンダーターゲットの設定
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

    // 6. ビューポートの設定
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    g_pImmediateContext->RSSetViewports(1, &vp);

    // ==========================================
    // 7. Dear ImGui の初期化
    // ==========================================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // キーボード操作の有効化

    // スタイル設定（ダークテーマ）
    ImGui::StyleColorsDark();

    // バックエンドの初期化
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pImmediateContext);

    return S_OK;
}

void DirectXDevice::CleanupDirect3D() {
    // ==========================================
    // Dear ImGui の終了処理
    // ==========================================
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // DirectX リソースの解放
    if (g_pRenderTargetView) {
        g_pRenderTargetView->Release();
        g_pRenderTargetView = nullptr;
    }
    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    if (g_pImmediateContext) {
        g_pImmediateContext->Release();
        g_pImmediateContext = nullptr;
    }
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void DirectXDevice::Render() {
    // ==========================================
    // 1. ImGui フレームの開始
    // ==========================================
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // ==========================================
    // 2. UI コンポーネントの記述（例）
    // ==========================================
    ImGui::Begin("VideoEditor Control Panel");
    ImGui::Text("Hello from DirectX 11 & ImGui!");
    if (ImGui::Button("Click Me")) {
        // ボタンが押されたときの処理
    }
    ImGui::End();

    // ==========================================
    // 3. 描画データの生成（ImGuiのRender）
    // ==========================================
    ImGui::Render();

    // ==========================================
    // 4. 画面を青色でクリアして描画ターゲット設定
    // ==========================================
    float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // 青
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

    // ==========================================
    // 5. ImGui の描画コマンドを実行
    // ==========================================
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // ==========================================
    // 6. 画面に反映 (Present)
    // ==========================================
    g_pSwapChain->Present(1, 0);
}