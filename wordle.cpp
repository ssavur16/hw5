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
std::set<std::string> wordle(
    std::string floating,
    const std::set<std::string>& dict,
    std::string result,
    std::set<std::string>& possible,
    size_t len,
    int tries);

void rearrange_floating(
    const std::string& floating,
    const std::set<std::string>& dict,
    std::string result,
    std::set<std::string>& possible,
    int n_blanks,
    int j);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // initialize paramters for primary helper function
    string in_copy = in;
    set<string> poss;
    int tries = 0;
    for(size_t i = 0; i < in.size(); i++) {
        if(in[i] == '-') {
            tries++;
        }
    }
    int len = in.size() - tries;
    tries -= floating.size();

    // call primary helper function
    return wordle(floating, dict, in_copy, poss, len, tries);
}

// Define any helper functions here
// Definition of primary helper function
std::set<std::string> wordle(
    std::string floating,
    const std::set<std::string>& dict,
    std::string result,
    std::set<std::string>& possible,
    size_t len,
    int tries)
{
    // check if at end of word and if yes, check to decide to add to possible
    if(result.size() == len) {
        // check if viable word
        if(dict.find(result) != dict.end()) {
            possible.insert(result);
        }
    }

    // recursive call - generating all combinations within constraints
    else {
        if(tries > 0) {
            // only recurse over non-fixed location letters
            for(size_t i = 0; i < result.size(); i++) {
                if(result[i] == '-') {
                    for(int j = 0; j < 26; j++) {
                        char next_char = (char)('a'+j);
                        result[i] = next_char;

                        // separate calls depending on if 
                        // next_char is one of the floating (yellow) chars
                        if(floating.find(next_char) != string::npos) {
                            floating.erase(floating.find(next_char), 1);
                            wordle(floating, dict, result, possible, len+1, tries);
                            floating.push_back(next_char);
                        }
                        else { // not in floating
                            wordle(floating, dict, result, possible, len+1, tries-1);
                        }
                    }
                    result[i] = '-';
                }
            }
        }

        // tries == 0
        else {
          rearrange_floating(floating, dict, result, possible, floating.size(), 0);
        }
    }

    return possible;
}

// Definition of function to rearrange floating characters
void rearrange_floating(
    const std::string& floating,
    const std::set<std::string>& dict,
    std::string result,
    std::set<std::string>& possible,
    int n_blanks,
    int j)
{
  if(n_blanks == 0) {
    wordle(floating, dict, result, possible, result.size(), 0);
  }

  else {
    for(size_t i = 0; i < result.size(); i++) {
      if(result[i] == '-') {
        result[i] = floating[j];
        rearrange_floating(floating, dict, result, possible, n_blanks-1, j+1);
        result[i] = '-';
      }
    }
  }
}