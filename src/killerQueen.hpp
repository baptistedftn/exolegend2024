#include "gladiator.h"
#include "utils.hpp"

class KillerQueen
{
private:
    Gladiator *gladiator;
    // uint8_t posServo = 45;

public:
    KillerQueen(Gladiator *gladiator)
    {
        this->gladiator = gladiator;
        gladiator->weapon->initWeapon(WeaponPin::M1, WeaponMode::SERVO);
    }

    void tryKill(void)
    {
        gladiator->weapon->setTarget(WeaponPin::M1, 60);
        delay(800);
        gladiator->weapon->setTarget(WeaponPin::M1, 120);
        delay(800);
    }

    void zero(void)
    {
        gladiator->weapon->setTarget(WeaponPin::M1, 90);
    }
};