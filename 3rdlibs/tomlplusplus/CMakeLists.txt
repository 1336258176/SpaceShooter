add_library(toml++ INTERFACE)
target_include_directories(toml++ INTERFACE ./)
target_compile_features(toml++ INTERFACE cxx_std_17)