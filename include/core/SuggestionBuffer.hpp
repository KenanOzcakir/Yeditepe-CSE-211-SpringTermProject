#pragma once

#include "../data_structures/LinkedList.hpp"
#include "Types.hpp"

/**
 * @brief Fixed-capacity sorted buffer for best candidates.
 *
 * Candidates are stored in a custom LinkedList and ordered by edit distance first,
 * then by frequency. The same structure is reused for autocomplete by assigning
 * distance 0 to completions.
 *
 * @pre capacity should be positive for the buffer to accept candidates.
 * @post The buffer keeps at most capacity candidates.
 */
class SuggestionBuffer {
   public:
    /**
     * @brief Constructs an empty candidate buffer.
     *
     * @param capacity Maximum number of candidates to keep.
     * @post size() returns 0.
     */
    SuggestionBuffer(int capacity);

    /**
     * @brief Releases the internal linked list.
     * @post All candidate nodes owned by the buffer are deleted.
     */
    ~SuggestionBuffer();

    /**
     * @brief Adds a candidate if it belongs in the top ranked results.
     *
     * @param cand Candidate to consider.
     * @pre cand.word should not be empty for a real result.
     * @post The buffer remains sorted and bounded by capacity.
     */
    void tryAdd(const Candidate& cand);

    /**
     * @brief Gets the number of stored candidates.
     *
     * @return Current number of candidates.
     * @post The buffer is not modified.
     */
    int size() const {
        return count;
    }

    /**
     * @brief Returns a candidate by rank.
     *
     * @param idx Zero-based rank.
     * @return Candidate at idx, or an empty candidate when idx is invalid.
     * @pre idx may be outside the valid range.
     * @post The buffer is not modified.
     */
    Candidate get(int idx) const;

   private:
    LinkedList<Candidate> list;
    int capacity;
    int count;

    static bool isBetter(const Candidate& a, const Candidate& b);

    void removeLast();
};
