#pragma once

#include <string>
#include "../include/utils/Parser.hpp"
#include "../include/core/SpellChecker.hpp"
#include "../include/core/SuggestionBuffer.hpp"

namespace Tui {
/**
 * @brief Runs command-line interactive spell checking.
 *
 * @param checker Loaded spell checker.
 * @param maxEditDistance Maximum edit distance for corrections.
 * @param maxSuggestions Maximum suggestions shown per query.
 * @pre checker should contain dictionary words.
 * @post User can query until typing exit or quit.
 */
void runInteractive(SpellChecker& checker, int maxEditDistance, int maxSuggestions);

/**
 * @brief Runs batch checking for parsed input data.
 *
 * @param checker Loaded spell checker.
 * @param data Parsed dictionary/check-word input.
 * @pre data.checkWords points to data.checkCount words.
 * @post Batch results are written to standard output.
 */
void runBatch(SpellChecker& checker, const InputData& data);
}
