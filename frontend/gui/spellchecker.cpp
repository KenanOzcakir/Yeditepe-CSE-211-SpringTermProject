#include "spellchecker.h"
#include <fstream>

#include "../../include/core/SpellChecker.hpp"
#include "../../include/utils/Parser.hpp"

namespace {
SpellChecker gChecker;
int gMaxEdit = 2;
int gMaxSuggestions = 3;
bool gInitialized = false;

std::string findDictionaryPath() {
    const std::string candidates[] = {
        "data/input_large.json",
        "../data/input_large.json",
        "../../data/input_large.json",
        "../../../data/input_large.json",
    };
    const int candidateCount = static_cast<int>(sizeof(candidates) / sizeof(candidates[0]));

    for (int i = 0; i < candidateCount; ++i) {
        std::ifstream file(candidates[i].c_str());
        if (file.is_open()) {
            return candidates[i];
        }
    }
    return {};
}

bool ensureInitialized() {
    if (gInitialized) return true;

    InputData data;
    const std::string dictPath = findDictionaryPath();

    if (!dictPath.empty() && Parser::loadFromFile(dictPath, data)) {
        gMaxEdit = data.maxEditDistance;
        gMaxSuggestions = data.maxSuggestions;
        for (int i = 0; i < data.dictionaryCount; ++i) {
            gChecker.addDictionaryWord(data.dictionary[i].word, data.dictionary[i].frequency);
        }
        Parser::free(data);
        gInitialized = true;
        return true;
    }

    gChecker.addDictionaryWord("hello", 30000);
    gChecker.addDictionaryWord("help", 25000);
    gChecker.addDictionaryWord("world", 40000);
    gChecker.addDictionaryWord("their", 50000);
    gChecker.addDictionaryWord("there", 45000);
    gChecker.addDictionaryWord("they", 60000);
    gInitialized = true;
    return true;
}
}

bool isWordCorrect(const std::string& word) {
    ensureInitialized();
    return gChecker.isWordCorrect(word);
}

int getSuggestions(const std::string& word, std::string results[], int capacity) {
    ensureInitialized();
    int limit = capacity > 0 ? capacity : gMaxSuggestions;

    SuggestionBuffer buffer(limit);
    gChecker.suggest(word, gMaxEdit, limit, buffer);
    int count = buffer.size();
    for (int i = 0; i < count && i < capacity; ++i) {
        results[i] = buffer.get(i).word;
    }
    return count;
}

int getCompletions(const std::string& prefix, std::string results[], int capacity) {
    ensureInitialized();
    int limit = capacity > 0 ? capacity : gMaxSuggestions;

    SuggestionBuffer buffer(limit);
    gChecker.autocomplete(prefix, limit, buffer);
    int count = buffer.size();
    for (int i = 0; i < count && i < capacity; ++i) {
        results[i] = buffer.get(i).word;
    }
    return count;
}
