#include <iostream>
#include <cassert>

int main() {
    // 簡単なテストの例
    int result = 2 + 2;
    assert(result == 4);

    std::cout << "All tests passed!" << std::endl;
    return 0; // 0を返すとCTestは「成功」と判定する
}