/*
Josh Mencsik
23 April 2020
The header for the spaces class, which represents the spaces on the games board
Can either be white, black, or empty
*/

#include<iostream>
//#include"game.h"

#ifndef SPACES_H
#define SPACES_H
class space {
    public:
        //enum type to read the value of a space
        enum tile {WHITE, BLACK, EMPTY};

        space (tile val = EMPTY){currentColor = val;}

        //getter and setter
        tile getSpace() const {return currentColor;}
        void setSpace(tile val) {currentColor = val;}

        tile opposite () const {if (currentColor == WHITE) return BLACK;
                                    else if (currentColor == BLACK) return WHITE;
                                    else return EMPTY;}

        void flip() {if(currentColor == EMPTY) return;
                    else if (currentColor == WHITE)currentColor = BLACK;
                    else currentColor = WHITE;}


    private:
        tile currentColor;
};


#endif