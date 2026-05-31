#pragma once

#include <string>

/**
 * @brief Utility class for bounded Levenshtein distance.
 *
 * @pre Input strings may be empty.
 * @post The class does not store state.
 */
class EditDistance {
   public:
    /**
     * @brief Computes Levenshtein distance with early termination.
     *
     * @param a Source string.
     * @param b Target string.
     * @param maxDist Maximum distance of interest.
     * @return Exact distance when it is <= maxDist; otherwise maxDist + 1.
     * @pre maxDist must be non-negative.
     * @post No input string is modified.
     */
    static int computeBounded(const std::string& a, const std::string& b, int maxDist);
};
