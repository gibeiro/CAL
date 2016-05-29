#include <iostream>
#include <vector>
#include <string>

#ifndef _STRINGS_H_
#define _STRINGS_H_

/**
 * Knuth-Morris-Pratt exact string search algorithm.
 * @param S
 * @param K
 * @return Returns a vector containing the zero based index of the start of each match of the string K in S.
 */
std::vector<int> KMP(std::string S, std::string K);

/**
 * Levenshtein string distance algorithm.
 * @param s1
 * @param s2
 * @return Returns the minimum amount of deletions, insertions and substitutions needed to transform s1 into s2.
 */
int levenshtein_distance(const std::string& s1, const std::string& s2) ;

#endif /*_STRINGS_H_*/
