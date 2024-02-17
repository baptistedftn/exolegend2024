#include "gladiator.h"
#include "trajectory.hpp"
#include "foe.hpp"
#include "mazeResolver.hpp"
#include "killerQueen.hpp"
#include <string.h>

using namespace std;

Gladiator *gladiator;

Trajectory *trajectory;
Foe *foe;
KillerQueen *killerQueen;
MazeResolver *mazeResolver;

MazeSquare *last = nullptr;

void reset();
void setup()
{
    gladiator = new Gladiator();
    trajectory = new Trajectory(gladiator);
    foe = new Foe(gladiator);
    killerQueen = new KillerQueen(gladiator);
    mazeResolver = new MazeResolver(gladiator);

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
        mazeResolver->run(trajectory);
        trajectory->run();
    }
    else
    {
        gladiator->log("[Exo 3D] - Waiting");
    }
    delay(10);
}