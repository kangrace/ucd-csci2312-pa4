#include "DefaultAgentStrategy.h"


namespace Gaming
{
    DefaultAgentStrategy::DefaultAgentStrategy()
    {
    }

    DefaultAgentStrategy::~DefaultAgentStrategy()
    {
    }

    ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const
    {
        std::vector<int> advantagePositions;
        std::vector<int> foodPositions;
        std::vector<int> strategyPositions;
        std::vector<int> emptyPositions;
        std::vector<int> simplePositions;

        for (int i = 0; i < 9; i++)
        {
            if (s.array[i] == ADVANTAGE)
            {
                advantagePositions.push_back(i);
            }

            else if (s.array[i] == FOOD)
            {
                foodPositions.push_back(i);
            }

            else if(s.array[i] == STRATEGIC)
            {
               strategyPositions.push_back(i);
            }

            else if (s.array[i] == EMPTY)
            {
                emptyPositions.push_back(i);
            }

            else if (s.array[i] == SIMPLE )
            {
                simplePositions.push_back(i);
            }
        }
        return STAY;
    }
}


