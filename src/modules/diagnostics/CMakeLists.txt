add_library(miransas_diagnostics STATIC
    src/diagnostics_feature.cpp
)

add_library(miransas::diagnostics ALIAS miransas_diagnostics)

target_include_directories(miransas_diagnostics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_diagnostics
    PUBLIC
        miransas::core
)

target_compile_features(miransas_diagnostics PUBLIC cxx_std_23)
