#ifndef WINDOWS_HPP__
#define WINDOWS_HPP__

#include "pch.hpp"

auto DestroyWindows = [](SDL_Window* windows) { SDL_DestroyWindow(windows); };

class Windows final {
 public:
  Windows(const std::string& title);
  SDL_Window* get() const { return windows_.get(); }

  static const int WindowsWidth = 600;
  static const int WindowsHeight = 800;

 private:
  std::unique_ptr<SDL_Window, decltype(DestroyWindows)> windows_;
};

#endif  // WINDOWS_HPP__