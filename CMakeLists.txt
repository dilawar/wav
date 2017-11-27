cmake_minimum_required(VERSION 2.6)
project(wavparser)

set(CMAKE_BUILD_TYPE Release )

add_definitions( -std=c++11 )
add_executable(test_wav test_wav.cpp)

enable_testing( )
file(GLOB_RECURSE WAV_FILES ${CMAKE_SOURCE_DIR}/test *.wav)
foreach( _wav ${WAV_FILES} )
    get_filename_component( _test  ${_wav} NAME_WE )
    message(STATUS "Adding test ${_test}" )
    add_test( NAME test_${_test} 
        COMMAND ./test_wav ${_wav} ${_test}.csv
        )
endforeach( )
