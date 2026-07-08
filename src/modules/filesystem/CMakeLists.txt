add_library(miransas_filesystem STATIC
    src/filesystem_feature.cpp
)

add_library(miransas::filesystem ALIAS miransas_filesystem)

target_include_directories(miransas_filesystem
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_filesystem
    PUBLIC
        miransas::core
)

target_compile_features(miransas_filesystem PUBLIC cxx_std_23)
