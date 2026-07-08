add_executable(miransas_registry_tests
    registry_tests.cpp
)

target_link_libraries(miransas_registry_tests
    PRIVATE
        miransas::registry
)

target_compile_features(miransas_registry_tests PRIVATE cxx_std_23)

add_test(NAME registry_module_tests COMMAND miransas_registry_tests)

add_executable(miransas_feature_tests
    feature_tests.cpp
)

target_link_libraries(miransas_feature_tests
    PRIVATE
        miransas::core
)

target_compile_features(miransas_feature_tests PRIVATE cxx_std_23)

add_test(NAME feature_module_tests COMMAND miransas_feature_tests)
