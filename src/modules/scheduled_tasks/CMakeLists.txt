add_library(miransas_scheduled_tasks STATIC
    src/scheduled_task_manager.cpp
)

add_library(miransas::scheduled_tasks ALIAS miransas_scheduled_tasks)

target_include_directories(miransas_scheduled_tasks
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(miransas_scheduled_tasks
    PUBLIC
        miransas::core
)

target_compile_features(miransas_scheduled_tasks PUBLIC cxx_std_23)
