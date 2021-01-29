#ifndef AYUM_DLD_HPP
#define AYUM_DLD_HPP

#include <string>

namespace com::github::ayum::dld {

size_t damerau_levenshtein_distance(std::string p_string1,
                                    std::string p_string2);

}  // namespace com::github::ayum::dld

#endif  // AYUM_DLD_HPP
