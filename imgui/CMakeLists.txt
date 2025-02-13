cmake_minimum_required(VERSION 3.10)
project(PacketSnifferGUI)

set(CMAKE_CXX_STANDARD 17)

# Include Dear ImGui and backends
include_directories(${CMAKE_SOURCE_DIR}/imgui ${CMAKE_SOURCE_DIR}/imgui/backends)

# Add Dear ImGui source files
file(GLOB IMGUI_SOURCES
    "${CMAKE_SOURCE_DIR}/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

# Add main.cpp and Dear ImGui sources to the executable
add_executable(PacketSnifferGUI main.cpp ${IMGUI_SOURCES})

# Link libraries
target_link_libraries(PacketSnifferGUI glfw GL GLU pcap)
