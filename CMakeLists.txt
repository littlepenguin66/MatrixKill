cmake_minimum_required(VERSION 3.10)
project(MatrixKill VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
set(SOURCES
    src/main.cpp
    src/core/solver.cpp
    src/core/config_reader.cpp
    src/solvers/jacobi_solver.cpp
    src/solvers/gauss_solver.cpp
    src/solvers/sor_solver.cpp
)

# 头文件
set(HEADERS
    include/core/solver.h
    include/core/config_reader.h
    include/solvers/jacobi_solver.h
    include/solvers/gauss_solver.h
    include/solvers/sor_solver.h
    include/utils/timer.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 设置包含目录
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

# 复制配置文件
configure_file(${PROJECT_SOURCE_DIR}/config.ini
               ${PROJECT_BINARY_DIR}/config.ini COPYONLY)

# 创建结果文件目录
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR})

# 编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif() 