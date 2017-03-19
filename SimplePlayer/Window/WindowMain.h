#pragma once
#include "Window.h"

#include <map>
#include <atomic>
#include <thread>
#include <libhelpers\Thread\critical_section.h>
#include <libhelpers\Thread\condition_variable.h>
#include <future>

enum class WindowMessages : uint32_t {
    Shutdown // used to call DestroyWindow from window thread
};

struct WindowInitData {
    std::promise<bool> InitPromise;
    std::wstring windowTitle;
};

class WindowMain {
public:
    WindowMain(WindowBaseData &baseData, Window &window, const std::wstring &windowTitle);
    ~WindowMain();

    void WaitForClose();

private:
    WindowBaseData &baseData;
    Window &window;

    bool destroyed;
    thread::critical_section destroyCs;
    thread::condition_variable destroyCv;

    void Show();

    void WndThreadMain(WindowInitData *initData);
    LRESULT WndProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);

    static std::atomic_uint64_t nextWndId;
    // It should be enough to have 1 CS for all wnd's
    // Also SetWindowLongPtr should NOT be used since :
    // http://stackoverflow.com/questions/117792/best-method-for-storing-this-pointer-for-use-in-wndproc
    static thread::critical_section thisMapCs;
    static std::map<HWND, WindowMain *> thisMap;
    static std::map<std::wstring, WindowMain *> thisMap2;

    static LRESULT CALLBACK WndProcTmp(HWND h, UINT msg, WPARAM wparam, LPARAM lparam);
    static void AddThisMap(HWND h, WindowMain *_this);
    static void RemoveThisMap(HWND h, const std::wstring &name);

    static void AddThisMap2(const std::wstring &name, WindowMain *_this);

    static std::wstring GetWindowClassName(HWND h);
};