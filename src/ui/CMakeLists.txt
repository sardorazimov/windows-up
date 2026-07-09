add_library(miransas_ui STATIC
    src/imgui_layer.cpp
    src/localization.cpp
)

add_library(miransas::ui ALIAS miransas_ui)

target_include_directories(miransas_ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_ui
    PUBLIC
        miransas::core
        miransas::logger
        miransas::privacy
        miransas::cleaner
        miransas::performance
        miransas::diagnostics
        miransas::restore
        miransas::security
        miransas::processes
        miransas::packages
        miransas::filesystem
)

target_compile_features(miransas_ui PUBLIC cxx_std_23)
