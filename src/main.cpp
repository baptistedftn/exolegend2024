#include "gladiator.h"
#include "trajectory.hpp"
#include "foe.hpp"
#include <killerQueen.hpp>
#include <string>
#include <chrono>

using namespace std;

Gladiator *gladiator;

Trajectory *trajectory;
Foe *foe;
KillerQueen *killerQueen;

MazeSquare *last = nullptr;

auto start = std::chrono::steady_clock::now();

void reset();
void setup()
{
    gladiator = new Gladiator();
    trajectory = new Trajectory(gladiator);
    foe = new Foe(gladiator);
    killerQueen = new KillerQueen(gladiator);

    gladiator->game->onReset(&reset);
}

void reset()
{
    gladiator->log("Call of reset function");
    killerQueen->zero();
    start = std::chrono::steady_clock::now();
}

void loop()
{
    if (gladiator->game->isStarted())
    {
        // gladiator->log("[Exo 3D] - Goooooooo !");
        if (gladiator->weapon->canLaunchRocket())
        {
            gladiator->weapon->launchRocket();
        }
        // if (foe->panikMode())
        // {
        //     trajectory->esquive(RIGHT);
        // }
        if (trajectory->isOutside(start))
        {
            trajectory->setTarget(trajectory->center);
            gladiator->log("Alerte générale!!! On rentre au bercail");
        }
        killerQueen->tryKill();

        if (trajectory->isBloqued())
        {
            trajectory->setTarget(trajectory->center);
        }

        const MazeSquare *nearestSquare = gladiator->maze->getNearestSquare();
        RobotData data = gladiator->robot->getData();
        unsigned char selfID = data.teamId;

        MazeSquare *arroundSquare[4] = {nearestSquare->northSquare, nearestSquare->westSquare,
                                        nearestSquare->eastSquare, nearestSquare->southSquare};
        Position newSquare[4];
        float squareSize = gladiator->maze->getSquareSize();
        for (int i = 0; i < 4; ++i)
        {
            if (arroundSquare[i] != nullptr && arroundSquare[i]->possession != selfID)
            {
                newSquare[i].x = (arroundSquare[i]->i + .5) * squareSize;
                newSquare[i].y = (arroundSquare[i]->j + .5) * squareSize;
            }
        }

        int minIndex = -1;
        float seuil = std::numeric_limits<float>::max();
        for (int i = 0; i < 4; ++i)
        {
            if (arroundSquare[i] != nullptr && arroundSquare[i]->possession != selfID)
            {
                float distance = sqrt((newSquare[i].x - trajectory->fakeTarget.x) * (newSquare[i].x - trajectory->fakeTarget.x) +
                                      (newSquare[i].y - trajectory->fakeTarget.y) * (newSquare[i].y - trajectory->fakeTarget.y));

                if (distance < seuil)
                {
                    seuil = distance;
                    minIndex = i;
                }
            }
        }

        if (minIndex != -1)
        {
            Position goal{newSquare[minIndex].x, newSquare[minIndex].y, 0};
            trajectory->setTarget(goal);
        }
        trajectory->run();
    }
    else
    {
        gladiator->log("[Exo 3D] - Waiting");
    }
    delay(10);
}