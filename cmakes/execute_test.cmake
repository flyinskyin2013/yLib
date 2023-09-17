message(WARNING "TESTS_ARGUMENTS=${TESTS_ARGUMENTS}")
execute_process(COMMAND 
    $ENV{QEMU_EXECUTABLE_LOADER} 
    $ENV{QEMU_EXECUTABLE_LOADER_ARGUMENTS} 
    ${TEST_EXECUTABLE} 
    ${TESTS_ARGUMENTS} ${TEST_ARG0} ${TEST_ARG1} ${TEST_ARG2} ${TEST_ARG3}
    RESULT_VARIABLE result)
if(NOT "${result}" STREQUAL "0")
    message(FATAL_ERROR "Test failed with return value '${result}'")
endif()  