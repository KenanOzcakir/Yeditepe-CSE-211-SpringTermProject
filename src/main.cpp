#include <iostream>
#include "../frontend/Tui.hpp"
#include "../include/utils/Parser.hpp"
#include "../include/core/SpellChecker.hpp"

#ifdef QT_WIDGETS_LIB
#include <QtWidgets/QApplication>
#include "../frontend/gui/SpellCheckerWidget.hpp"
#endif

int main(int argc, char** argv) {
    std::string inputPath = "data/input_sample.json";
    if (argc > 1) {
        inputPath = argv[1];
    }

    InputData data;
    if (!Parser::loadFromFile(inputPath, data)) {
        std::cerr << "Failed to load input file: " << inputPath << "\n";
        return 1;
    }

    SpellChecker checker;
    for (int i = 0; i < data.dictionaryCount; ++i) {
        checker.addDictionaryWord(data.dictionary[i].word, data.dictionary[i].frequency);
    }

    std::cout << "=== Batch Check ===\n";
    Tui::runBatch(checker, data);

#ifdef QT_WIDGETS_LIB
    QApplication app(argc, argv);

    SpellCheckerWidget widget;
    widget.setWindowTitle("Spell Checker");
    widget.resize(600, 160);
    widget.show();

    Parser::free(data);
    return app.exec();
#else
    std::cout << "\n=== Interactive ===\n";
    Tui::runInteractive(checker, data.maxEditDistance, data.maxSuggestions);

    Parser::free(data);
    return 0;
#endif
}
