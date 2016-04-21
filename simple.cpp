#include "Simple.h"

namespace Gaming
{
    const char Simple::SIMPLE_ID = 'S';

    Simple::Simple(const Game &g, const Position &p, double energy) : Agent(g, p, energy)
    {
    }

    Simple::~Simple()
    {
    }

    void Simple::print(std::ostream &os) const
    {
        os << SIMPLE_ID << __id;
    }

    ActionType Simple::takeTurn(const Surroundings &s) const
    {

        std::vector<int> resourcePositions;
        std::vector<int> emptyPositions;

        int emptySpace = 4; //stay

        for (int i = 0; i < 9; i++)
        {
            if (s.array[i] == FOOD || s.array[i] == ADVANTAGE)
            {
                resourcePositions.push_back(i);
            }

            else if (s.array[i] == EMPTY)
            {
                emptyPositions.push_back(i);
            }
        }

        unsigned int randNum;
        if (resourcePositions.size() > 0)
        {
            randNum = rand() % resourcePositions.size();
            emptySpace = resourcePositions[randNum];
        }
        else if (emptyPositions.size() > 0)
        {
            randNum = rand() % emptyPositions.size();
            emptySpace = emptyPositions[randNum];
        }


        switch (emptySpace)
        {
            case 0:
                emptySpace = 2;
                break;
            case 1:
                emptySpace = 0;
                break;
            case 2:
                emptySpace = 1;
                break;
            case 3:
                emptySpace = 4;
                break;
            case 4:
                emptySpace = 8;
                break;
            case 5:
                emptySpace = 3;
                break;
            case 8:
                emptySpace = 5;
                break;
        }
        return static_cast<ActionType>(emptySpace);
    }
}



