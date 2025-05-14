add_subdirectory(spdlog)
add_subdirectory(tomlplusplus)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE spdlog::spdlog toml++)