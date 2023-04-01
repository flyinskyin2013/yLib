execute_process(COMMAND $ENV{TESTS_EXECUTABLE_LOADER} $ENV{TESTS_EXECUTABLE_LOADER_ARGUMENTS} ${TEST_EXECUTABLE} ${TESTS_ARGUMENTS} RESULT_VARIABLE result)
if(NOT "${result}" STREQUAL "0")
    message(FATAL_ERROR "Test failed with return value '${result}'")
endif()