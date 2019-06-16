# Adding sub directories to be included in the source.
add_subdirectory(tcp-server)
add_subdirectory(tcp-client)

# Add the executbale main.cpp as ouputted as bin_main.
add_executable(
  bin_main 
  main.cpp
  server.h
  client.h
)

# Linking (binary, to a library)
target_link_libraries(
    bin_main 
    tcp-server
    tcp-client
)

