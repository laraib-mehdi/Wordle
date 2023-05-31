#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int LISTSIZE = 1000;
const int EXACT = 2;
const int CLOSE = 1;
const int WRONG = 0;

const string GREEN = "\033[38;2;255;255;255;1m\033[48;2;106;170;100;1m";
const string YELLOW = "\033[38;2;255;255;255;1m\033[48;2;201;180;88;1m";
const string RED = "\033[38;2;255;255;255;1m\033[48;2;220;20;60;1m";
const string RESET = "\033[0;39m";

string get_guess(int wordsize);
int check_word(const string& guess, int wordsize, vector<int>& status, const string& choice);
void print_word(const string& guess, int wordsize, const vector<int>& status);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./wordle wordsize" << endl;
        return 1;
    }

    int wordsize = atoi(argv[1]);

    if (wordsize < 5 || wordsize > 8) {
        cout << "Error: wordsize must be either 5, 6, 7, or 8" << endl;
        return 1;
    }

    string wl_filename = to_string(wordsize) + ".txt";
    ifstream wordlist(wl_filename);

    if (!wordlist.is_open()) {
        cout << "Error opening file " << wl_filename << endl;
        return 1;
    }

    vector<string> options;
    string word;

    while (wordlist >> word) {
        options.push_back(word);
    }

    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    int guesses = wordsize + 1;
    bool won = false;

    cout << GREEN << "This is WORDLE50" << RESET << endl;
    cout << "You have " << guesses << " tries to guess the " << wordsize << "-letter word I'm thinking of" << endl;

    for (int i = 0; i < guesses; i++) {
        string guess = get_guess(wordsize);

        vector<int> status(wordsize, WRONG);

        int score = check_word(guess, wordsize, status, choice);

        cout << "Guess " << i + 1 << ": ";
        print_word(guess, wordsize, status);

        if (score == EXACT * wordsize) {
            won = true;
            break;
        }
    }

    if (won) {
        cout << "You Won!" << endl;
    } else {
        cout << "The word was " << choice << "." << endl;
        cout << "Better luck next time!" << endl;
    }

    return 0;
}

string get_guess(int wordsize) {
    string guess;

    do {
        cout << "Input a " << wordsize << "-letter word: ";
        cin >> guess;
    } while (guess.length() != wordsize);

    return guess;
}

int check_word(const string& guess, int wordsize, vector<int>& status, const string& choice) {
    int score = 0;

    for (int i = 0; i < wordsize; i++) {
        if (guess[i] == choice[i]) {
            status[i] = EXACT;
            score += EXACT;
        } else {
            for (int j = 0; j < wordsize; j++) {
                if (guess[i] == choice[j]) {
                    status[i] = CLOSE;
                    score += CLOSE;
                    break;
                }
            }
        }
    }

    return score;
}

void print_word(const string& guess, int wordsize, const vector<int>& status) {
    for (int i = 0; i < wordsize; i++) {
        if (status[i] == EXACT) {
            cout << GREEN << guess[i] << RESET;
        } else if (status[i] == CLOSE) {
            cout << YELLOW << guess[i] << RESET;
        } else {
            cout << RED << guess[i] << RESET;
        }
    }
    cout << endl;
}