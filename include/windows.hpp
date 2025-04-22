#ifndef WINDOWS_HPP__
#define WINDOWS_HPP__

#include "pch.hpp"

inline auto DestroyWindows = [](SDL_Window* windows) { SDL_DestroyWindow(windows); };

class Windows final {
 public:
  Windows(const std::string& title);
  SDL_Window* get() const { return windows_.get(); }

  const int WindowsWidth = 600;
  const int WindowsHeight = 750;

 private:
  std::unique_ptr<SDL_Window, decltype(DestroyWindows)> windows_;
};

#endif  // WINDOWS_HPP__