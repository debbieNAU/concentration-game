#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <cstring>

using namespace std;

/* CYB-136 Module 10 - Assignment 3
 * Student: Debra Caruthers. Date: Mar. 24, 2023
 * Program title: Memory Game and Typing Game
 * Filename: memory_game.cpp
 * Description: This program simulates a popular game that many people call "Concentration" or "Match Pairs."
 * In real life, you play this with a deck of cards that contain two of every pip value. In this game here,  
 * we play with an imaginary deck of 16 cards. So, there are two Ones, two Two's, two Threes, etc., in the  
 * deck, up to the number Eight.
 *  
 * Game play: User will see a 4x4 grid of 16 Row,Column positions occupied by an "*" character.
 * Eight pairs of numbers 1-8 are randomly hidden in that grid. User inputs the Row,Column position of 
 * the first card to flip over as his first guess. So, to uncover the first card in the upper left corner, 
 * he would input this:
 * 1 1 (One spacebar One).
 * 
 * Next, User inputs his guess as to what Row,Col position on the grid holds the card (or Tile) to match 
 * the first card guess. So he might input this to see the Tile in Row 3, Column 4:
 * 3 4 (Three spacebar Four).
 * 
 * If his two guesses match each other, he sees a Matched! message and the " * " changes into a " @ " character.
 * If his two guesses do not match, he will be prompted to input two fresh guesses.
 * 
 * Object: User wants to keep guessing until he has matched all pairs and the whole grid now shows " @ " characters. 
 * The game exercises your memory powers because when your guesses are wrong, that card is flipped back down.
 * You need to remember where it was for when you eventually uncover its mate! Game ends when all cards have been matched.
 * 
 * This is similar to Exercise P15.11 of our textook, page 640.
 * 
 * When User finishes or quite the MEMORY game, program will prompt user to ask if he wants to play 
 * an optional SPEED TYPING GAME. I added this to demonstrate the usual STACK ALLOCATION of memory
 * and to submit a program whose line count is closer to the assignment's rubric.
 * 
 * The TYPING GAME will display a string of characters to user and challenge him to type those
 * same characters as fast as possible. He can choose a string difficulty level of 1-3. 
 * User will have option to exit from that game menu too.
 */


// Create our Tile class constructor.
// The Tile object represents an instance of each of the 16 cards in our 4x4 game grid.
class Tile {
public:
    int value;      //Will hold a value of one of the elements of the 16-member array named 'cards.'
    bool flipped;   //If user has guessed correctly, the Tile's status is now 'flipped' true to show itself to User.

    Tile(int value) : value(value), flipped(false) {}
};

// FUNCTION printBoard to display the board of 16 cards or 'tiles' in a grid pattern.
void printBoard(Tile *board[4][4]) {

    // Show the 4x4 board by filling the ROWS and the COLUMNS in a nested FOR loop.

    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            // If (board[i][j]->flipped) {   //THIS LINE THREW SEGEMENTATION ERROR UNTIL WE CHECKED FOR NULLPOINTER BELOW FIXED.
            /* The issue might be related to accessing a deleted pointer. 
            * After a match is found, we should remove the pointers from the board by setting them to nullptr. 
            * To fix the issue, update the "checkMatch" function and the "printBoard" function as follows:
            */

            // IF user's guess matches the Row,Col of the Tile, the 'flipped' BOOL value is now true.
            // Let user see the matched card tile.
            if (board[i][j] != nullptr && board[i][j]->flipped) 
            {
                std::cout << board[i][j]->value << " ";
            } 
            // Show previous guessed tiles with '@' character.
            else if (board[i][j] == nullptr)
            {
                std::cout << "@ ";
            }
            // IF user's guess does not match, card remains hidden and shows "*" character.
            else 
            {
                std::cout << "* ";
            }
        }
        std::cout << std::endl;
    }
// End printBoard function here.
}

// FUNCTION to return the boolean value if User's guess matches the board tile.
bool checkMatch(Tile *board[4][4], int r1, int c1, int r2, int c2) {

    if (board[r1][c1]->value == board[r2][c2]->value) {

        // Do this if the two chosen Tile object values match each other.
        // User no longer needs to find these two objects, so the
        // memory on the two correctly guessed pair of positions is deallocated.
        delete board[r1][c1];
        delete board[r2][c2];

        // After a match is found, we remove the pointers from the board by setting them to nullptr (force evauluate to FALSE). 
        // Otherwise, a Segmentation error occurred at run time if User inputs a guess that was previously matched already.
        board[r1][c1] = nullptr;
        board[r2][c2] = nullptr;

        return true;
    }
    // If the two guessed Tiles do not match each other, this function returns false.
    return false;
}

// -------------------------------------------------------
// I ADDED AN OPTIONAL GAME FUNCTION HERE TO MEET THE 
// ASSIGNMENT REQUIREMENT OF 500 CODE LINES. THIS GAME
// DEMONSTRATES THE USE OF STACK MEMORY.
//
// DECLARE VARIABLES, FUNCTIONS FOR OPTIONAL TYPING GAME

const int MAX_LENGTH = 10; // maximum length of sequence
const int MAX_LEVEL = 3; // maximum difficulty level
const int TIME_LIMIT = 5000; // time limit in milliseconds

int high_scores[MAX_LEVEL + 1] = {0}; // fastest times for each level

void display_high_scores() {
    cout << "Fastest times:\n";
    for (int i = 1; i <= MAX_LEVEL; i++) {
        if (high_scores[i] == 0) {
            cout << "Level " << i << ": Not yet completed.\n";
        } else {
            cout << "Level " << i << ": " << high_scores[i] << " milliseconds.\n";
        }
    }
}
// END VARIABLES, FUNCTIONS FOR OPTIONAL TYPING GAME

// FUNCTION BEGIN TO PLAY OPTIONAL TYPING GAME HERE.
// The memory for these variables are stack-allocated.
int typingGame() {
    char sequence[MAX_LENGTH + 1]; // sequence buffer (+1 for null terminator)
    char input[MAX_LENGTH + 1]; // input buffer (+1 for null terminator)
    int length; //    for length of sequence
    int level; // difficulty level
    int start_time, end_time; // start and end times in milliseconds
    int i; // loop counter
    
    // seed the random number generator
    srand(time(nullptr));
    
    // display the menu
    do {
        cout << "Type the number of the option you want:\n";
        cout << "1. Play the game\n";
        cout << "2. View high scores\n";
        cout << "3. Reset high scores\n";
        cout << "4. Quit the program\n";
        
        // get the player's choice
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                // get the difficulty level
                do {
                    cout << "Choose a difficulty level (1-" << MAX_LEVEL << "):\n";
                    cin >> level;
                } while (level < 1 || level > MAX_LEVEL);
                
                // generate the sequence
                length = level * 2;
                for (i = 0; i < length; i++) {
                    sequence[i] = 'a' + rand() % 26; // generate a random lowercase letter
                }
                sequence[length] = '\0'; // null terminate the sequence
                
                // display the sequence
                cout << "Type the following sequence as quickly as possible:\n";
                cout << sequence << endl;
                
                // record the start time
                start_time = clock();
                
                // get the player's input
                cin.ignore(); // ignore the newline character left by cin
                cin.getline(input, MAX_LENGTH + 1);
                
                // check if the input matches the sequence
                if (strcmp(input, sequence) == 0) {
                    // record the end time
                    end_time = clock();
                    
                    // calculate the time taken
                    int time_taken = end_time - start_time;
                    cout << "Congratulations! You typed the sequence correctly in "
                                         << time_taken << " milliseconds." << endl;
                
                // check if the player beat the high score
                if (high_scores[level] == 0 || time_taken < high_scores[level]) {
                    high_scores[level] = time_taken;
                    cout << "You beat the high score for level " << level << "!\n";
                }
            } else {
                cout << "Sorry, you typed the wrong sequence. Please try again." << endl;
            }
            
            break;
        
        case 2:
            // display the high scores
            display_high_scores();
            break;
        
        case 3:
            // reset the high scores
            for (int i = 1; i <= MAX_LEVEL; i++) {
                high_scores[i] = 0;
            }
            cout << "High scores have been reset.\n";
            break;
        
        case 4:
            // exit the program
            return 0;
        
        default:
            // invalid choice
            cout << "Invalid choice. Please try again.\n";
            break;
    }
} while (true);


}
// END TYPING GAME FUNCTION HERE
// -------------------------------------------------------


// BEGIN MAIN HERE.
int main() {
    
    // INITIALIZE VARIABLES

    // Using 'time' to randomize the 'shuffling' of the 16 cards
    srand(time(0));

    // Class tILE to hold our 'board' objects of 16 cards in a 4x4 grid.
    // Pointer to 'board'. Heap memory allocated
    Tile *board[4][4];

    // Variable to track how many many patched pairs yet need to be found. Original value is 8 total pairs (from 16 cards).
    int remainingPairs = 8;

    // Variable to keep track of how many 'tries' or 'steps' it took User to find all pairs.
    int steps = 0;

    // Array to hold int value of the 16 cards, There are 8 pairs numbered 1-8.
    int cards[16] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8};

    // Statement to 'shuffle' the 16 cards in a FOR loop using a randomizer function and modulo.
    // This uses the 'swap' command in the standard C++ library.
    for (int i = 0; i < 16; ++i) {
        int j = rand() % (16 - i) + i;
        std::swap(cards[i], cards[j]);
    }

    // Now place those shuffled cards in a 4x4 grid.
    // Instantiates a "Tile" object for each card in the grid. Memory allocated here with 'new'.
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            board[i][j] = new Tile(cards[i * 4 + j]);
        }
    }

    // DO this loop so long as matched pairs have not yet been all found.
    while (remainingPairs > 0) 
    {
        // Call function to display our board of 16 shuffled cards "face-down."
        printBoard(board);

        // Initialize variables to hold User's two Row-Column guesses.
        int r1, c1, r2, c2;

        // Using string input because User can opt to type "Q" to quit game anytime or can enter his integer guesses.
        std::string input;
        std::istringstream iss;

    // BEGIN EXCEPTION HANDLING HERE WITH TRY-CATCH AND THROW STATEMENTS
    // We use this for input validation errors or when User wants to exit the game early.

        try 
        {
            std::cout << "Enter row and column for the first card (1-4), or type 'Q' to quit: ";
            std::getline(std::cin, input);
            iss.str(input);

            if (input == "Q" || input == "q") 
            {
                std::cout << "Exiting the game early." << std::endl;
                break;
            }

            // If User has not quit, receive his r1 and c1 integers for his first guess.
            iss >> r1 >> c1;

            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                throw std::runtime_error("Invalid input. Please enter integers.");
            }       

            if (r1 < 1 || r1 > 4 || c1 < 1 || c1 > 4) 
            {
                throw std::out_of_range("Input out of bounds. Please try again.");
            }

            // Receive User's input for his second guess.
            
            std::cout << "Enter row and column for the second card (1-4), or type 'Q' to quit: ";

            std::getline(std::cin, input);
            iss.clear();
            iss.str(input);

            if (input == "Q" || input == "q") 
            {
                std::cout << "Exiting the game early." << std::endl;
                break;
            }

            // User has not quit, so process his input of integers for his second guess

            iss >> r2 >> c2;

            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                throw std::runtime_error("Invalid input. Please enter integers.");
            }

            if (r2 < 1 || r2 > 4 || c2 < 1 || c2 > 4) 
            {
                throw std::out_of_range("Input out of bounds. Please try again.");
            }

        // End 'try' here.
        } 
        
        // Begin 'catches' here.
        // Display appropriate exception error messages.
        catch (std::runtime_error &e) 
        {
            std::cout << e.what() << std::endl;
            continue;
        } 
        catch (std::out_of_range &e) 
        {
            std::cout << e.what() << std::endl;
            continue;
        }

        // Check if User's input shows the same card position twice: input validation.
        if (r1 == r2 && c1 == c2) 
        {
        std::cout << "You selected the same card twice. Please try again." << std::endl;
        continue;
        }

        // Decrement entered values by 1 to match indices; otherwise User has to enter Row 0, Column 0, etc.
        r1--; c1--; r2--; c2--;  

        // Check if User has input two card positions that were already matched previously and are now set to nullptr.
        if (board[r1][c1] == nullptr || board[r2][c2] == nullptr) 
        {
        std::cout << "You selected a card that has already been matched. Please try again." << std::endl;
        continue;
        }

        // User's two guesses pass input validation.
        // The status of the guessed Row,Col of a Tile is now 'flipped' true but not necessarily matched yet.
        // The variable 'steps' is incremented once to tracker number of User's attempts.
        board[r1][c1]->flipped = true;
        board[r2][c2]->flipped = true;
        steps++;

        // Now call function to check for a matched pair.
        printBoard(board);

        if (checkMatch(board, r1, c1, r2, c2)) 
        // The checkMatch function returns a True or False of finding a matched pair.
        // IF true, then display Match Found message.
        {
            std::cout << "Match found!" << std::endl;
            // Decrement number of possible matched pairs now that one has been found.
            remainingPairs--;
            if (remainingPairs == 0) {
                std::cout << "Congratulations! You found all pairs in " << steps << " steps." << std::endl;
                printBoard(board); // Display the finished board. Should now display the '@' in all tiles.

            }
        } 
        else
        // IF false, then display No Match message.
        // The flipped status of the guessed Tile is changed back to false. 
        // User can choose an unmatched Tile later.
        {
            std::cout << "No match. Try again." << std::endl;
            board[r1][c1]->flipped = false;
            board[r2][c2]->flipped = false;
        }
    }

    // Free the memory and clear board.
    // This section frees the allocated memory for the remaining Tile instances in the board and clears the board. 
    // The program returns 0 to indicate successful execution.
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            if (board[i][j] != nullptr) 
            {
                delete board[i][j];
            }
        }
    }

    // Prompt User to play optional TYPING GAME or quit the program entirely.
    std::cout << "Would you like to play the TYPING GAME now? Enter Y for yes, or any other key to quit program: " << std::endl;
    // get the player's choice
    string nextGame;
    cin >> nextGame;
    if (nextGame == "Y" || nextGame == "y" )
    {
        // Call optional function to play a quick TYPING GAME
        typingGame();
    }

    std::cout << "Thanks for playing the MEMORY and TYPING GAMES! Bye-bye. " << std::endl;
    return 0;
}

