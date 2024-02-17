#include "gladiator.h"
#include "utils.hpp"
#include "trajectory.hpp"

class MazeResolver
{
private:
    Gladiator *gladiator;

    unsigned char selfID;

public:
    MazeResolver(Gladiator *gladiator)
    {
        this->gladiator = gladiator;
        this->selfID = gladiator->robot->getData().id;
    }

    void run(Trajectory *trajectory)
    {
        const MazeSquare *nearestSquare = gladiator->maze->getNearestSquare();

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
                float distance = sqrt((newSquare[i].x - trajectory->center.x) * (newSquare[i].x - trajectory->center.x) +
                                      (newSquare[i].y - trajectory->center.y) * (newSquare[i].y - trajectory->center.y));

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
    }
};