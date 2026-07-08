add_library(miransas_logger STATIC
    src/logger.cpp
)

add_library(miransas::logger ALIAS miransas_logger)

target_include_directories(miransas_logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_logger
    PUBLIC
        miransas::core
)

target_compile_features(miransas_logger PUBLIC cxx_std_23)
