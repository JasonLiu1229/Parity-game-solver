#include <iostream>

#include "tools/tester.hh"
#define TEST_DIR "../HOA_files"

int main()
{
    Tester tester(TEST_DIR);
    tester.test_solver_directory();
}
