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
        return other.interact(this);
    }

    Piece& Agent::interact(Agent *other)
    {
        if (this->getEnergy() == other->getEnergy())
        {
            this->finish();
            other->finish();
        }

        else if (this->getEnergy() > other->getEnergy())
        {
            this->addEnergy(-(other->getEnergy()));
            other->finish();
        }

        else
        {
            other->addEnergy(-(this->getEnergy()));
            this->finish();
        }

        return *this;
    }

    Piece& Agent::interact(Resource *other)
    {
        other -> interact(this);
        return *this;
    }

}