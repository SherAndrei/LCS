
link_libraries(LCS)
include_directories(${PROJECT_SOURCE_DIR}/utils)

foreach(test integral_tests floating_tests)
    add_executable(${test}.out ${test}.cpp)    
endforeach(test)

