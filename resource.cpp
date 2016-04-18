#include "Resource.h"
#include "Agent.h"

namespace Gaming
{
    const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

    Resource::Resource(const Game &g, const Position &p, double capacity) :Piece(g, p)
    {
        this -> __capacity = __capacity;
    }

    Resource::~Resource()
    {
    }

    double Resource::consume()
    {
        // consume()
        double consume = 10.0;
        return consume;
    }

    void Resource::age()
    {
        if(__capacity < 0.000001)
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