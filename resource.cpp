#include "Resource.h"
#include "Agent.h"

namespace Gaming
{
    const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

    Resource::Resource(const Game &g, const Position &p, double capacity) :Piece(g, p)
    {
        __capacity = capacity;
    }

    Resource::~Resource()
    {
    }

    double Resource::consume()
    {
        double capacity = getCapacity();
        __capacity = 0;
        finish();
        return capacity;
    }

    void Resource::age()
    {
        if(__capacity < 0.001)
        {
            __capacity = 0.0;
        }
        __capacity /= RESOURCE_SPOIL_FACTOR;
    }

    ActionType Resource::takeTurn(const Surroundings &s) const
    {
        return STAY;
    }

    Piece &Resource::operator*(Piece &other)
    {
        return *this;
    }

    Piece &Resource::interact(Agent *other)
    {
        return *this;
    }

    Piece &Resource::interact(Resource *)
    {
        return *this;
    }

}
