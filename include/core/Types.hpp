#pragma once

#include <string>

/**
 * @brief Ranked correction or autocomplete candidate.
 *
 * @pre word may be empty only for sentinel/default values.
 * @post The candidate stores the word text, edit distance, and frequency score.
 */
struct Candidate {
    std::string word;
    int distance;
    int frequency;
};

/**
 * @brief Dictionary entry loaded from an input data file.
 *
 * @pre frequency should be non-negative.
 * @post The entry can be inserted into the spell checker dictionary.
 */
struct WordEntry {
    std::string word;
    int frequency;
};
