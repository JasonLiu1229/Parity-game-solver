#include <iostream>

#include "tools/tester.hh"
#include "tools/solver.hh"
#define INPUT_FILE "../HOA_files/ActionConverter.tlsf.ehoa"
#define TEST_DIR "../HOA_files"

int main()
{
    Parser parser;
    auto aut = parser.parse(INPUT_FILE);

    Solver solver(aut, parser.getControllableAPs(), parser.isMax(), parser.isEven(), parser.getNoPriorities());
    solver.solve();

    // aut = solver.get_automaton();

    // std::cout << aut->prop_state_acc() << std::endl;

    // Renderer renderer;
    // renderer.render(aut, INPUT_FILE, true);

    //aut = parser.getSbaccAutomaton();

    // Renderer renderer;
    // renderer.render(aut, INPUT_FILE, true);

    // Tester tester(TEST_DIR);
    // tester.run_render();
    // tester.clear_render();
}
