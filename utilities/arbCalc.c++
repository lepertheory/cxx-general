#include <iostream>
#include <string>
#include <vector>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

//#include "Arb.h++"

int main () {
    using namespace ftxui;

//    Element document = hbox({text("left")   | border,
//                             text("middle") | border | flex,
//                             text("right")  | border});
//
//    auto screen = Screen::Create(Dimension::Full(),
//                                 Dimension::Fit(document));
//    Render(screen, document);
//    screen.Print();

    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries {"entry 1", "entry 2", "entry 3"};
    int selected { 0 };
    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    screen.Loop(menu);
    std::cout << "Selected element: " << selected << std::endl;

    //return EXIT_SUCCESS;
    return 0;
}