#include "AggressiveAgentStrategy.h"
#include "Game.h"

namespace Gaming
{
    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD =
                                            Game::STARTING_AGENT_ENERGY * 0.75;

    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy)
    {
        __agentEnergy = agentEnergy;
    }

    AggressiveAgentStrategy::~AggressiveAgentStrategy()
    {
    }

    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const
    {
        std::vector<int> agentPositions;
        std::vector<int> advantagePositions;
        std::vector<int> foodPositions;
        std::vector<int> strategyPositions;
        std::vector<int> emptyPositions;

        for (int i = 0; i < 9; i++)
        {
            if (__agentEnergy >= DEFAULT_AGGRESSION_THRESHOLD && (s.array[i] == SIMPLE || s.array[i] == STRATEGIC))
            {
                agentPositions.push_back(i);
            }

            if (s.array[i] == ADVANTAGE)
            {
                advantagePositions.push_back(i);
            }

            else if (s.array[i] == FOOD)
            {
                foodPositions.push_back(i);
            }

            else if (s.array[i] == STRATEGIC)
            {
                strategyPositions.push_back(i);
            }

            else if (s.array[i] == EMPTY)
            {
                emptyPositions.push_back(i);
            }
        }

        return STAY;
    }
}
