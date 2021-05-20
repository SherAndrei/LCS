link_libraries(LCS)

foreach(test integral_tests floating_tests)
    add_executable(${test}.out ${test}.cpp)    
endforeach(test)

