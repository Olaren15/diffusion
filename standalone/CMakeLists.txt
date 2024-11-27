set(APP_NAME ${PROJECT_NAME}-standalone)

add_executable(${APP_NAME} src/main.c)
target_link_libraries(${APP_NAME} PUBLIC ${PROJECT_NAME})
set_compile_options(${APP_NAME})