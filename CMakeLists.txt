cmake_minimum_required (VERSION 3.8)

enable_testing()

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.x
)

FetchContent_MakeAvailable(spdlog)

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

target_link_libraries(VideoEditor PRIVATE VideoEditorLib)
#testの設定
add_executable(my_test test/test_main.cpp)

target_link_libraries(my_test VideoEditorLib)

add_test(NAME BasicTest COMMAND my_test)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VideoEditor PROPERTY CXX_STANDARD 20)
endif()