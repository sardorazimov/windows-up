add_library(miransas_core STATIC
    src/feature_manager.cpp
    src/windows_privilege.cpp
)

add_library(miransas::core ALIAS miransas_core)

target_include_directories(miransas_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(miransas_core PUBLIC cxx_std_23)
