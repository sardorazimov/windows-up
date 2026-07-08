add_library(miransas_packages STATIC
    src/packages_feature.cpp
)

add_library(miransas::packages ALIAS miransas_packages)

target_include_directories(miransas_packages
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_packages
    PUBLIC
        miransas::core
)

target_compile_features(miransas_packages PUBLIC cxx_std_23)
