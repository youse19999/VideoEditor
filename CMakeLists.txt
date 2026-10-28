cmake_minimum_required (VERSION 3.8)

enable_testing()

include(FetchContent)

# --- spdlog の取得 ---
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.x
)
FetchContent_MakeAvailable(spdlog)

# --- Dear ImGui の取得 ---
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.93.0 # 必要に応じてバージョンタグやブランチ名(master/docking等)に変更してください
)
FetchContent_MakeAvailable(imgui)

# Dear ImGui 用のライブラリターゲットを作成 (コア + Win32 + DirectX 11 バックエンド)
add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)

# インクルードパスの設定 (本体とバインド用の両方を通す)
target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# DirectX 11 バックエンドに必要なシステムライブラリをリンク
target_link_libraries(imgui PUBLIC d3d11 dxgi)
# -------------------------

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VideoEditor")

add_library (VideoEditorLib 
                "src/window.cpp"
                "src/directx.cpp")

add_executable (VideoEditor 
                "src/entry.cpp")

target_include_directories(VideoEditorLib PRIVATE include)
target_include_directories(VideoEditor PRIVATE include)

# VideoEditorLib から ImGui を利用できるようにリンク
target_link_libraries(VideoEditorLib PUBLIC imgui)

target_link_libraries(VideoEditor PRIVATE VideoEditorLib)

#testの設定
add_executable(my_test test/test_main.cpp)

target_link_libraries(my_test VideoEditorLib)

add_test(NAME BasicTest COMMAND my_test)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VideoEditor PROPERTY CXX_STANDARD 20)
endif()