//
// Created by huhua on 2021/8/11.
//
#include <iostream>
#include <Windows.h>
#include <easybot/util_window.h>
#include "util/util_color.h"
#include "util/util_keyboard.h"

const std::string WINDOW_NAME = "Zuma Deluxe 1.0";

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
        }  else if (GetAsyncKeyState(VK_NUMPAD9)) {
//            clickMouseLeft();
            inputKey('a');
            Sleep(500);
        }
        else if (GetAsyncKeyState(VK_NUMPAD3)) {
            break;
        }

        Sleep(10);
    }
    return 0;
}