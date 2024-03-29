#include "gladiator.h"
#include "utils.hpp"
#include <cmath>
#include <chrono>

#if !defined(TRAJ)
#define TRAJ

using namespace std;

enum DIRECTION
{
    RIGHT,
    LEFT
};

class Trajectory
{
private:
    Gladiator *gladiator;

    float moduloPi(float a)
    {
        return (a < 0.0) ? (fmod(a - M_PI, 2 * M_PI) + M_PI) : (fmod(a + M_PI, 2 * M_PI) - M_PI);
    }

    double reductionAngle(double x)
    {
        x = fmod(x + PI, 2 * PI);
        if (x < 0)
            x += 2 * PI;
        return x - PI;
    }

    double initialMazeSize;
    int currentReduction;
    double squareSize;

    float kw = 1.2;
    float kv = 1.f;
    float wlimit = 3.f;
    float vlimit = 0.6;
    float erreurPos = 0.07;
    float kForward = 1.5;
    float kAngular = 2.;

    Position target;

public:
    Position center{1.5, 1.5, 0};
    Position fakeTarget = {0, 2, 0};

    Trajectory(Gladiator *gladiator)
    {
        this->gladiator = gladiator;
        this->target = {gladiator->robot->getData().position.x, gladiator->robot->getData().position.y};
        this->currentReduction = 0;
        this->squareSize = gladiator->maze->getSquareSize();
    }

    void setTarget(Position target)
    {
        this->target = target;
    }

    void run(void)
    {
        Position pos = gladiator->robot->getData().position;
        double dx = target.x - pos.x;
        double dy = target.y - pos.y;
        double d = sqrt(dx * dx + dy * dy);

        if (d > erreurPos)
        {
            double rho = atan2(dy, dx);
            double consw = kw * reductionAngle(rho - pos.a);
            double consv = kv * d * cos(reductionAngle(rho - pos.a));

            consw = min((max((float)consw, -wlimit)), wlimit) * kAngular;
            consv = min((max((float)consv, -vlimit)), vlimit) * kForward;

            double consvl = consv - gladiator->robot->getRobotRadius() * consw;
            double consvr = consv + gladiator->robot->getRobotRadius() * consw;

            if (consvl == .0 && consvr == .0)
            {
                consvr = wlimit;
            }

            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, consvr, false);
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, consvl, false);
        }
        else
        {
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0, false);
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0, false);
        }
    }
    bool isBloqued()
    {
        return (gladiator->control->getWheelSpeed(WheelAxis::LEFT) <= 0.01 && gladiator->control->getWheelSpeed(WheelAxis::RIGHT) <= 0.01);
    }

    bool rotate(double alpha)
    {
        double currentAngle = gladiator->robot->getData().position.a / (PI / 2);
        gladiator->log("%f", currentAngle);
        if (currentAngle > alpha + .1)
        {
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, .1);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -.1);
        }
        else if (currentAngle < alpha - .1)
        {
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, -.1);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, .1);
        }
        else
        {
            return true;
        }
    }

    bool isOutside(std::chrono::_V2::steady_clock::time_point startTime)
    {
        auto end = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - startTime).count();
        this->currentReduction = elapsedSeconds / 20;
        gladiator->log("Réductions = %d blocs", currentReduction);
        Position currentPosition = gladiator->robot->getData().cposition; // position en m
        if ((currentReduction * squareSize) < currentPosition.x < (12 - (currentReduction * squareSize)))
        {
            return false;
        }
        else if ((currentReduction * squareSize) < currentPosition.y < (12 - (currentReduction * squareSize)))
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void esquive(DIRECTION direction)
    {
        gladiator->log("Esquive de la loutre !!!");
        switch (direction)
        {
        case DIRECTION::LEFT:
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, 1, true);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -1, true);
            break;
        case DIRECTION::RIGHT:
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, -1, true);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 1, true);
            break;
        default:
            break;
        }
    }
};

#endif