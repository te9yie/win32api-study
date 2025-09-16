#include <Windows.h>
#include <memory>

#define IDC_BUTTON 100

namespace {

  struct LocalMemoryFree {
    void operator()(HLOCAL hMem) { LocalFree(hMem); }
  };
  using MessagePtr = std::unique_ptr<WCHAR, LocalMemoryFree>;

  MessagePtr MakeErrorMessage(DWORD nCode) {
    LPWSTR pBuffer = nullptr;
    const DWORD nLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, nCode, 0, reinterpret_cast<LPWSTR>(&pBuffer), 0, nullptr);
    if (nLen == 0) {
      return {};
    }
    return MessagePtr(pBuffer);
  }

  void ShowErrorMessage(HWND hWnd, DWORD nCode, PWSTR pApiName) {
    WCHAR buffer[1024];
    auto pErrorMessage = MakeErrorMessage(nCode);
    swprintf_s(buffer, L"%s: %s", pApiName, pErrorMessage.get());
    MessageBoxW(hWnd, buffer, L"エラー", MB_OK | MB_ICONERROR);
  }

  LRESULT CALLBACK WindowProc(HWND hWnd,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam) {
    switch (uMsg) {
      case WM_CREATE: {
        const auto hInstance =
            reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
        // const HWND hButton =
        CreateWindowEx(0, L"BUTTON", L"ボタン", WS_CHILD | WS_VISIBLE, 20, 10,
                       200, 100, hWnd, reinterpret_cast<HMENU>(IDC_BUTTON),
                       hInstance, nullptr);
        break;
      }
      case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_BUTTON) {
          MessageBox(hWnd, L"ボタンが押された", L"通知",
                     MB_OK | MB_ICONINFORMATION);
        }
        break;
      }
      case WM_DESTROY:
        PostQuitMessage(0);
        break;
      default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
  }

}  // namespace

int WINAPI wWinMain(HINSTANCE hInstance,
                    [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine,
                    [[maybe_unused]] int nCmdShow) {
  WNDCLASSEX wcex;
  ZeroMemory(&wcex, sizeof(wcex));
  wcex.cbSize = sizeof(wcex);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hInstance = hInstance;
  wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  wcex.lpfnWndProc = WindowProc;
  wcex.lpszClassName = L"Study";

  const ATOM atom = RegisterClassEx(&wcex);
  if (atom == 0) {
    ShowErrorMessage(NULL, GetLastError(), L"RegisterClassEx");
    return 0;
  }

  const HWND hWnd =
      CreateWindowEx(0, MAKEINTATOM(atom), L"ボタン", WS_OVERLAPPEDWINDOW,
                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                     NULL, NULL, hInstance, nullptr);
  if (!hWnd) {
    ShowErrorMessage(NULL, GetLastError(), L"CreateWindowEx");
    return 0;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  MSG msg;
  BOOL bRet;
  for (;;) {
    bRet = GetMessage(&msg, NULL, 0, 0);
    if (bRet == FALSE) {
      break;
    }
    if (bRet < 0) {
      ShowErrorMessage(NULL, GetLastError(), L"GetMessage");
      return 0;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return static_cast<int>(msg.wParam);
}
