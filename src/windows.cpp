#include "windows.hpp"

Windows::Windows(const std::string& title) :
    windows_(SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WindowsWidth,
                              WindowsHeight,
                              SDL_WINDOW_SHOWN),
             DestroyWindows) {}
