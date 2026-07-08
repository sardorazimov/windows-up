add_library(miransas_registry STATIC
    src/registry_manager.cpp
    src/registry_service.cpp
)

add_library(miransas::registry ALIAS miransas_registry)

target_include_directories(miransas_registry
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_registry
    PUBLIC
        miransas::core
)

target_compile_features(miransas_registry PUBLIC cxx_std_23)
