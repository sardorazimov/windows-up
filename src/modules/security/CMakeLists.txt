add_library(miransas_security STATIC
    src/security_feature.cpp
)

add_library(miransas::security ALIAS miransas_security)

target_include_directories(miransas_security
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_security
    PUBLIC
        miransas::core
)

target_compile_features(miransas_security PUBLIC cxx_std_23)
