#include "../include/utils/Parser.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>

static std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        start++;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        end--;
    return s.substr(start, end - start);
}

static bool extractBetween(const std::string& line, char delimiter, std::string& out) {
    size_t first = line.find(delimiter);
    if (first == std::string::npos)
        return false;
    size_t second = line.find(delimiter, first + 1);
    if (second == std::string::npos)
        return false;
    out = line.substr(first + 1, second - first - 1);
    return true;
}

static void appendDictionaryEntry(WordEntry*& dict, int& dictCount, int& dictCap,
                                  const WordEntry& entry) {
    if (dictCount >= dictCap) {
        int newCap = dictCap * 2;
        WordEntry* bigger = new WordEntry[newCap];
        for (int i = 0; i < dictCount; ++i)
            bigger[i] = dict[i];
        delete[] dict;
        dict = bigger;
        dictCap = newCap;
    }
    dict[dictCount++] = entry;
}

static void appendCheckWord(std::string*& checks, int& checkCount, int& checkCap,
                            const std::string& word) {
    if (checkCount >= checkCap) {
        int newCap = checkCap * 2;
        std::string* bigger = new std::string[newCap];
        for (int i = 0; i < checkCount; ++i)
            bigger[i] = checks[i];
        delete[] checks;
        checks = bigger;
        checkCap = newCap;
    }
    checks[checkCount++] = word;
}

static bool parseDictionaryEntrySegment(const std::string& line, WordEntry& entry) {
    size_t wordKeyPos = line.find("\"word\"");
    if (wordKeyPos == std::string::npos)
        return false;

    size_t colonAfterWord = line.find(':', wordKeyPos);
    if (colonAfterWord == std::string::npos)
        return false;

    std::string word;
    size_t firstQuote = line.find('"', colonAfterWord);
    if (firstQuote == std::string::npos)
        return false;
    size_t secondQuote = line.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos)
        return false;
    word = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);

    size_t freqPos = line.find("frequency");
    if (freqPos == std::string::npos)
        return false;
    size_t colon = line.find(':', freqPos);
    if (colon == std::string::npos)
        return false;
    std::string numStr = trim(line.substr(colon + 1));
    if (!numStr.empty() && numStr.back() == ',')
        numStr.pop_back();
    int freq = std::atoi(numStr.c_str());

    entry.word = word;
    entry.frequency = freq;
    return true;
}

static void parseDictionaryEntriesFromLine(const std::string& line, WordEntry*& dict,
                                           int& dictCount, int& dictCap) {
    size_t pos = 0;
    while (true) {
        size_t wordPos = line.find("\"word\"", pos);
        if (wordPos == std::string::npos) {
            return;
        }

        size_t objectEnd = line.find('}', wordPos);
        std::string segment =
            objectEnd == std::string::npos ? line.substr(wordPos) : line.substr(wordPos, objectEnd - wordPos + 1);

        WordEntry entry;
        if (parseDictionaryEntrySegment(segment, entry)) {
            appendDictionaryEntry(dict, dictCount, dictCap, entry);
        }

        pos = objectEnd == std::string::npos ? wordPos + 6 : objectEnd + 1;
    }
}

static void parseCheckWordsFromLine(const std::string& line, std::string*& checks,
                                    int& checkCount, int& checkCap) {
    size_t pos = 0;
    while (true) {
        size_t firstQuote = line.find('"', pos);
        if (firstQuote == std::string::npos) {
            return;
        }
        size_t secondQuote = line.find('"', firstQuote + 1);
        if (secondQuote == std::string::npos) {
            return;
        }

        std::string token = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
        if (token != "check_words") {
            appendCheckWord(checks, checkCount, checkCap, token);
        }
        pos = secondQuote + 1;
    }
}

bool Parser::parseDictionaryLine(const std::string& line, WordEntry& entry) {
    return parseDictionaryEntrySegment(line, entry);
}

bool Parser::parseCheckWord(const std::string& line, std::string& wordOut) {
    std::string word;
    if (!extractBetween(line, '"', word))
        return false;
    wordOut = word;
    return true;
}

bool Parser::loadFromFile(const std::string& path, InputData& outData) {
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return false;

    const int initialDictCap = 128;
    const int initialCheckCap = 32;
    WordEntry* dict = new WordEntry[initialDictCap];
    int dictCap = initialDictCap;
    int dictCount = 0;

    std::string* checks = new std::string[initialCheckCap];
    int checkCap = initialCheckCap;
    int checkCount = 0;

    bool inDictionary = false;
    bool inCheckWords = false;

    std::string line;
    while (std::getline(file, line)) {
        std::string t = trim(line);
        size_t dictionaryKey = t.find("\"dictionary\"");
        size_t checkWordsKey = t.find("\"check_words\"");

        if (dictionaryKey != std::string::npos) {
            inDictionary = true;
            inCheckWords = false;
            parseDictionaryEntriesFromLine(t, dict, dictCount, dictCap);
            size_t close = t.find(']', dictionaryKey);
            if (close != std::string::npos) {
                inDictionary = false;
            }
        } else if (inDictionary) {
            parseDictionaryEntriesFromLine(t, dict, dictCount, dictCap);
            if (t.find(']') != std::string::npos) {
                inDictionary = false;
            }
        }

        if (checkWordsKey != std::string::npos) {
            inCheckWords = true;
            inDictionary = false;
            parseCheckWordsFromLine(t, checks, checkCount, checkCap);
            size_t close = t.find(']', checkWordsKey);
            if (close != std::string::npos) {
                inCheckWords = false;
            }
        } else if (inCheckWords) {
            parseCheckWordsFromLine(t, checks, checkCount, checkCap);
            if (t.find(']') != std::string::npos) {
                inCheckWords = false;
            }
        }

        if (t.find("max_edit_distance") != std::string::npos) {
            size_t colon = t.find(':');
            std::string num = trim(t.substr(colon + 1));
            outData.maxEditDistance = std::atoi(num.c_str());
        }
        if (t.find("max_suggestions") != std::string::npos) {
            size_t colon = t.find(':');
            std::string num = trim(t.substr(colon + 1));
            outData.maxSuggestions = std::atoi(num.c_str());
        }
    }

    outData.dictionary = dict;
    outData.dictionaryCount = dictCount;
    outData.checkWords = checks;
    outData.checkCount = checkCount;
    return true;
}

bool Parser::loadDictionaryFromFile(const std::string& path, WordEntry*& outDictionary, int& outCount) {
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return false;

    const int initialDictCap = 1024;
    WordEntry* dict = new WordEntry[initialDictCap];
    int dictCap = initialDictCap;
    int dictCount = 0;

    bool inDictionary = false;
    std::string line;
    while (std::getline(file, line)) {
        std::string t = trim(line);
        size_t dictionaryKey = t.find("\"dictionary\"");

        if (dictionaryKey != std::string::npos) {
            inDictionary = true;
            parseDictionaryEntriesFromLine(t, dict, dictCount, dictCap);
            if (t.find(']', dictionaryKey) != std::string::npos) {
                inDictionary = false;
            }
        } else if (inDictionary) {
            parseDictionaryEntriesFromLine(t, dict, dictCount, dictCap);
            if (t.find(']') != std::string::npos) {
                inDictionary = false;
            }
        }
    }

    outDictionary = dict;
    outCount = dictCount;
    return true;
}

bool Parser::loadCheckWordsFromFile(const std::string& path, std::string*& outWords, int& outCount) {
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return false;

    const int initialCheckCap = 128;
    std::string* checks = new std::string[initialCheckCap];
    int checkCap = initialCheckCap;
    int checkCount = 0;

    bool inCheckWords = false;
    std::string line;
    while (std::getline(file, line)) {
        std::string t = trim(line);
        size_t checkWordsKey = t.find("\"check_words\"");

        if (checkWordsKey != std::string::npos) {
            inCheckWords = true;
            parseCheckWordsFromLine(t, checks, checkCount, checkCap);
            if (t.find(']', checkWordsKey) != std::string::npos) {
                inCheckWords = false;
            }
        } else if (inCheckWords) {
            parseCheckWordsFromLine(t, checks, checkCount, checkCap);
            if (t.find(']') != std::string::npos) {
                inCheckWords = false;
            }
        }
    }

    outWords = checks;
    outCount = checkCount;
    return true;
}

void Parser::free(InputData& data) {
    delete[] data.dictionary;
    delete[] data.checkWords;
    data.dictionary = nullptr;
    data.checkWords = nullptr;
    data.dictionaryCount = 0;
    data.checkCount = 0;
}
