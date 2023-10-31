#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "game/Game.h"
#include "particle.h"

/*
random size, directions, velocities, and lifetimes where the player clicks
*/
class ParticleEmitter : public Game {
    public:
        ParticleEmitter();
        void Render() override;
};

#endif