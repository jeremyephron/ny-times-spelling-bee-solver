/*
 * File: spellingbeesolver.cpp
 * Author: Jeremy Ephron
 * ---------------------------
 * This is a program to solve the NY Times online Spelling Bee game.
 * (https://www.nytimes.com/puzzles/spelling-bee).
 *
 * The NY Times says they use the Oxford English Dictionary, for which I could
 * not find a text file with all recognized words. As such I am using a
 * Scrabble dictionary text file from
 * https://raw.githubusercontent.com/jonbcard/scrabble-bot/master/src/dictionary.txt
 *
 * This means that not all words generated will necessarily be recognized
 * as valid, but there certainly will be some valid words in the output.
 *
 * You can modify the dictionary used by editing "dictionary.txt" in the res
 * folder, or adding a dictionary of your own and specifying the dictionary
 * you would like to use.
 */

#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <iostream>

static const size_t kMinWordLength = 4;
static const std::string DEFAULT_DICT = "dictionary.txt";

/**
 * Function: getDictionaryNameFromUser
 * -----------------------------------
 * Gets a valid dictionary filename from the user.
 *
 * Uses the default dictionary filename if user input is empty, and reprompts
 * if file does not exist.
 *
 * @return the filename of the chosen dictionary.
 */
std::string getDictionaryNameFromUser();

/**
 * Function: getLettersFromUser
 * ----------------------------
 * Gets the letters constituting the spelling bee hive from the user.
 *
 * @param letters the set of letters to be filled in.
 * @return the middle character of the letter hive.
 */
char getLettersFromUser(std::unordered_set<char>& letters);

/**
 * Function: solveSpellingBee
 * --------------------------
 * Finds valid words to solve the spelling bee game.
 *
 * @param letters the set of letters constituting the spelling bee game.
 * @param middle the middle character in the spelling bee game.
 * @param dictionary the dictionary file stream.
 * @param result the vector to be filled in with valid words.
 * @return the number of words found.
 */
size_t solveSpellingBee(const std::unordered_set<char>& letters, char middle,
                        std::ifstream& dictionary,
                        std::vector<std::string>& result);

/**
 * Function: writeResultToFile
 * ------------------------------
 * Writes results to a file, specified by the user.
 *
 * @param filename the name of the output file.
 * @param results the valid words found in the spelling bee game.
 */
void writeResultToFile(const std::string &filename,
                       const std::vector<std::string> &result);

/**
 * Function: fileExists
 * --------------------
 * Checks whether a given file exists.
 *
 * @param filename the name of the file to check.
 * @return true if the file exists, false otherwise.
 */
bool fileExists(const std::string &filename);

/**
 * Function: isValidWord
 * ---------------------
 * Checks whether a word is valid for the spelling be game.
 *
 * Validity is comprised of the following:
 *     1) Length > 4
 *     2) Contains middle character
 *     3) All characters in word must be in letters
 *
 * @param word the word to check.
 * @param letters the set of letters constituting the spelling bee game.
 * @param middle the middle character of the spelling bee game.
 * @return true if the word is valid, false otherwise.
 */
bool isValidWord(const std::string& word,
                 const std::unordered_set<char>& letters, char middle);

int main() {
    std::string dictionaryFilename = getDictionaryNameFromUser();
    std::ifstream dictionary(dictionaryFilename);

    std::unordered_set<char> letters;
    char middle = getLettersFromUser(letters);

    std::vector<std::string> result;
    size_t nWordsFound = solveSpellingBee(letters, middle, dictionary, result);

    std::cout << nWordsFound << " words found! Would you like to "
                                 "save them to a file? (y/n): ";

    std::string line;
    getline(std::cin, line);
    if (!line.empty() && tolower(line[0]) == 'y') {
        std::cout << "Enter the output filename: ";
        getline(std::cin, line);
        writeResultToFile(line, result);
    } else {
        std::cout << "Would you like to print them out? (y/n): ";
        getline(std::cin, line);

        if (!line.empty() && tolower(line[0]) == 'y') {
            for (const std::string& word : result) {
                std::cout << word << std::endl;
            }
        } else {
            std::cout << "Alright, goodbye!" << std::endl;
        }
    }

    std::cout << "Have a nice day!" << std::endl;
    return 0;
}

std::string getDictionaryNameFromUser() {
    std::cout << "Enter the filename of the dictionary you want to use "
                 "(hit enter for \"dictionary.txt\"): ";

    std::string dictionaryFilename;
    std::getline(std::cin, dictionaryFilename);

    while (dictionaryFilename != "" && !fileExists(dictionaryFilename)) {
        std::cout << "File \"" << dictionaryFilename << "\" does not exist. "
                     "Please try again: ";

        std::getline(std::cin, dictionaryFilename);
    }

    return dictionaryFilename != "" ? dictionaryFilename : DEFAULT_DICT;
}

char getLettersFromUser(std::unordered_set<char>& letters) {
    std::cout << "Enter each letter starting with the middle letter: ";

    std::string input;
    std::getline(std::cin, input);

    for (size_t i = 0; i < input.length(); i++) {
        letters.insert(toupper(input[i]));
    }

    return toupper(input[0]);
}

size_t solveSpellingBee(const std::unordered_set<char>& letters, char middle,
                        std::ifstream& dictionary,
                        std::vector<std::string>& result) {
    std::string word;
    while (std::getline(dictionary, word)) {
        if (isValidWord(word, letters, middle)) {
            result.push_back(word);
        }
    }

    return result.size();
}

void writeResultToFile(const std::string &filename,
                          const std::vector<std::string> &result) {
    std::ofstream out(filename);

    for (const auto &word : result) {
        out << word << std::endl;
    }
}

bool fileExists(const std::string &filename) {
    return bool(std::ifstream(filename));
}

bool isValidWord(const std::string& word,
                 const std::unordered_set<char>& letters,char middle) {
    if (word.length() < kMinWordLength) return false;
    if (word.find(middle) == std::string::npos) return false;

    for (char ch : word) {
        if (letters.count(ch) == 0) return false;
    }

    return true;
}
