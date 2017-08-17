set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

find_library(Affdex_LIBRARY
        NAMES libaffdex-native.so
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../external/affdex/lib REQUIRED)

# Get the real path to the affdex data directory and pass to "AFFDEX_DATA"
get_filename_component(Affdex_DATADIR ${Affdex_LIBRARY} DIRECTORY)
get_filename_component(Affdex_DATADIR ${Affdex_DATADIR}/../data REALPATH)

message(STATUS "Adding Affdex library: " ${Affdex_LIBRARY})
message(STATUS "Setting Affdex data dir to " ${Affdex_DATADIR})
add_definitions(-DAFFDEX_DATA=\"${Affdex_DATADIR}\")

find_package(OpenCV REQUIRED)

find_package(Boost REQUIRED system)

list(APPEND analyzerLibs
    ${Affdex_LIBRARY})

list(APPEND analyzerIncludes
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/affdex/include)

add_executable(affdexCrash
        main.cpp
        ${analyzerClasses})

target_include_directories(affdexCrash
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${analyzerIncludes})

target_link_libraries(affdexCrash
        ${analyzerLibs}
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES})
