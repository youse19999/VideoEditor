#include "Window.h"
#include <cassert> // assertを使うために追加
#include <imgui.h>

Window::Window(int width, int height, const std::wstring& title) : hwnd_(nullptr) {
    // 幅と高さは正の値であるべき
    assert(width > 0 && height > 0);

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    assert(hInstance != nullptr);

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SimpleWindowClassName";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // 登録失敗チェック（すでに登録されている場合などはATOMが0を返すことがあるが、
    // 同名クラスの二重登録などをデバッグ時に検知するため）
    ATOM atom = RegisterClassW(&wc);
    assert(atom != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS);

    // lpParam には何も渡さない（nullptr）
    hwnd_ = CreateWindowExW(
        0,
        L"SimpleWindowClassName",
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr, hInstance, nullptr
    );

    // ウィンドウの作成に失敗していないことをアサート
    assert(hwnd_ != nullptr && "Failed to create window via CreateWindowExW");
}

Window::~Window() {
    hwnd_ = nullptr;
}

void Window::Show(int nCmdShow) {
    // ウィンドウハンドルが有効な状態でのみ呼ぶべき
    assert(hwnd_ != nullptr);

    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);
}

bool Window::ProcessMessages() {
    MSG msg = {};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true;
}

HWND Window::GetHandle() const {
    return hwnd_;
}
// ImGui のWin32メッセージハンドラを宣言
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// thisを使わない、ただのシンプルなメッセージ処理関数
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}