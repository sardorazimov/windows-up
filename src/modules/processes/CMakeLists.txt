add_library(miransas_processes STATIC
    src/processes_feature.cpp
)

add_library(miransas::processes ALIAS miransas_processes)

target_include_directories(miransas_processes
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_processes
    PUBLIC
        miransas::core
)

target_compile_features(miransas_processes PUBLIC cxx_std_23)
