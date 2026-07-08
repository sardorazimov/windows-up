add_library(miransas_performance STATIC
    src/performance_feature.cpp
)

add_library(miransas::performance ALIAS miransas_performance)

target_include_directories(miransas_performance
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_performance
    PUBLIC
        miransas::core
)

target_compile_features(miransas_performance PUBLIC cxx_std_23)
