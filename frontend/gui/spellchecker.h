#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <string>

int getSuggestions(const std::string& word, std::string results[], int capacity);
int getCompletions(const std::string& prefix, std::string results[], int capacity);
bool isWordCorrect(const std::string& word);

#endif
