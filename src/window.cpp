#include "Window.h"

Window::Window(int width, int height, const std::wstring& title) : hwnd_(nullptr) {
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SimpleWindowClassName";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

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
}

Window::~Window() {
    if (hwnd_) {
        DestroyWindow(hwnd_);
    }
}

void Window::Show(int nCmdShow) {
    if (hwnd_) {
        ShowWindow(hwnd_, nCmdShow);
        UpdateWindow(hwnd_);
    }
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

// thisを使わない、ただのシンプルなメッセージ処理関数
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
    DestroyUsage: // （そのまま閉じる）
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}