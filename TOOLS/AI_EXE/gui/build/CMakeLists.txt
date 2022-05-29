

cmake_minimum_required(VERSION 3.10)

# set the project name
project(AI_EXE_GUI VERSION 1.0)

# add the executable
add_executable(AI_EXE_GUI ./main.cc)

set(AI_EXE_PROJ_GUI "${CMAKE_INSTALL_PREFIX}:$ENV{PATH}")


#relese only
#set(file_sh ${CMAKE_CURRENT_BINARY_DIR}/path.sh)
#set(path "${CMAKE_INSTALL_PREFIX}:$ENV{PATH}")
#file(WRITE ${file_sh} "#!/usr/bin/env bash\n")
#file(APPEND ${file_sh} "export PATH=\"${path}\"")
#execute_process(COMMAND chmod a+x ${file_sh} RESULT_VARIABLE res)