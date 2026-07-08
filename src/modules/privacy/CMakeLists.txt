add_library(miransas_privacy STATIC
    src/privacy_feature.cpp
)

add_library(miransas::privacy ALIAS miransas_privacy)

target_include_directories(miransas_privacy
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_privacy
    PUBLIC
        miransas::core
)

target_compile_features(miransas_privacy PUBLIC cxx_std_23)
