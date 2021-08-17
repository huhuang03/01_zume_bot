find_package(easybot REQUIRED)


FILE(GLOB_RECURSE SRC *.h *.cc)
# can I use zuma_bot as a lib?
add_executable(zuma_bot ${SRC})

# cant find the include?
target_link_libraries(zuma_bot easybot)