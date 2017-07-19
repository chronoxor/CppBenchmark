# CMake generated Testfile for 
# Source directory: Z:/Projects/CppProjects/CppBenchmark
# Build directory: Z:/Projects/CppProjects/CppBenchmark/temp
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cppbenchmark-tests "cppbenchmark-tests" "--durations" "yes" "--order" "lex")
subdirs("modules")
