#include "Advantage.h"

namespace Gaming
{
    const char Advantage::ADVANTAGE_ID = 'D';
    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

    Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity)
    {
        __capacity *= ADVANTAGE_MULT_FACTOR;
    }

    Advantage::~Advantage()
    {
    }

    void Advantage::print(std::ostream &os) const
    {
        os << ADVANTAGE_ID << __id;
    }

    double Advantage::getCapacity() const
    {
        return __capacity;
    }

    double Advantage::consume()
    {
        double capacity = getCapacity();
        __capacity = 0;
        finish();
        return capacity;
    }
}