blt_add_executable(
  NAME exception_tests
  SOURCES exception_tests.cpp
  DEPENDS_ON umpire_util gtest
  OUTPUT_DIR ${UMPIRE_TEST_OUTPUT_DIR})

blt_add_test(
  NAME exception_tests
  COMMAND exception_tests)

blt_add_executable(
  NAME allocation_map_tests
  SOURCES allocation_map_tests.cpp
  DEPENDS_ON umpire_util gtest
  OUTPUT_DIR ${UMPIRE_TEST_OUTPUT_DIR})

blt_add_test(
  NAME allocation_map_tests
  COMMAND allocation_map_tests)