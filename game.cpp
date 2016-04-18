#include "Game.h"
#include "Simple.h"
#include "Strategic.h"
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
    PositionRandomizer Game::__posRandomizer = Gaming::PositionRandomizer();


    void Game::populate()
    {
        //populate();
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
                    __grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);
                    numStrategic --;
                }
            }

            while (numSimple > 0)
            {
                int i = d(gen);
                if (__grid[i] == nullptr)
                {
                    Position pos(i / __width, i % __width);
                    __grid[i] = new Simple(*this, pos, Game::STARTING_AGENT_ENERGY);
                    numSimple --;
                }
            }

            while (numAdvantages > 0)
            {
                int i = d(gen);
                if (__grid[i] == nullptr)
                {
                    Position pos(i / __width, i % __width);
                    __grid[i] = new Advantage(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
                    numAdvantages --;
                }
            }

            while (numFoods > 0)
            {
                int i = d(gen);
                if (__grid[i] == nullptr)
                {
                    Position pos(i / __width, i % __width);
                    __grid[i] = new Food(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
                    numFoods --;
                }
            }
        }
        else
        {
            __numInitResources = 0;
            __numInitAgents = 0;
        }
    }

    Game::Game(const Game &another)
    {
    }

    Game::~Game()
    {
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr)
                delete __grid[i];
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
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr && (__grid[i]->getType() == SIMPLE || __grid[i]->getType() == STRATEGIC))
                pieces++;
        }
        return pieces;
    }

    unsigned int Game::getNumSimple() const
    {
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr && __grid[i]->getType() == SIMPLE)
                pieces++;
        }
        return pieces;
    }


    unsigned int Game::getNumStrategic() const
    {
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++)
        {
            if (__grid[i] != nullptr && __grid[i]->getType() == STRATEGIC)
                pieces++;
        }
        return pieces;
    }

    unsigned int Game::getNumResources() const
    {
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++){
            if (__grid[i] != nullptr && (__grid[i]->getType() == FOOD || __grid[i]->getType() == ADVANTAGE))
                pieces++;
        }
        return pieces;
    }

    const Piece *Game::getPiece(unsigned int x, unsigned int y) const
    {

        int location = x +(y* __width);
        return __grid[location];
    }

    void Game::addSimple(const Position &position)
    {
        if((position.x < 0 && position.x > getWidth()) || (position.y < 0 && position.y > getHeight()))
        {
            throw OutOfBoundsEx(getWidth(),getHeight(),position.x,position.y);
        }

        int location = position.x + (position.y * __width);
        __grid[location] = new Simple(*this, position,STARTING_AGENT_ENERGY);

    }

    void Game::addSimple(const Position &position, double energy)
    {
        if((position.x < 0 && position.x > getWidth()) || (position.y < 0 && position.y > getHeight()))
        {
            throw OutOfBoundsEx(getWidth(),getHeight(),position.x,position.y);
        }

        int location = position.x + (position.y * __width);
        __grid[location] = new Simple(*this, position,energy);
    }

    void Game::addSimple(unsigned x, unsigned y)
    {
        if((x < 0 && x > getWidth()) || (y < 0 && y > getHeight()))
        {
            throw OutOfBoundsEx(getWidth(),getHeight(),x,y);
        }

        Position pos(x,y);
        int location = x + (y * __width);
        __grid[location] = new Simple(*this, pos,STARTING_AGENT_ENERGY);
    }

    void Game::addSimple(unsigned x, unsigned y, double energy)
    {
        if((x < 0 && x > getWidth()) || (y < 0 && y > getHeight()))
        {
            throw OutOfBoundsEx(getWidth(),getHeight(),x,y);
        }

        Position pos(x,y);
        int location = x + (y * __width);
        __grid[location] = new Simple(*this, pos,energy);
    }

    void Game::addStrategic(const Position &position, Strategy *s)
    {
        int location = position.x + (position.y * __width);
        __grid[location] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
    }

    void Game::addStrategic(unsigned x, unsigned y, Strategy *s)
    {
        Position pos(x,y);
        int location = x + (y * __width);
        __grid[location] = new Strategic(*this, pos, STARTING_AGENT_ENERGY,s);
    }

    void Game::addFood(const Position &position)
    {
        int location = position.x + (position.y * __width);
        __grid[location] = new Food(*this, position,STARTING_RESOURCE_CAPACITY);
    }

    void Game::addFood(unsigned x, unsigned y)
    {
        Position pos(x,y);
        int location = x + (y * __width);
        __grid[location] = new Food(*this, pos,STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(const Position &position)
    {
        int location = position.x + (position.y * __width);
        __grid[location] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(unsigned x, unsigned y)
    {
        Position pos(x,y);
        int location = x +(y *__width);
        __grid[location] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
    }

    const Surroundings Game::getSurroundings(const Position &pos) const
    {
        Surroundings surroundings;
        return surroundings;
    }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to)
    {
        return STAY;
    }

    bool Game::isLegal(const ActionType &ac, const Position &pos) const
    {
        Surroundings surr = getSurroundings(pos);
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
        if (surr.array[direction] != INACCESSIBLE)
            return true;
        return false;
    }

    const Position Game::move(const Position &pos, const ActionType &ac) const
    {
        Position p(pos);
        return p;
    }

    void Game::round()
    {
        // round()

        if (getNumResources() == 0)
        {
            __status = OVER;
        }


    }

    void Game::play(bool verbose)
    {
        while (getStatus() != OVER)
        {
            if (verbose)
            {
                std::cout << *this;
            }

            round();
        }
    }

    std::ostream &operator<<(std::ostream &os, const Game &game)
    {
        os << "Status: " << game.__status << std::endl;
    }

}
