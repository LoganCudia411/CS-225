/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;
using std::sort; 

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    /* Your code goes here! */
    string newLine; 
    ifstream infile(filename); 
    if(infile.is_open()){
        while(getline(infile, newLine)){
            string s = newLine; 
            std::sort(s.begin(), s.end());
            dict[s].push_back(newLine); 
        }
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    /* Your code goes here! */
    for(auto currentS = words.begin(); currentS != words.end(); currentS++){
        string temp = *currentS; 
        sort(temp.begin(), temp.end());
        dict[temp].push_back(*currentS);
    }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    /* Your code goes here! */
    string s = word; 
    std::sort(s.begin(), s.end()); 
    if(dict.find(s) == dict.end()){
        return vector<string>(); 
    }
    if(dict.at(s).size() == 1){
        return vector<string>(); 
    }

    return dict.at(s); 
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    /* Your code goes here! */
    vector<vector<string>> anagram; 
    vector<string> s; 
    for(auto & curr : dict){
        s = get_anagrams(curr.first);
        if(s.size() >= 2){
            anagram.push_back(s); 
        }
    }
    return anagram;
}
