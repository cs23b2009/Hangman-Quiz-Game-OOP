#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <fstream>
#include <stdexcept>

using namespace std;

//----Function Declarations----
void displayMenu();
void displayInstructions();
void displayWelcomeArt();
void displayWinArt();
void displayLoseArt();

//----Encapsulation (Base Class for Games)----
class BaseGame {
protected:
    static int totalGamesPlayed;
    static int totalWins;

public:
    virtual void play() = 0; // Pure virtual function for playing the game
    static void displayStats() {
        cout << "\nTotal Games Played: " << totalGamesPlayed << endl;
        cout << "Total Wins: " << totalWins << endl;
    }

    virtual ~BaseGame() {}
};

int BaseGame::totalGamesPlayed = 0;
int BaseGame::totalWins = 0;

//----Encapsulation (Question Class)----
class Question {
private:
    string prompt;
    string answer;

public:
    Question(const string& p, const string& a) : prompt(p), answer(a) {}
    string getPrompt() const { return prompt; }
    string getAnswer() const { return answer; }

    // Overload the << operator
    friend ostream& operator<<(ostream& os, const Question& q) {
        os << "Question: " << q.prompt;
        return os;
    }
};


//----Templates (QuestionManager Class)----
template <typename T>
class QuestionManager {
private:
    vector<T> questions;

public:
    void addQuestion(const T& question) {
        questions.push_back(question);
    }

    T getRandomQuestion() {
        if (questions.empty()) {
            throw runtime_error("No questions available.");
        }
        srand(static_cast<unsigned int>(time(0)));
        int index = rand() % questions.size();
        return questions[index];
    }

    size_t size() const {
        return questions.size();
    }
};

//----Static Members and Utility Functions----
class Utility {
public:
    static void showGameRules();
};

void Utility::showGameRules() {
   cout << "\nHow to Play Hangman:\n";
    cout << "1. You will be given a question or a hint related to a word.\n";
    cout << "2. Type one letter at a time to guess the word.\n";
    cout << "3. If the letter is correct, it will be revealed in the word.\n";
    cout << "4. If you guess all letters correctly, you win!\n";
    cout << "5. You lose if you make 6 incorrect guesses.\n";
    cout << "6. You can type '!' to use a hint if you are stuck (limited to 2 hints).\n\n";
}

//----Namespace Usage (HangmanGame Namespace)----
namespace HangmanGame {
//----Inheritance and Polymorphism (Hangman Class)----
    class Hangman : public BaseGame {
    private:
        QuestionManager<Question> questionManager;
        string guess_word;
        string user_guess;
        char guess;
        int correct_guess;
        int incorrect_guess;
        int word_length;
        int hints_used;
        int remainingHints;

        void selectRandomQuestion();
        bool checkGuess();

    public:
        Hangman();
        ~Hangman();

        void play() override;
        void provideHint();
        void displayLevel(int level) const;
        void loadQuestionsFromFile(const string& filename);
    };

    Hangman::Hangman()
        : correct_guess(0), incorrect_guess(0), word_length(0), hints_used(0), remainingHints(2) {
        questionManager.addQuestion(Question("What is the ability of different classes to respond to the same method in different ways?", "polymorphism"));
        questionManager.addQuestion(Question("What is the process of bundling data and methods within a class?", "encapsulation"));
        questionManager.addQuestion(Question("What is the technique of hiding internal details and showing only necessary features?", "abstraction"));
        questionManager.addQuestion(Question("What is the concept of one class inheriting properties and methods from another?", "inheritance"));
        questionManager.addQuestion(Question("What is the concept where an object can take many forms based on its context?", "polymorphism"));
        questionManager.addQuestion(Question("What is the concept where a subclass provides its own implementation of a method?", "overriding"));
        questionManager.addQuestion(Question("What is the type of class that cannot be instantiated directly but serves as a blueprint?", "abstractclass"));
        questionManager.addQuestion(Question("What is the concept where an object’s internal state is hidden from outside access?", "encapsulation"));
        questionManager.addQuestion(Question("What is the concept where a class can inherit from multiple classes?", "multipleinheritance"));
        questionManager.addQuestion(Question("What is the feature that allows code to be organized into separate modules?", "modularity"));
    }

    Hangman::~Hangman() { cout << "\nGame Finished. Thanks for playing!\n"; }

    void Hangman::selectRandomQuestion() {
    try {
        auto question = questionManager.getRandomQuestion();
        guess_word = question.getAnswer();
        word_length = guess_word.size();
        user_guess = string(word_length, '_');
        correct_guess = word_length;

        cout << question << endl; // Use the overloaded << operator
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        throw;
    }
}


    bool Hangman::checkGuess() {
        bool flag = false;
        for (int i = 0; i < word_length; ++i) {
            if (guess == guess_word[i] && user_guess[i] == '_') {
                user_guess[i] = guess;
                flag = true;
                correct_guess--;
            }
        }
        return flag;
    }

void Hangman::displayLevel(int level) const {
//----Function Overloading (Display Hangman Levels)----
        cout << "Hangman Level: " << level << endl;
        switch (level) {
            case 0:
                cout << R"(
                   -----
                   |   |
                   |
                   |
                   |
                   |
                ---------)";
                break;
            case 1:
                cout << R"(
                   -----
                   |   |
                   |   O
                   |
                   |
                   |
                ---------)";
                break;
            case 2:
                cout << R"(
                   -----
                   |   |
                   |   O
                   |   |
                   |
                   |
                ---------)";
                break;
            case 3:
                cout << R"(
                   -----
                   |   |
                   |   O
                   |  /|
                   |
                   |
                ---------)";
                break;
            case 4:
                cout << R"(
                   -----
                   |   |
                   |   O
                   |  /|\
                   |
                   |
                ---------)";
                break;
            case 5:
                cout << R"(
                   -----
                   |   |
                   |   O
                   |  /|\
                   |  /
                   |
                ---------)";
                break;
            case 6:
                cout << R"(
                   -----
                   |   |
                   |   O
                   |  /|\
                   |  / \
                   |
                ---------)";
                break;
        }
        cout << "\n\n";
    }

    void Hangman::provideHint() {
        if (remainingHints > 0) {
            for (int i = 0; i < word_length; ++i) {
                if (user_guess[i] == '_') {
                    user_guess[i] = guess_word[i];
                    remainingHints--;
                    correct_guess--;
                    break;
                }
            }
        } else {
            cout << "No hints remaining!\n";
        }
    }

    void Hangman::play() {
        try {
            // Reset game-specific variables for a fresh game
            incorrect_guess = 0;  // Reset incorrect guesses
            remainingHints = 2;   // Reset hints to the initial value

            totalGamesPlayed++;
            selectRandomQuestion();

            while (correct_guess > 0 && incorrect_guess < 6) {
                displayLevel(incorrect_guess);
                cout << "Word: " << user_guess << endl;
                cout << "Enter your guess (or type '!' for a hint): ";
                cin >> guess;

                if (guess == '!') {
                    provideHint();
                    cout << "Hints remaining: " << remainingHints << "\n";
                } else if (isalpha(guess) && checkGuess()) {
                    cout << "Correct guess!\n";
                } else {
                    cout << "Incorrect guess.\n";
                    incorrect_guess++;
                }
            }

            if (correct_guess == 0) {
                totalWins++;
                displayWinArt();
            } else {
                displayLoseArt();
            }

            displayStats();

            // Prompt the user to play again or exit
            char choice;
            cout << "\nDo you want to play again? (Y/N): ";
            cin >> choice;
            if (toupper(choice) == 'Y') {
                play();  // Restart the game
            } else {
                cout << "\nExiting the game. Thank you for playing!\n";
                exit(0); // Exit the program
            }
        } catch (const exception& e) {
            cerr << "An error occurred: " << e.what() << endl;
        }
    }

    void Hangman::loadQuestionsFromFile(const string& filename) {
        try {
            ifstream file(filename);
            if (!file) {
                throw runtime_error("Error: Could not open file " + filename);
            }

            string prompt, answer;
            while (getline(file, prompt) && getline(file, answer)) {
                questionManager.addQuestion(Question(prompt, answer));
            }
            file.close();
        } catch (const exception& e) {
            cerr << "An error occurred while loading questions: " << e.what() << endl;
        }
    }
}

void displayMenu() {
    cout << "\n";
    cout << "+-------------------------------+\n";
    cout << "|          Main Menu            |\n";
    cout << "+-------------------------------+\n";
    cout << "| (1) -> Play game              |\n";
    cout << "| (2) -> How to play            |\n";
    cout << "| (3) -> Exit                   |\n";
    cout << "+-------------------------------+\n";
    cout << "Enter your choice: ";
}

void displayWelcomeArt() {
    cout << R"(
HH       HH       AA       NN      NN       GGGG        MM      MM       AA       NN     NN
HH       HH     AA  AA     NN N    NN     GG    GG      MMMM  MMMM     AA  AA     NNN    NN
HH       HH     AA  AA     NN  N   NN     GG            MM MMMM MM     AA  AA     NNNN   NN
HHHHHHHHHHH    AAAAAAAA    NN   N  NN     GG   GGGG     MM  MM  MM    AAAAAAAA    NN  NN NN
HH       HH    AA    AA    NN   N  NN     GG     GG     MM      MM    AA    AA    NN   NNNN
HH       HH    AA    AA    NN    N NN      GG    GG     MM      MM    AA    AA    NN    NNN
HH       HH    AA    AA    NN     NNN        GGGG GG    MM      MM    AA    AA    NN     NN
)" << endl;
}

void displayWinArt() {
    cout << R"(
WW      WW     II     NN      NN     NN     NN     EEEEEEEE     RRRRRR
WW      WW     II     NN N    NN     NNN    NN     EE           RR   RR
WW      WW     II     NN  N   NN     NN N   NN     EEEEEEEE     RRRRRR
 WW WW WW      II     NN   N  NN     NN  N  NN     EE           RR  RR
   W   W       II     NN    N NN     NN   N NN     EEEEEEEE     RR   RR
)" << endl;
}

void displayLoseArt() {
    cout << R"(
LL             OOOOOO      SSSSSSS     EEEEEEEE
LL            OO    OO    SS           EE      
LL            OO    OO     SSSSSSS     EEEEEEEE
LL            OO    OO           SS    EE      
LLLLLLL        OOOOOO      SSSSSSS     EEEEEEEE
)" << endl;
}

int main() {
    displayWelcomeArt();

    HangmanGame::Hangman hangman;
    hangman.loadQuestionsFromFile("questions.txt");
    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                hangman.play();
                break;
            case 2:
                Utility::showGameRules();
                break;
            case 3:
                cout << "Exiting the game. Thank you for playing!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}
