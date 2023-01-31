project(faiss_clustering)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -Wall -pedantic -Wextra -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -flto -ftree-vectorize -march=native")


add_executable(faiss_clustering main.cpp)
target_link_libraries(faiss_clustering PRIVATE faiss blas)

