#include <iostream>

#include "tools/tester.hh"

#define INPUT_FILE "../HOA_files/TwoCountersRefinedRefined.tlsf.ehoa"
#define TEST_DIR "../HOA_files"

int main()
{
    Parser parser;
    auto aut = parser.parse(INPUT_FILE);
    std::vector<int> controllable_aps = parser.getControllableAPs();
    for (auto &ap : controllable_aps) {
        std::cout << ap << std::endl;
    }

    //aut = parser.getSbaccAutomaton();

    // Renderer renderer;
    // renderer.render(aut, INPUT_FILE, true);

    // Tester tester(TEST_DIR);
    // tester.run_render();
    // tester.clear_render();
}
