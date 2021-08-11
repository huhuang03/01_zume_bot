find_library(easybot REQUIRED)

add_executable(zuma_bot main.cc)

# cant find the include?
target_link_libraries(zuma_bot easybot)