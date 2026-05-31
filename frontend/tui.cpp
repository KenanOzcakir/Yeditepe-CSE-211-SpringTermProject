#include "Tui.hpp"
#include <iostream>

namespace {
void printSuggestions(SuggestionBuffer& buffer) {
    for (int i = 0; i < buffer.size(); ++i) {
        Candidate c = buffer.get(i);
        std::cout << "  " << c.word << " (dist=" << c.distance << ", freq=" << c.frequency << ")\n";
    }
}
}

namespace Tui {

void runInteractive(SpellChecker& checker, int maxEditDistance, int maxSuggestions) {
    std::cout << "Spell Checker Interactive Mode (type 'exit' to quit)\n";
    std::string word;
    while (true) {
        std::cout << "Enter word: ";
        std::getline(std::cin, word);
        if (word == "exit" || word == "quit") break;
        if (word.empty()) {
            continue;
        }
        if (checker.isWordCorrect(word)) {
            std::cout << "'" << word << "' is correct.\n";
            continue;
        }
        SuggestionBuffer buffer(maxSuggestions);
        int count = checker.suggest(word, maxEditDistance, maxSuggestions, buffer);
        if (count == 0) {
            std::cout << "No suggestions found.\n";
        } else {
            std::cout << "Suggestions:\n";
            printSuggestions(buffer);
        }
    }
}

void runBatch(SpellChecker& checker, const InputData& data) {
    for (int i = 0; i < data.checkCount; ++i) {
        const std::string& w = data.checkWords[i];
        if (checker.isWordCorrect(w)) {
            std::cout << w << " is correct.\n";
        } else {
            SuggestionBuffer buffer(data.maxSuggestions);
            int count = checker.suggest(w, data.maxEditDistance, data.maxSuggestions, buffer);
            std::cout << w << " -> ";
            if (count == 0) {
                std::cout << "no suggestions\n";
            } else {
                for (int j = 0; j < count; ++j) {
                    Candidate c = buffer.get(j);
                    std::cout << c.word;
                    if (j != count - 1) std::cout << ", ";
                }
                std::cout << "\n";
            }
        }
    }
}

}


