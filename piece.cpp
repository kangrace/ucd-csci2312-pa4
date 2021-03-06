#include "Piece.h"

namespace Gaming
{
    unsigned int Piece::__idGen = 0;

    Piece::Piece(const Game &g, const Position &p) : __game(g)
    {
        __finished = false;
        __turned = false;
        __position = p;
        __id = __idGen++;
    }

    Piece::~Piece()
    {
    }

    std::ostream &operator<< (std::ostream &os, const Piece & piece)
    {
        piece.print(os);
        return os;
    }
}
