#include "../include/core/SuggestionBuffer.hpp"
#include <cstddef>

SuggestionBuffer::SuggestionBuffer(int cap) : list(), capacity(cap), count(0) {
}

SuggestionBuffer::~SuggestionBuffer() {
}

bool SuggestionBuffer::isBetter(const Candidate& a, const Candidate& b) {
    if (a.distance < b.distance) {
        return true;
    }
    if (a.distance > b.distance) {
        return false;
    }
    return a.frequency > b.frequency;
}

Candidate SuggestionBuffer::get(int idx) const {
    if (idx < 0 || idx >= count) {
        Candidate empty;
        empty.distance = 0;
        empty.frequency = 0;
        empty.word = "";
        return empty;
    }

    ListNode<Candidate>* cur = list.getHead();
    for (int i = 0; i < idx && cur; ++i) {
        cur = cur->next;
    }

    if (cur) {
        return cur->value;
    }

    Candidate empty;
    empty.distance = 0;
    empty.frequency = 0;
    empty.word = "";
    return empty;
}

void SuggestionBuffer::removeLast() {
    list.removeLast();
    --count;
}

void SuggestionBuffer::tryAdd(const Candidate& cand) {
    if (capacity <= 0) {
        return;
    }

    if (count < capacity) {
        list.insertSorted(cand, isBetter);
        ++count;
    } else {
        Candidate worst = get(count - 1);
        if (isBetter(cand, worst)) {
            removeLast();
            list.insertSorted(cand, isBetter);
            ++count;
        }
    }
}
