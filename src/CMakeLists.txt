# Adding sub directories to be included in the source.
add_subdirectory(node)
add_subdirectory(server)

# Add the executbale main.cpp as ouputted as bin_main.
add_executable(
  bin_main 
  main.cpp
)

# Linking (binary, to a library)
target_link_libraries(
    bin_main 
    node
    server
)

