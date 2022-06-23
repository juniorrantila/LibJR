add_executable(run-tests
    run_tests.cpp
    ../Cli/ArgumentParser.cpp
    ../Core/Folder.cpp
    ../Core/System.cpp
    ../JR/String.cpp
    ../JR/StringView.cpp
    )

file(GLOB SOURCES JR/*.cpp)
foreach(file ${SOURCES})
    get_filename_component(filename ${file} NAME)
    string(REPLACE ".cpp" "" output_name ${filename})
    string(PREPEND output_name "LibJR-")
    add_library(${output_name} SHARED ${file})
    target_link_libraries(${output_name} LibJR)
endforeach(file ${SOURCES})

file(GLOB SOURCES Core/*.cpp)
foreach(file ${SOURCES})
    get_filename_component(filename ${file} NAME)
    string(REPLACE ".cpp" "" output_name ${filename})
    string(PREPEND output_name "LibCore-")
    add_library(${output_name} SHARED ${file})
    target_link_libraries(${output_name} LibCore LibJR)
endforeach(file ${SOURCES})

file(GLOB SOURCES Cli/*.cpp)
foreach(file ${SOURCES})
    get_filename_component(filename ${file} NAME)
    string(REPLACE ".cpp" "" output_name ${filename})
    string(PREPEND output_name "LibCli-")
    add_library(${output_name} SHARED ${file})
    target_link_libraries(${output_name} LibCli LibJR)
endforeach(file ${SOURCES})
