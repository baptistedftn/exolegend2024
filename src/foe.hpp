#include "gladiator.h"
#include <vector>
using namespace std;

// Structure pour représenter un ennemi avec sa distance par rapport au robot actuel
struct Enemy
{
    unsigned char id;       // ID du robot ennemi
    double distanceToRobot; // distance entre le robot ennemi et le robot actuel
};

class Foe
{
private:
    Gladiator *gladiator;

    vector<Enemy> enemiesWithDistance;

    // Fonction pour calculer la distance entre deux positions
    double distance(const Position &p1, const Position &p2)
    {
        return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    }

    // Fonction pour trier les ennemis par ordre croissant de distance par rapport au robot
    bool sortByDistance(const Enemy &e1, const Enemy &e2)
    {
        return e1.distanceToRobot < e2.distanceToRobot;
    }

public:
    Foe(Gladiator *gladiator)
    {
        this->gladiator = gladiator;
    }

    bool panikMode()
    {
        return false;
    }

    vector<Enemy> getEnemies(void)
    {
        return enemiesWithDistance;
    }
};