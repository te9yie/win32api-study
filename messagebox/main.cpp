#include <Windows.h>

int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance,
                    [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine,
                    [[maybe_unused]] int nCmdShow) {
  MessageBox(NULL, L"メッセージを表示する", L"タイトル", MB_OK);
  return 0;
}
