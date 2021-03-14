/*
Josh Mencsik
23 April 2020
A child class of game.h, allows the playing of Othello
*/

#include<iostream>
#include<queue>
#include"game.h"
#include"spaces.h"
#include"colors.h"

#ifndef OTHELLO_H
#define OTHELLO_H
using namespace main_savitch_14; 

class Othello : public game {
    public:
        Othello();

        void make_move(const std::string& move); //check game, the function to make a move
        void restart(); //also check game, does what it says

        void display_status( ) const; //display the board
        bool is_legal(const std::string& move) const; //true if the move is valid

        int evaluate( ) const; //return a rating value of a move, pos is good for computer
        bool is_game_over() const; //true if no valid moves on either side

        who winning( ) const;

        //grabbed this just to allow my check of invalid turns
        std::string get_user_move( ) const;

        game* clone( ) const; //copy the game

        //run through the entire board and check if legal, if it is add to the queue
        void compute_moves(std::queue<std::string>& moves) const;

        //the game get move functions don't work because i'm not using game's move_number, i'm using othello's
        //so use Othello's move functions
        who last_mover( ) const 
	    { return (move_number % 2 == 1 ? HUMAN : COMPUTER); }

        who next_mover( ) const
	    { return (move_number % 2 == 0 ? HUMAN : COMPUTER); }


       

    private:
        //return the position of an adjacent space
        bool checkAdj(int row, int col, space::tile color) const;
        bool isFlanked(int row, int col, std::string direction, space::tile color) const;
        void flipIt(int row, int col, space::tile color);

        int passes;
        int move_number;
        space board[8][8];


};






#endif