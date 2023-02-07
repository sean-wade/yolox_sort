cmake_minimum_required(VERSION 3.10)
project(yolox_forzc VERSION 0.0.1)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CUDA_LIB_PATH /usr/local/cuda/lib64/)


# find CUDA
find_package(CUDA)
message("CUDA Libs: ${CUDA_LIBRARIES}")
message("CUDA Headers: ${CUDA_INCLUDE_DIRS}")


# find tensorRT
find_library(NVINFER NAMES nvinfer)
find_library(NVPARSERS NAMES nvparsers)
find_library(NVONNXPARSERS NAMES nvonnxparser)
message("NVINFER: ${NVINFER}")
message("NVPARSERS: ${NVPARSERS}")
message("NVONNXPARSERS: ${NVONNXPARSERS}")


# find CUDNN
find_library(CUDNN_LIBRARY
    NAMES libcudnn.so${__cudnn_ver_suffix} libcudnn${__cudnn_ver_suffix}.dylib ${__cudnn_lib_win_name}
    PATHS $ENV{LD_LIBRARY_PATH} ${__libpath_cudart} ${CUDNN_ROOT_DIR} ${PC_CUDNN_LIBRARY_DIRS} ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib lib64 bin
    DOC "CUDNN library."
)
message("CUDNN_LIBRARY: ${CUDNN_LIBRARY}")


# find OpenCV
find_package(OpenCV)
message("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message("OpenCV_LIBS: ${OpenCV_LIBS}")


## head files include 
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/lib
    ${PROJECT_SOURCE_DIR}/src/types
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/src/base
    ${PROJECT_SOURCE_DIR}/src/lib/detector
    ${PROJECT_SOURCE_DIR}/src/lib/tracker
    ${OpenCV_INCLUDE_DIRS}
    ${CUDA_INCLUDE_DIRS}
)


#############################################
################# Test Sample ###############

file(GLOB_RECURSE API_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
message("API_SOURCES: ${API_SOURCES}")

# add_executable(test_detector ${PROJECT_SOURCE_DIR}/test/test_detector.cpp ${API_SOURCES})
# target_link_libraries(test_detector ${OpenCV_LIBS})
# target_link_libraries(test_detector nvinfer nvonnxparser ${PROJECT_SOURCE_DIR}/lib/libyolov5_trt.so)
# target_link_libraries(test_detector ${CUDA_LIBRARIES})

# add_executable(test_tracker ${PROJECT_SOURCE_DIR}/test/test_tracker.cpp ${API_SOURCES})
# target_link_libraries(test_tracker ${OpenCV_LIBS})
# target_link_libraries(test_tracker nvinfer nvonnxparser)
# target_link_libraries(test_tracker ${CUDA_LIBRARIES})
# # target_link_libraries(test_tracker ${PROJECT_SOURCE_DIR}/lib/libyolov5_trt.so)

add_executable(test_processer ${PROJECT_SOURCE_DIR}/test/test_pipeline.cpp ${API_SOURCES})
target_link_libraries(test_processer ${OpenCV_LIBS})
target_link_libraries(test_processer nvinfer nvonnxparser)
target_link_libraries(test_processer ${CUDA_LIBRARIES})
# target_link_libraries(test_processer ${PROJECT_SOURCE_DIR}/lib/libyolov5_trt.so)



#############################################


