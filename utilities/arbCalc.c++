#include <iostream>
#include <string>
#include <vector>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
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

//    auto screen = ScreenInteractive::TerminalOutput();
//
//    std::vector<std::string> entries {"entry 1", "entry 2", "entry 3"};
//    int selected { 0 };
//    MenuOption option;
//    option.on_enter = screen.ExitLoopClosure();
//    auto menu = Menu(&entries, &selected, option);

    std::string formula;
    InputOption formulaInputOption;
    formulaInputOption.placeholder = "formula";
    auto formulaInput = Input(&formula, formulaInputOption);

    auto outputText = text("Output");
    auto outputRenderer = Renderer([&] { return outputText; });

    std::vector<std::string> previousFormulas {"a", "b"};
    int previousFormulasSelected = 0;
    MenuOption previousFormulasMenuOption;
    previousFormulasMenuOption.on_enter = [&] {
        auto* typedInputOption = dynamic_cast<InputOption*>(formulaInput.get());
        typedInputOption->content = previousFormulas[previousFormulasSelected];
    };
    auto previousFormulasMenu = Menu(&previousFormulas, &previousFormulasSelected, previousFormulasMenuOption);

    int formulaInputSize = 1;
    int inputOutputSize = formulaInputSize + 1 + 2;
    auto inputOutputSplit = ResizableSplitTop(formulaInput, outputRenderer, &formulaInputSize);
    auto previousSplit = ResizableSplitBottom(inputOutputSplit, previousFormulasMenu, &inputOutputSize);

    formulaInputOption.on_change = [&] {
        outputText = text(formula);
        std::cout << "FART!" << std::endl;
        exit(1);
    };
    formulaInput |= CatchEvent([&] (Event event) {
        exit(1);
        if (event == Event::Return) {
            return true;
        } else {
            return false;
        }
    });

    auto renderer = Renderer(previousSplit, [&] { return previousSplit->Render(); });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

    return 0;
}