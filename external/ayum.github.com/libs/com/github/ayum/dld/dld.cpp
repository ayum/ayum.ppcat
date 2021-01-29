#include "dld.hpp"

#include <string>

namespace ayum = com::github::ayum;

/*
 * See
 * https://stackoverflow.com/questions/10727174/damerau-levenshtein-distance-edit-distance-with-transposition-c-implementation
 * for details
 */

size_t ayum::dld::damerau_levenshtein_distance(std::string p_string1,
                                               std::string p_string2) {
    int l_string_length1 = p_string1.length();
    int l_string_length2 = p_string2.length();
    std::size_t d[l_string_length1 + 1][l_string_length2 + 1];

    int i;
    int j;
    unsigned l_cost;

    for (i = 0; i <= l_string_length1; i++) {
        d[i][0] = i;
    }
    for (j = 0; j <= l_string_length2; j++) {
        d[0][j] = j;
    }
    for (i = 1; i <= l_string_length1; i++) {
        for (j = 1; j <= l_string_length2; j++) {
            if (p_string1[i - 1] == p_string2[j - 1]) {
                l_cost = 0;
            } else {
                l_cost = 1;
            }
            d[i][j] =
                std::min(d[i - 1][j] + 1,                    // delete
                         std::min(d[i][j - 1] + 1,           // insert
                                  d[i - 1][j - 1] + l_cost)  // substitution
                );
            if ((i > 1) && (j > 1) && (p_string1[i - 1] == p_string2[j - 2]) &&
                (p_string1[i - 2] == p_string2[j - 1])) {
                d[i][j] = std::min(d[i][j],
                                   d[i - 2][j - 2] + l_cost  // transposition
                );
            }
        }
    }
    return d[l_string_length1][l_string_length2];
}
