#include "gladiator.h"
#include "trajectory.hpp"
#include "foe.hpp"
#include <killerQueen.hpp>
#include <string.h>

using namespace std;

Gladiator *gladiator;

Trajectory *trajectory;
Foe *foe;
KillerQueen *killerQueen;

MazeSquare *last = nullptr;

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
}

void loop()
{
    if (gladiator->game->isStarted())
    {
        gladiator->log("[Exo 3D] - Goooooooo !");
        if (gladiator->weapon->canLaunchRocket())
        {
            gladiator->weapon->launchRocket();
        }
        if (foe->panikMode())
        {
            trajectory->esquive(RIGHT);
            killerQueen->tryKill();
        }
        if (trajectory->isOutside())
        {
            gladiator->log("Alerte générale!!!");
        }

        const MazeSquare *nearestSquare = gladiator->maze->getNearestSquare();
        Position myPosition = gladiator->robot->getData().position;
        RobotData data = gladiator->robot->getData();
        unsigned char selfID = data.teamId;

        MazeSquare *neighbors[4] = {nearestSquare->northSquare, nearestSquare->westSquare,
                                    nearestSquare->eastSquare, nearestSquare->southSquare};
        Position squareCenter[4];
        float squareSize = gladiator->maze->getSquareSize();
        for (int i = 0; i < 4; ++i)
        {
            if (neighbors[i] != nullptr && neighbors[i]->possession != selfID)
            {
                // squareCenter[i].x = (neighbors[i]->i + 0.5) * squareSize;
                squareCenter[i].x = (neighbors[i]->i) * squareSize;
                squareCenter[i].y = (neighbors[i]->j) * squareSize;
            }
        }

        int minIndex = -1;
        float seuil = std::numeric_limits<float>::max();
        for (int i = 0; i < 4; ++i)
        {
            if (neighbors[i] != nullptr && neighbors[i]->possession != selfID)
            {
                float distance = sqrt((squareCenter[i].x - trajectory->center.x) * (squareCenter[i].x - trajectory->center.x) +
                                      (squareCenter[i].y - trajectory->center.y) * (squareCenter[i].y - trajectory->center.y));
                if (distance < seuil)
                {
                    seuil = distance;
                    minIndex = i;
                }
            }
        }

        if (minIndex != -1)
        {
            Position goal{squareCenter[minIndex].x, squareCenter[minIndex].y, 0};
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