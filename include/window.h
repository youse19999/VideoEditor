#pragma once

#include <windows.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::wstring& title);
    ~Window();

    void Show(int nCmdShow);
    bool ProcessMessages();
    HWND GetHandle() const;

private:
    HWND hwnd_;

    // 静的プロシージャだが、thisを使わずシンプルに記述
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};