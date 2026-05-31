#include "../include/core/EditDistance.hpp"
#include <cstdlib>
#include <cstring>

int EditDistance::computeBounded(const std::string& a, const std::string& b, int maxDist) {
    const int lenA = static_cast<int>(a.size());
    const int lenB = static_cast<int>(b.size());

    if (abs(lenA - lenB) > maxDist)
        return maxDist + 1;
    if (lenA == 0)
        return lenB <= maxDist ? lenB : maxDist + 1;
    if (lenB == 0)
        return lenA <= maxDist ? lenA : maxDist + 1;

    int* prev = new int[lenB + 1];
    int* curr = new int[lenB + 1];

    for (int j = 0; j <= lenB; ++j)
        prev[j] = j;

    for (int i = 1; i <= lenA; ++i) {
        curr[0] = i;
        int rowMin = curr[0];
        for (int j = 1; j <= lenB; ++j) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            int deletion = prev[j] + 1;
            int insertion = curr[j - 1] + 1;
            int substitution = prev[j - 1] + cost;
            int val = deletion;
            if (insertion < val)
                val = insertion;
            if (substitution < val)
                val = substitution;
            curr[j] = val;
            if (curr[j] < rowMin)
                rowMin = curr[j];
        }
        if (rowMin > maxDist) {
            delete[] prev;
            delete[] curr;
            return maxDist + 1;
        }
        int* tmp = prev;
        prev = curr;
        curr = tmp;
    }

    int result = prev[lenB];
    delete[] prev;
    delete[] curr;
    return result;
}
