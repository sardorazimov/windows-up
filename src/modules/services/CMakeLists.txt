add_library(miransas_services STATIC
    src/service_manager.cpp
)

add_library(miransas::services ALIAS miransas_services)

target_include_directories(miransas_services
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_services
    PUBLIC
        miransas::core
)

target_compile_features(miransas_services PUBLIC cxx_std_23)
