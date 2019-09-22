# Adding sub directories to be included in the source.
add_subdirectory(tcp-server)
add_subdirectory(tcp-client)
add_subdirectory(raft-node)

# Add the executbale main.cpp as outputted as bin_main.
add_executable(
  bin_main 
  constants.h
  main.cpp
  network_client.h
  network_server.h
  server_event_listener.h
)

# Linking (binary, to a library)
target_link_libraries(
    bin_main 
    tcp-client
    tcp-server
    raft-node
)
