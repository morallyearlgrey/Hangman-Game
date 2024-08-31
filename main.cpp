#include "headers.h"
#include <cctype>

using namespace std;

// encompasses the stickman and its 6 body parts for the hangman game
struct stickman {
    bool head;
    bool body;
    bool larm;
    bool rarm;
    bool lleg;
    bool rleg;
    
    vector<string> man;

    // constructor, creates a stickman object and initializes the vector to be empty 
    stickman(bool h, bool b, bool la, bool ra, bool ll, bool rl) 
        : head(h), body(b), lleg(ll), rleg(rl), larm(la), rarm(ra), man(6, " ") {}

    // updates the specific body part to show up on hangman after each wrong answer
    void update(int index) {
        switch(index) {
            case(1):
                head = true;
                man[0] = "O";
                break;
            case(2):
                body = true;
                man[1] = "|";
                break;
            case(3):
                lleg = true;
                man[2] = "/";
                break;
            case(4):
                rleg = true;
                man[3] = "\\";
                break;
            case(5):
                larm = true;
                man[4] = "\\";
                break;
            case(6):
                rarm = true;
                man[5] = "/";
                break;
        }
    }

    // returns the stickman vector for printing out to the terminal
    vector<string> returnTheMan() {
        return man;
    }

    // clears the stickman struct and vector
    void clearTheMan() {
        head = false;
        body = false;
        lleg = false;
        rleg = false;
        larm = false;
        rarm = false;
        man = vector<string>(6, " ");

    }
};

// encompasses the entire game and its associated functions
class game {
    int rows = 8;
    int cols = 8;

    int nwon;
    int nplays;

    stickman man;

    vector<vector<string> > grid;

    string word;
    vector<string> wordGrid;
    char letter;

    int nWrong;


public:
    // creates a game object and initializes the grid
    game(int w, int p, stickman& m) 
        : nwon(w), nplays(p), man(m), grid(rows, vector<string>(cols, " ")), nWrong(0) {}

    // starts the game with a word input and then creates the line for the word that fills up during the game
    // sets the grid to contain the hanging machine
    void startGame(string newWord) {
        nWrong = 0;
        word = newWord;
        wordGrid = vector<string>(word.size(), "_");
        man.clearTheMan();

        for(int r = 0; r < rows; r++) {
            for(int c = 0; c < cols; c++) {
                grid[r][c] = " ";
            }
        }

        for(int i = 1; i < rows - 2; i++) {
            grid[1][i] = "-";
            grid[6][i] = "-";
        }

        for(int j = 2; j < cols - 2; j++) {
            grid[j][5] = "|";
        }

        grid[2][1] = "|";
    }

    // enters a character and processes if the letter is in the word or not
    void enterInput(char l) {
        letter = l;

        bool wrong = true;

        for(int i = 0; i < word.size(); i++) {
            if(word[i] == letter) {
                wordGrid[i] = letter;
                wrong = false;
            } 
        }

        if(wrong) {
            nWrong += 1;
            addBodyPart(nWrong);
        }
    }

    // makes the body parts actually show up on the hangman game
    void addBodyPart(int nWrong) {
        man.update(nWrong);

        for(int x = 0; x < 3; x++) {
            grid[3 + x][1] = man.returnTheMan()[x];
        }

        grid[4][0] = man.returnTheMan()[4];
        grid[4][2] = man.returnTheMan()[5];
        grid[5][2] = man.returnTheMan()[3];
    }

    // prints out the game each iteration
    void printGame() {
        for(int r = 0; r < grid.size(); r++) {
            for(int c = 0; c < grid[r].size(); c++) {
                cout << grid[r][c];
            }
            cout << "\n";
        }

        for(int i = 0; i < wordGrid.size(); i++) {
            cout << wordGrid[i] << " ";
        }
    }

    // checks to see the status of the game, whether it's been won, lost, or neither
    int checkGame() {
        if(man.returnTheMan()[5] != " ") {
            return -1;
        } else {
            bool won = true;
            for(int i=0; i<wordGrid.size(); i++) {
                if(wordGrid[i]=="_") {
                    won = false;
                    break;
                }
            }
            if(won) {
                return 1;
            } else {
                return 0;
            }
            return 1;

        }
    }

    // updates the score accordingly
    void changeScore(int num) {
        switch(num) {
            case(1):
                nwon += 1;
                nplays += 1;
                break;

            case(-1):
                nplays += 1;
                break;
        }
    }

    // returns the num games played
    int plays() {
        return nplays;
    }

    // prints score 
    void printScore() {
        string bar = "====================";
        string message = bar + "\nSCOREBOARD: \nWON: " + to_string(nwon) + "\n" + "PLAYED: " + to_string(nplays) + "\n" + bar + "\n";
        cout << message;
    }
};

// plays the game
int main() {
    srand(time(0));

    string bar = "====================";
    cout << "\n\n" << bar << "\n\n";
    cout << "SIMPLE COMMAND LINE HANGMAN GAME";
    cout << "\n\n" << bar << "\n";
    cout << "INSTRUCTIONS:\n";
    cout << " 1) When prompted, enter the number of words that will be used during Hangman.\n 2) Then, list each word separated by spaces.\n 3) Play Hangman by guessing letters for the random words.";
    cout << "\n" << bar << "\n";
    cout << "Enter the number of words that will be used: ";

    int numWords;
    cin >> numWords;
    cout << "" << bar << "\n";
    cout << "Enter each word (separate with spaces): \n";

    vector<string> wordList;
    for(int i=0; i<numWords; i++) {
        string word;
        cin >> word;
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        wordList.push_back(word);
    }

    cout << "" << bar << "\n";
    cout << "Starting game... Input 0 to quit";
    cout << "\n" << bar << "\n";

    char input;
    stickman man (false, false, false, false, false, false);
    game newGame(0, 0, man);

    int wordListSize = wordList.size()-1;
    
    while((input!='0')&&(wordList.size()>1)) {
        int randomWordIndex = 1+(rand()%(wordListSize));
        string word = wordList[randomWordIndex];
        wordListSize-=1;
        wordList.erase(wordList.begin() + randomWordIndex);

        newGame.printScore();
        newGame.startGame(word);
        newGame.printGame();
        cout << "\n" << bar << "\n";

        while((newGame.checkGame()==0) && (input!='0')) {
            cout << "\nENTER LETTER: ";
            cin >> input;

            input = tolower(input);

            newGame.enterInput(input);
            newGame.printGame();
            cout << "\n" << bar << "\n";
        }

        if(newGame.checkGame()==1) {
            cout << "YOU WON!\n";
            newGame.changeScore(1);

        } else if(newGame.checkGame()==-1) {
            cout << "YOU LOST!" << "\n";
            newGame.changeScore(-1);

        }
        
    }
    newGame.printScore();
    cout << "Quiting...";
    return 0;

}
