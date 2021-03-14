/*
Josh Mencsik
23 April 2020
The implementation of the Othello class
*/

#include <iostream>
#include<string>
#include<cctype>
#include "othello.h"

using namespace std;

Othello::Othello() {
    move_number = 0;
    passes = 0;
    board[3][3].setSpace(space::WHITE);
    board[3][4].setSpace(space::BLACK);
    board[4][3].setSpace(space::BLACK);
    board[4][4].setSpace(space::WHITE);
}

void Othello::display_status( ) const {
    //the letter row
    cout << "    A   B   C   D   E   F   G   H" << endl;
    cout << "  ";
    for (int n = 0; n < 8; n++)
            cout << " ---";
    cout << endl;

    for (int i = 0; i < 8; i++) {
        //how we get the number column
        cout << i+1 << " ";
        for (int j = 0; j < 8; j++) {
            cout << "|";
            if (board[i][j].getSpace() == space::WHITE)
                cout << B_WHITE << "   " << RESET;
            else if (board[i][j].getSpace() == space::BLACK)
                cout << B_BLACK << "   " << RESET;
            else 
                cout << "   ";
            //cout << RESET;
        }
        cout << "|" << endl;
        cout << "  ";
        for (int n = 0; n < 8; n++)
            cout << " ---";
        cout << endl;
    }
    cout << RESET << endl;
}

void Othello::restart() {
    //clearing the board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j].setSpace(space::EMPTY);
        }
    }
    
    //setting up the first four pieces
    board[3][3].setSpace(space::WHITE);
    board[3][4].setSpace(space::BLACK);
    board[4][3].setSpace(space::BLACK);
    board[4][4].setSpace(space::WHITE);
    move_number = 0;
}

void Othello::make_move(const std::string& move) {
    //checking for a pass
    if (move == "PASS") {
        //cout << "No valid moves" << endl;
        passes++;
        return;
    }
    passes = 0;

    //assuming move is purely letter than number, no spaces before else it would break
    //Also already checked the legality of the move
    char letter = tolower(move.at(0));
    char num = move.at(1);

    //setting their piece down
    if (move_number%2 == 0) {
        board[num - '1'][letter - 'a'].setSpace(space::BLACK);
    }
    else {
        board[num - '1'][letter - 'a'].setSpace(space::WHITE);
    }

    //old code for a 2 player game
    // if (move_number%2 ==0) {
    // }
    // else {
    //     board[num - '1'][letter - 'a'].setSpace(space::WHITE);
    //     color = space::WHITE;
    // }

    
    //special function to flip everything
    flipIt(num - '1', letter - 'a', board[num - '1'][letter - 'a'].opposite());
   
    move_number++;
}


bool Othello::is_legal(const std::string& move) const {
    //check for a pass
    if (move == "PASS") {
        return true;
    }

    //basic validity check, make sure they gave a valid input
    if (move.length() != 2)
        return false;

    char letter = tolower(move.at(0));
    char num = move.at(1);

    if(!(letter >= 'a' && letter <='h') || !(num >= '1' && num <= '8'))
        return false;

    //not allowed to place a piece over another (something i didn't have in the two player, whoops)
    if (board[num - '1'][letter - 'a'].getSpace() != space::EMPTY)
        return false; 

    //if the move is being made by the player, they will be playing BLACK
    if (move_number%2 == 0) {//other way to check turn (next_mover() == game::HUMAN)
        return checkAdj(num-'1',letter-'a', space::WHITE);
    }

    //if the next move is being made by the computer, they will be playing WHITE
    else {
        return checkAdj(num-'1',letter-'a', space::BLACK);
    }
}

//color is the inverse of the player's piece, messed that up don't wanna change it, so have to do that :(
bool Othello::checkAdj(int row, int col, space::tile color) const {
    //if speed is too slow, could throw in a preceeding if to check for a zero off the bat
    //special case checking
        //check structure, it checks the immediate square for a precheck, then runs isFlanked
        if (row > 0 && col > 0 && board[row-1][col-1].getSpace() == color && isFlanked(row, col, "NW", color))
            return true;
        else if (row > 0 && board[row-1][col].getSpace() == color && isFlanked(row, col, "N", color))
            return true;
        else if (row > 0 && col < 7 && board[row-1][col+1].getSpace() == color && isFlanked(row, col, "NE", color))
            return true;
        else if (col > 0 && board[row][col-1].getSpace() == color && isFlanked(row, col, "W", color))
            return true;
        else if (col < 7 && board[row][col+1].getSpace() == color && isFlanked(row, col, "E", color))
            return true;
        else if (row < 7 && col > 0 && board[row+1][col-1].getSpace() == color && isFlanked(row, col, "SW", color))
            return true;
        else if (row < 7 && board[row+1][col].getSpace() == color && isFlanked(row, col, "S", color))
            return true;
        else if (row < 7 && col < 7 && board[row+1][col+1].getSpace() == color && isFlanked(row, col, "SE", color))
            return true;
        else 
            return false;
    }

bool Othello::isFlanked(int row, int col, string direction, space::tile color) const {
    //empty case check on current space, shouldn't ever call but a good backup i guess
    // if (board[row][col].getSpace() == space::EMPTY)
    //     return false;

    //that first case is out of the way, so we know the first time won't return true
    //ride color until you find the opposite (which is the current piece color)
    
    if (direction == "NW" && row > 0 && col > 0) {
        if (color == board[row-1][col-1].opposite())
            return true;
        else if (board[row-1][col-1].getSpace() == space::EMPTY)
            return false;
        else //same color found
            return isFlanked(row-1, col-1, "NW", color);
    }
    //N -1 0
    else if (direction == "N" && row > 0) {
        if (color == board[row-1][col].opposite())
            return true;
        else if (board[row-1][col].getSpace() == space::EMPTY)
            return false;
        else {//same color found
            return isFlanked(row-1, col, "N", color);
        }
    }
    //NE, -1 +1
    else if (direction == "NE" && row > 0 && col < 7) {
        if (color == board[row-1][col+1].opposite())
            return true;
        else if (board[row-1][col+1].getSpace() == space::EMPTY)
            return false;
        else {//same color found
            return isFlanked(row-1, col+1, "NE", color);
        }
    }
    //W 0 -1
    else if (direction == "W" && col > 0) {
        if (color == board[row][col-1].opposite())
            return true;
        else if (board[row][col-1].getSpace() == space::EMPTY)
            return false;
        else {//same color found
            return isFlanked(row, col-1, "W", color);
        }
    }
    //E 0 +1
    else if (direction == "E" && col < 7) {
        if (color == board[row][col+1].opposite())
            return true;
        else if (board[row][col+1].getSpace() == space::EMPTY)
            return false;
        else {//same color found
            return isFlanked(row, col+1, "E", color);
        }
    }
    //SW +1 -1
    else if (direction == "SW" && row < 7 && col > 0) {
        if (color == board[row+1][col-1].opposite())
            return true;
        else if (board[row+1][col-1].getSpace() == space::EMPTY)
            return false;
        else {//same color found
            return isFlanked(row+1, col-1, "SW", color);
        }
    }
    //S +1 0
    else if (direction == "S" && row < 7) {
        if (color == board[row+1][col].opposite())
            return true;
        else if (board[row+1][col].getSpace() == space::EMPTY)
            return false;
        else {//same color found
            return isFlanked(row+1, col, "S", color);
        }
    }
    //SE +1 +1
    else if (direction == "SE" && row < 7 && col < 7) {
        if (color == board[row+1][col+1].opposite())
            return true;
        else if (board[row+1][col+1].getSpace() == space::EMPTY)
            return false;
        else {//same color found
            return isFlanked(row+1, col+1, "SE", color);
        }
    }
    else {//would call if direction isn't valid
        //cout << "ERROR: Invalid direction passed to isFlanked" << endl;
        return false;
    }
    
}


void Othello::flipIt(int row, int col, space::tile color) {
    //recycled bits from checkAdj and isFlanked
    int tempR = row;
    int tempC = col;

    if (row > 0 && col > 0 && board[row-1][col-1].getSpace() == color && isFlanked(row, col, "NW", color)) {
        while(board[tempR-1][tempC-1].getSpace() == color) {
            board[tempR-1][tempC-1].flip();
            tempR--;
            tempC--;
        }
    }
    tempR = row;
    tempC = col;

    if (row > 0 && board[row-1][col].getSpace() == color && isFlanked(row, col, "N", color)) {
        while(board[tempR-1][tempC].getSpace() == color) {
            board[tempR-1][tempC].flip();
            tempR--;
        }
    }
    tempR = row;
    tempC = col;
        
    if (row > 0 && col < 7 && board[row-1][col+1].getSpace() == color && isFlanked(row, col, "NE", color)) {
        while(board[tempR-1][tempC+1].getSpace() == color) {
            board[tempR-1][tempC+1].flip();
            tempR--;
            tempC++;
        }
    }
    tempR = row;
    tempC = col;
        
    if (col > 0 && board[row][col-1].getSpace() == color && isFlanked(row, col, "W", color)) {
        while(board[tempR][tempC-1].getSpace() == color) {
            board[tempR][tempC-1].flip();
            tempC--;
        }
    }
    tempR = row;
    tempC = col;
        
    if (col < 7 && board[row][col+1].getSpace() == color && isFlanked(row, col, "E", color)) {
        while(board[tempR][tempC+1].getSpace() == color) {
            board[tempR][tempC+1].flip();
            tempC++;
        }
    }
    tempR = row;
    tempC = col;
        
    if (row < 7 && col > 0 && board[row+1][col-1].getSpace() == color && isFlanked(row, col, "SW", color)) {
        while(board[tempR+1][tempC-1].getSpace() == color) {
            board[tempR+1][tempC-1].flip();
            tempR++;
            tempC--;
        }
    }
    tempR = row;
    tempC = col;
        
    if (row < 7 && board[row+1][col].getSpace() == color && isFlanked(row, col, "S", color)) {
        while(board[tempR+1][tempC].getSpace() == color) {
            board[tempR+1][tempC].flip();
            tempR++;
        }
    }
    tempR = row;
    tempC = col;
        
    if (row < 7 && col < 7 && board[row+1][col+1].getSpace() == color && isFlanked(row, col, "SE", color)) {
        while(board[tempR+1][tempC+1].getSpace() == color) {
            board[tempR+1][tempC+1].flip();
            tempR++;
            tempC++;
        }
    }
}

int Othello::evaluate() const { //is_game_over uses this, if this is changed change that 
    //need this to just return a count of pieces, return white - black
    int white = 0;
    int black = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].getSpace() == space::WHITE)
                white++;
            else if (board[i][j].getSpace() == space::BLACK)
                black++;
        }
    }

    return white - black;

    //this will calculate the number of moves the current player can make, the old evaluate
    // int moves = 0;
    // space::tile color;
    // if (move_number%2 == 0)
    //     color = space::WHITE;
    // else 
    //     color = space::BLACK;

    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         if (board[i][j].getSpace() == space::EMPTY && checkAdj(i, j, color))
    //             moves++;
    //     }
    // }

    // return moves;
}


bool Othello::is_game_over() const{
    if (passes == 2)
        return true;
    else 
        return false;

    //old check, now housed in evaluate
    // space::tile color;
    // if (move_number%2 == 0)
    //     color = space::WHITE;
    // else 
    //     color = space::BLACK;

    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         if (board[i][j].getSpace() == space::EMPTY && (checkAdj(i, j, space::BLACK) || checkAdj(i, j, space::WHITE)))
    //             return false;
    //     }
    // }

    // return true;
}

game::who Othello::winning( ) const {
    int black = 0, white = 0;

    //run through the board and tally the pieces
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].getSpace() ==space::BLACK)
                black++;
            else if (board[i][j].getSpace() == space::WHITE)
                white++;
        }
    }

    if (black > white)
        return game::HUMAN;
    else if (white > black)
        return game::COMPUTER;
    else 
        return game::NEUTRAL;
}

string Othello::get_user_move( ) const {
    //old evaluate, now just written in this function
    int moves = 0;
    space::tile color;
    if (move_number%2 == 0)
        color = space::WHITE;
    else 
        color = space::BLACK;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].getSpace() == space::EMPTY && checkAdj(i, j, color))
                moves++;
        }
    }

    if (moves == 0)
        return "PASS";
    else {
        string answer;
	
        display_message("Your move, please: ");
        getline(cin, answer);
        return answer;
    }
}

//functions for the computer

game* Othello::clone() const{
    return new Othello(*this);
}

void Othello::compute_moves(std::queue<std::string>& moves) const {
    string temp;
    //in case the computer doesn't have any moves
    int count = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].getSpace() == space::EMPTY && checkAdj(i, j, space::BLACK))
                count++;
        }
    }

    if (count == 0) {
        moves.push("PASS");
    }

    for (char i = 'a'; i < 'i'; i++) {
        for (char j = '1'; j < '9'; j++) {
            //create the move
            temp = i;
            temp += j;

            //test the move
            if (is_legal(temp)) {
                //add the move
                moves.push(temp);
                //cout << temp << endl;
            }
        }
    }
}