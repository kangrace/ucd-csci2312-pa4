#include "Game.h"
#include "Strategic.h"
#include "Simple.h"
#include "Food.h"
#include "Advantage.h"

namespace Gaming
{
    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;

    PositionRandomizer Game::__posRandomizer = PositionRandomizer();

    void Game::populate()
    {
        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;
        unsigned int numStrategic = __numInitAgents / 2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantages = __numInitResources / 4;
        unsigned int numFoods = __numInitResources - numAdvantages;

        std::default_random_engine gen;
        std::uniform_int_distribution<int> d(0, __width * __height);

        while (numStrategic > 0)
        {
            int i = d(gen);
            if (__grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = nullptr;
                __grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);

                numStrategic--;
            }
        }

        while (numSimple > 0)
        {
            int i = d(gen);
            if (__grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = nullptr;
                __grid[i] = new Simple(*this, pos, Game::STARTING_AGENT_ENERGY);
                numSimple--;
            }
        }

        while (numFoods > 0)
        {
            int i = d(gen);
            if (__grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numFoods--;
            }
        }

        while (numAdvantages > 0)
        {
            int i = d(gen);
            if (__grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
                numAdvantages--;
            }
        }
    }

    Game::Game() : __grid(MIN_WIDTH * MIN_HEIGHT, nullptr)
    {
        __numInitAgents = 0;
        __numInitResources = 0;
        __width = MIN_WIDTH;
        __height = MIN_HEIGHT;
        __round = 0;
        __status = NOT_STARTED;
        __verbose = false;
    }

    Game::Game(unsigned width, unsigned height, bool manual) : __grid(width * height, nullptr)
    {
        if (width < MIN_WIDTH || height < MIN_HEIGHT)
        {
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }

        __width = width;
        __height = height;
        __round = 0;
        __status = NOT_STARTED;
        __verbose = false;

        if (!manual)
        {
            populate();
        }
    }


    Game::~Game()
    {
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr)
            {
                delete __grid[i];
            }
        }
    }


    unsigned int Game::getNumPieces() const
    {
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr)
                pieces++;
        }
        return pieces;
    }

    unsigned int Game::getNumAgents() const
    {
        unsigned int agent = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr && (__grid[i]->getType() == SIMPLE || __grid[i]->getType() == STRATEGIC))
                agent++;
        }
        return agent;
    }


    unsigned int Game::getNumSimple() const
    {
        unsigned int simple = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr && __grid[i]->getType() == SIMPLE)
                simple++;
        }
        return simple;
    }

    unsigned int Game::getNumStrategic() const
    {
        unsigned int strategy = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr && __grid[i]->getType() == STRATEGIC)
                strategy++;
        }
        return strategy;
    }

    unsigned int Game::getNumResources() const
    {
        unsigned int resource = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr && (__grid[i]->getType() == FOOD || __grid[i]->getType() == ADVANTAGE))
                resource++;
        }
        return resource;
    }

    const Piece *Game::getPiece(unsigned int x, unsigned int y) const
    {
        if (y >= __width || x >= __height)
        {
            throw OutOfBoundsEx(__width, __height, x, y);
        }

        if (__grid[x * __width + y] == nullptr)
        {
            throw PositionEmptyEx(x, y);
        }

        int location = x * __width + y;
        return __grid[location];
    }

    void Game::addSimple(const Position &position)
    {

        if (position.x >= __height || position.y >= __width)
        {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }

        if (__grid[position.x * __width + position.y])
        {
            throw PositionNonemptyEx(position.x, position.y);
        }

        int location = position.x * __width + position.y;
        __grid[location] = new Simple(*this, position, STARTING_AGENT_ENERGY);

    }

    void Game::addSimple(const Position &position, double energy)
    {

        if (position.x >= __height || position.y >= __width)
        {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }

        if (__grid[position.x * __width + position.y])
        {
            throw PositionNonemptyEx(position.x, position.y);
        }

        int location = position.x * __width + position.y;
        __grid[location] = new Simple(*this, position, energy);
    }


    void Game::addSimple(unsigned x, unsigned y)
    {
        Position p(x, y);
        this->addSimple(p);
    }

    void Game::addSimple(unsigned x, unsigned y, double energy)
    {
        Position p(x, y);
        this->addSimple(p, energy);
    }


    void Game::addStrategic(const Position &position, Strategy *s)
    {
        if ((position.x * __width + position.y) > __grid.size())
        {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }

        if ((__grid[position.x * __width + position.y]) != nullptr)
        {
            throw PositionNonemptyEx(position.x, position.y);
        }

        int location = position.x * __width + position.y;
        __grid[location] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
    }

    void Game::addStrategic(unsigned x, unsigned y, Strategy *s)
    {
        Position p(x, y);
        this->addStrategic(p, s);
    }

    void Game::addFood(const Position &position)
    {
        if ((position.x * __width + position.y) > __grid.size())
        {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }

        if ((__grid[position.x * __width + position.y]) != nullptr)
        {
            throw PositionNonemptyEx(position.x, position.y);
        }

        int location = position.x * __width + position.y;
        __grid[location] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);

    }


    void Game::addFood(unsigned x, unsigned y)
    {
        Position p(x, y);
        this->addFood(p);
    }

    void Game::addAdvantage(const Position &position)
    {
        if ((position.x * __width + position.y) > __grid.size())
        {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }

        if (__grid[position.x * __width + position.y] != nullptr)
        {
            throw PositionNonemptyEx(position.x, position.y);
        }

        int location = position.x * __width + position.y;
        __grid[location] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
    }


    void Game::addAdvantage(unsigned x, unsigned y)
    {
        Position p(x, y);
        this->addAdvantage(p);
    }


    const Surroundings Game::getSurroundings(const Position &pos) const
    {

        Surroundings temp;
        int i = 0;
        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                if ((pos.x + x) < 0 || (pos.y + y) < 0 || (pos.x + x) > (__height - 1) || (pos.y + y) > (__width - 1))
                    temp.array[i] = INACCESSIBLE;

                else if (y == 0 && x == 0)
                    temp.array[i] = SELF;

                else if (__grid[(pos.x + x) * __width + (pos.y + y)] == nullptr)
                    temp.array[i] = EMPTY;

                else
                    temp.array[i] = __grid[(pos.x + x) * __width + (pos.y + y)]->getType();
                i++;
            }
        }
        return temp;
    }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to)
    {
        if ((from.x + 1) < to.x || (from.y + 1) < to.y || (from.x - 1) > to.x || (from.y - 1) > to.y)
            return STAY;
        else
        {
            if (to.x > from.x)
            {
                if (to.y > from.y)
                    return SE;
                else if (to.y < from.y)
                    return SW;
                else
                    return S;
            }

            else if (to.x < from.x)
            {
                if (to.y > from.y)
                    return NE;
                else if (to.y < from.y)
                    return NW;
                else
                    return N;
            }

            else
            {
                if (to.y > from.y)
                    return E;
                else if (to.y < from.y)
                    return W;
                else
                    return STAY;
            }
        }
    }


    bool Game::isLegal(const ActionType &ac, const Position &pos) const
    {
        Surroundings s = getSurroundings(pos);
        int direction;
        switch (ac)
        {
            case N:
                direction = 1;
                break;
            case NE:
                direction = 2;
                break;
            case NW:
                direction = 0;
                break;
            case E:
                direction = 5;
                break;
            case W:
                direction = 3;
                break;
            case SE:
                direction = 8;
                break;
            case SW:
                direction = 6;
                break;
            case S:
                direction = 7;
                break;
            case STAY:
                direction = 4;
        }
        if (s.array[direction] != INACCESSIBLE)
            return true;
        return false;
    }


    const Position Game::move(const Position &pos, const ActionType &ac) const
    {
        if (isLegal(ac, pos))
        {
            Position p(pos);
            switch (ac)
            {
                case N:
                    p.x -= 1;
                    break;
                case NE:
                    p.x -= 1;
                    p.y += 1;
                    break;
                case NW:
                    p.x -= 1;
                    p.y -= 1;
                    break;
                case E:
                    p.y += 1;
                    break;
                case W:
                    p.y -= 1;
                    break;
                case SE:
                    p.x += 1;
                    p.y += 1;
                    break;
                case SW:
                    p.x += 1;
                    p.y -= 1;
                    break;
                case S:
                    p.x += 1;
                    break;
            }
            return p;
        }
    }

    void Game::round()
    {
        if (__verbose && __round == 0)
            __status = Status::PLAYING;

        for (int count = 0; count < __grid.size(); ++count)
        {
            if (__grid[count] != nullptr && !__grid[count]->getTurned() && __grid[count]->isViable())
            {

                Agent *a = dynamic_cast<Agent *>(__grid[count]);

                if (a)
                {
                    __grid[count]->setTurned(true);

                    Position p = __grid[count]->getPosition();
                    Surroundings s = getSurroundings(p);
                    ActionType act = __grid[count]->takeTurn(s);


                    if (act != STAY)
                    {
                        Position newp = move(p, act);
                        int i = (newp.x * __width + newp.y);

                        (*__grid[count]) * (*__grid[i]);


                        if (!__grid[count]->isViable())
                        {
                            delete __grid[count];
                            __grid[count] = nullptr;
                        }

                        else
                        {
                            __grid[count]->setPosition(newp);

                            if (__grid[i] == nullptr)
                            {
                                __grid[i] = __grid[count];
                                __grid[count] = nullptr;
                            }

                            else
                            {
                                delete __grid[i];
                                __grid[i] = __grid[count];
                                __grid[count] = nullptr;

                            }
                        }

                        if (!__grid[i]->isViable())
                        {
                            delete __grid[i];
                            __grid[i] = nullptr;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < __grid.size(); ++i)
        {
            if (__grid[i] != nullptr)
            {
                if (__grid[i]->isViable())
                {
                    __grid[i]->setTurned(false);
                    __grid[i]->age();

                }

                else
                {
                    delete __grid[i];
                    __grid[i] = nullptr;
                }
            }
        }

        if (getNumResources() < 1)
        {
            __status = Status::OVER;
        }

        __round++;
    }



    void Game::play(bool verbose)
    {
        while (getStatus() != OVER)
        {
            if (verbose)
                std::cout << *this;
            round();
        }
    }

    std::ostream &operator<<(std::ostream &os, const Game &game)
    {
        os << "Round " << game.__round << std::endl;
        for (int h = 0; h < game.__height; h++)
        {
            for (int w = 0; w < game.__width; w++)
            {
                os << "[";
                if (game.__grid[h * game.__width + w] != nullptr)
                    os << *(game.__grid[h * game.__width + w]);
                else
                    os << "     ";
                os << "]";
            }
            os << std::endl;
        }
        os << "Status: " << game.__status << std::endl;
    }


}