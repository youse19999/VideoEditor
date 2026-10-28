#include <window.h>
int main()
{
    Window window(800, 600, L"My C++ Window");
    window.Show(1);

    while (window.ProcessMessages()) {

        Sleep(10);
    }

    return 0;
}