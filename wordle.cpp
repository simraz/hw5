#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
static void wordle_helper(size_t pos, std::string pattern, std::string remaining, const std::set<std::string>& dict, 
        size_t numBlanks, std::set<std::string>& results);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> results;
    int numBlanks = 0;
    for (size_t i = 0; i < in.size(); i++) {
        if (in[i] == '-') {
            numBlanks++;
        }
    }

    wordle_helper(0, in, floating, dict, numBlanks, results);
    return results;

}

// Define any helper functions here
static void wordle_helper(size_t pos, std::string pattern, std::string remaining,
                  const std::set<std::string>& dict, size_t numBlanks, std::set<std::string>& results) {

    // Base case: reached end of pattern
    if (pos == pattern.size()) {
        // All floating must be used
        if (!remaining.empty()) {
            return;
        }
        // Pattern completed - check if the word is in dict. Add to results.
        if (dict.find(pattern) != dict.end()) {
            results.insert(pattern);
        }
        return;
    }

    // Base case: Too many floating for remaining blanks
    if (remaining.size() > numBlanks) {
        return;
    }

    // Recursive: If current position is a fixed letter
    if (pattern[pos] != '-') {
        wordle_helper(pos + 1, pattern, remaining, dict, numBlanks, results);
        return;
    }


    // Recursive case: try each remaining floating letter for this blank
    for (size_t i = 0; i < remaining.size(); i++) {
        pattern[pos] = remaining[i];
        // remove this character from remaining
        std::string nextRemaining = remaining.substr(0, i) + remaining.substr(i + 1);
        wordle_helper(pos + 1, pattern, nextRemaining, dict, numBlanks - 1, results);
    }

    // Recursive case: try all letters a-z for this blank
    std::string alphabets_str = "abcdefghijklmnopqrstuvwxyz";
    for (size_t i = 0; i < alphabets_str.size(); i++) {
        if (remaining.find(alphabets_str[i]) == string::npos) {
            pattern[pos] = alphabets_str[i];
            wordle_helper(pos + 1, pattern, remaining, dict, numBlanks - 1, results);
        }
    }
}