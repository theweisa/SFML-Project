#ifndef PARTICLE_H
#define PARTICLE_H

#include "game/gameObject.h"

class Particle : public GameObject {
    public:
        float lifetime;
        float lifetimeTimer;
        float speed;
        sf::Vector2f direction;

        sf::CircleShape * shape;

        Particle(sf::Vector2f, float, float, sf::Vector2f, float);
        ~Particle();

        void Update(float) override;
        void UpdatePosition(float);
        void UpdateSpeed(float);
        void UpdateTimer(float);
        bool DeleteParticle();

        void Render(sf::RenderTarget&) override;
};

#endif