add_library(miransas_restore STATIC
    src/restore_feature.cpp
)

add_library(miransas::restore ALIAS miransas_restore)

target_include_directories(miransas_restore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_restore
    PUBLIC
        miransas::core
)

target_compile_features(miransas_restore PUBLIC cxx_std_23)
