#include <iostream>
#include <format>
#include "wtypes.h"
#include <chrono>
#include <thread>

int16_t windowWidth;
int16_t windowHeight;

int16_t mouseX;
int16_t mouseY;

int16_t targetMouseX;
int16_t targetmouseY;

std::chrono::milliseconds targetIdleTimems;
std::chrono::milliseconds timeSinceLastMouseMovement = std::chrono::duration<long long, std::milli>{ 0 };
std::chrono::time_point timeMouseLastMoved = std::chrono::system_clock::now();



void GetScreenResolution(int16_t& width, int16_t& height)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    width = desktop.right;
    height = desktop.bottom;
}


POINT getCurrentMousePosition() {
    POINT p;
    GetCursorPos(&p);
    return p;
}


boolean mouseHasMoved() {
    POINT updatedMousePosition = getCurrentMousePosition();
    return (updatedMousePosition.x != mouseX && updatedMousePosition.y != mouseY);
}

void updateMousePosition() {
    POINT updatedMousePosition = getCurrentMousePosition();
    mouseX = updatedMousePosition.x;
    mouseY = updatedMousePosition.y;
}


int main()
{
    GetScreenResolution(windowWidth, windowHeight);
    std::cout << std::format("Screen Resolution {}x{}.", windowWidth, windowHeight) 
        << std::endl;

    int16_t targetIdleTime;
    std::cout << "Move mouse after how many seconds of idle time: ";
    std::cin >> targetIdleTime;
    targetIdleTimems = std::chrono::milliseconds(targetIdleTime * 1000);

    std::cout << "Move mouse to pixel X: ";
    std::cin >> targetMouseX;

    std::cout << "Move mouse to pixel Y: ";
    std::cin >> targetmouseY;


    updateMousePosition();
    timeMouseLastMoved = std::chrono::system_clock::now();
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (mouseHasMoved()) {
            timeMouseLastMoved = std::chrono::system_clock::now();
            updateMousePosition();
        }
        else {
            std::chrono::duration duration = std::chrono::system_clock::now() - timeMouseLastMoved;
            if (duration > targetIdleTimems) {
                SetCursorPos(targetMouseX, targetmouseY);
            }
        }
    }
}
