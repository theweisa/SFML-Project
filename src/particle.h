#ifndef PARTICLE_H
#define PARTICLE_H

#include "game/gameObject.h"

class Particle : public GameObject {
    public:
        float lifetime;
        float lifetimeTimer;
        float speed;
        sf::Vector2f direction;
    
};

#endif