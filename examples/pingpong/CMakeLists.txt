add_executable(pingpong_server server.cc)
target_link_libraries(pingpong_server muduo_net)

add_executable(pingpong_client client.cc)
target_link_libraries(pingpong_client muduo_net)
