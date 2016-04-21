#include "Agent.h"
#include "Resource.h"


namespace Gaming
{
    const double Agent::AGENT_FATIGUE_RATE = 0.3;

    Agent::Agent(const Game &g, const Position &p, double energy) : Piece(g, p)
    {
        __energy = energy;
    }

    Agent::~Agent()
    {
    }

    void Agent::age()
    {
        __energy -= AGENT_FATIGUE_RATE;
    }

    Piece& Agent::operator*(Piece &other)
    {
        return *this;
    }

    Piece& Agent::interact(Agent *other)
    {
        other->interact(this);
        return *this;
    }

    Piece& Agent::interact(Resource *other)
    {
        other->interact(this);
        return *this;
    }

}
