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
        gladiator->weapon->setTarget(WeaponPin::M1, 30);
        delay(100);
        gladiator->weapon->setTarget(WeaponPin::M1, 90);
        delay(100);
    }

    void zero(void)
    {
        gladiator->weapon->setTarget(WeaponPin::M1, 90);
    }
};