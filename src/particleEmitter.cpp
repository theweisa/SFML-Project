#include "particleEmitter.h"

ParticleEmitter::ParticleEmitter() {
    float randAngle = (rand() % 361) * 3.141592654f / 180.f;
    sf::Vector2f randDir = sf::Vector2f(cos(randAngle), sin(randAngle));
    //float randLife = rand() % 4;
    float randLife = 10000.f;
    float randSpeed = rand() % 10;
    float randSize = rand() % 4;
    std::cout << "lmao" << std::endl;
    
    gameObjects["particles"].push_back(new Particle(GetMousePosView(), randLife, randSpeed, randDir, randSize));
}

void ParticleEmitter::Render() {
    Game::Render();
    for (auto& p : gameObjects["particles"]) {
        p->Render(*window);
    }
}