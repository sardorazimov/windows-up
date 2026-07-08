add_library(miransas_cleaner STATIC
    src/cleaner_feature.cpp
)

add_library(miransas::cleaner ALIAS miransas_cleaner)

target_include_directories(miransas_cleaner
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_cleaner
    PUBLIC
        miransas::core
)

target_compile_features(miransas_cleaner PUBLIC cxx_std_23)
