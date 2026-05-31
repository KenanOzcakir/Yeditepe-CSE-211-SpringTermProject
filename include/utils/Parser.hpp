#pragma once

#include <string>
#include "../core/Types.hpp"

/**
 * @brief Parsed input data for dictionary loading and batch checking.
 *
 * @pre Parser::loadFromFile initializes the pointers before use.
 * @post Parser::free releases owned dynamic storage and resets the fields.
 */
struct InputData {
    WordEntry* dictionary;
    int dictionaryCount;
    std::string* checkWords;
    int checkCount;
    int maxEditDistance;
    int maxSuggestions;

    InputData()
        : dictionary(nullptr),
          dictionaryCount(0),
          checkWords(nullptr),
          checkCount(0),
          maxEditDistance(0),
          maxSuggestions(0) {}
};

/**
 * @brief Lightweight parser for the project's JSON-like data files.
 *
 * @pre Input files should follow the documented dictionary/check_words schema.
 * @post Parsed data is stored in InputData and must be released with free().
 */
class Parser {
   public:
    /**
     * @brief Loads all input sections from a file.
     *
     * @param path Path to the input file.
     * @param outData Destination data structure.
     * @return true when the file was opened and parsed.
     * @pre outData should not own unreleased memory.
     * @post On success, outData owns dictionary and check word storage.
     */
    static bool loadFromFile(const std::string& path, InputData& outData);

    /**
     * @brief Loads only the dictionary section from a project input file.
     *
     * @param path Path to the input file.
     * @param outDictionary Destination pointer that receives owned entries.
     * @param outCount Number of parsed dictionary entries.
     * @return true when the file was opened and scanned.
     * @post Caller owns outDictionary and must release it with delete[].
     */
    static bool loadDictionaryFromFile(const std::string& path, WordEntry*& outDictionary, int& outCount);

    /**
     * @brief Loads only the check_words section from a project input file.
     *
     * @param path Path to the input file.
     * @param outWords Destination pointer that receives owned words.
     * @param outCount Number of parsed check words.
     * @return true when the file was opened and scanned.
     * @post Caller owns outWords and must release it with delete[].
     */
    static bool loadCheckWordsFromFile(const std::string& path, std::string*& outWords, int& outCount);

    /**
     * @brief Releases memory owned by parsed input data.
     *
     * @param data Parsed data to clear.
     * @pre data was initialized by InputData or Parser::loadFromFile.
     * @post data pointers are nullptr and counts are zero.
     */
    static void free(InputData& data);

   private:
    /**
     * @brief Parses one dictionary object containing word and frequency fields.
     */
    static bool parseDictionaryLine(const std::string& line, WordEntry& entry);

    /**
     * @brief Parses one quoted check word from a line.
     */
    static bool parseCheckWord(const std::string& line, std::string& wordOut);
};
