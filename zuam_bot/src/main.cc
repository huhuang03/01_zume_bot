//
// Created by huhua on 2021/8/11.
//
#include <iostream>
#include <Windows.h>
#include <easybot/util_window.h>
#include "util/util_color.h"
#include <easybot/util_process.h>
#include <iterator>
#include <TlHelp32.h>

const std::string WINDOW_NAME = "Zuma Deluxe 1.0";

const std::string MAIN_MODULE_NAME = "popcapgame1.exe";


DWORD eb::getBaseAddr(DWORD processId, const std::string &moduleName) {
    if (processId <= 0) {
        return 0;
    }
    // ok, let's do the logic
    auto thSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processId);
    if (!thSnap) {
        std::cout << "why thSnap is empty" << std::endl;
        return 0;
    }

    MODULEENTRY32 me{0};
    me.dwSize = sizeof(MODULEENTRY32);
    // need close pe?
    if (!Module32First(thSnap, &me)) {
        CloseHandle(thSnap);
        return 0;
    }

    DWORD rst = 0;
    do {
        if (moduleName == me.szModule) {
            CloseHandle(thSnap);
            rst = (DWORD)me.modBaseAddr;
            break;
        }
    } while (Module32Next(thSnap, &me));

    CloseHandle(thSnap);
    return rst;
}

static DWORD getTheScoreAddr(HWND hwnd) {
    // ok, I have an hwnd, how can I have the process?
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    printf("pid: %0lX\n", pid);
    if (pid <= 0) {
        std::cout << "why pid <= 0" << std::endl;
        return 0;
    }

    DWORD baseAddr = eb::getBaseAddr(pid, MAIN_MODULE_NAME);
    printf("baseAddr: %0lX\n", baseAddr);

    // but now we need open the process. right?
    auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (hProcess == nullptr) {
        std::cout << "Can't open process " << std::endl;
        return 0;
    }
    int offsets[] = {0x19BD40, 0x6c, 0x34, 0xC8, 0x8, 0xF0, 0x10, 0xE8};
    uint32_t rst = 0;
    DWORD addr = baseAddr;
    size_t readed = 0;

    for (int i = 0; i < std::size(offsets); i++) {
        auto offset = offsets[i];
        addr += offset;
        printf("item addr: %0lX\n", addr);
        if (i != std::size(offsets) - 1) {
            ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(addr), &rst, sizeof(rst), &readed);
            addr = rst;
        }
    }

    CloseHandle(hProcess);
    return addr;
}

static uint32_t getScore(HWND hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid <= 0) {
        std::cout << "why pid <= 0" << std::endl;
        return 0;
    }

    auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (hProcess == nullptr) {
        std::cout << "Can't open process " << std::endl;
        return 0;
    }

    uint32_t rst = 0;
    size_t readed = 0;
    auto addr = getTheScoreAddr(hwnd);
    printf("addr: %0lX\n", addr);
    // ok, now we have the right addr?
    ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(addr), &rst, sizeof(rst), &readed);
    printf("score: %d\n", rst);
    CloseHandle(hProcess);
    return rst;
}

// Reading And Writing Memory in C++ | Game Hacking Tutorial Part 4
static uint32_t setScore(HWND hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid <= 0) {
        std::cout << "why pid <= 0" << std::endl;
        return 0;
    }

    auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (hProcess == nullptr) {
        std::cout << "Can't open process " << std::endl;
        return 0;
    }

    uint32_t score = 999999;
    size_t write = 0;
    auto addr = getTheScoreAddr(hwnd);
    // ok, now we have the right addr?
    WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(addr), &score, sizeof score, &write);
    return score;
}

int main() {
    HWND hwnd = nullptr;
    while (true) {
        if (hwnd == nullptr) {
            hwnd = eb::findWindow(WINDOW_NAME);
            std::cout << "hwnd: " << hwnd << std::endl;
        }

        if (hwnd == nullptr) {
            std::cout << "Please open game";
            continue;
        }

        if (GetAsyncKeyState(VK_NUMPAD1)) { // MousePosition
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);
            std::cout << "cursor pos: " << p.x << ", " << p.y << std::endl;
            Sleep(1000);
        } else if (GetAsyncKeyState(VK_NUMPAD2)) {  // PixelColor
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);
            if (p.x < 0 || p.y < 0) {
                std::cout << "out window" << std::endl;
            } else {
                HDC hdc = GetDC(hwnd);
                COLORREF c = GetPixel(hdc, p.x, p.y);
                std::cout << color_str(c) << std::endl;
            }

            Sleep(1000);
        } else if (GetAsyncKeyState(VK_NUMPAD8)) {
            setScore(hwnd);
            Sleep(1000);
        }
        else if (GetAsyncKeyState(VK_NUMPAD9)) {
            getScore(hwnd);
            Sleep(1000);
        }
        else if (GetAsyncKeyState(VK_NUMPAD3)) {
            break;
        }

        Sleep(10);
    }
    return 0;
}