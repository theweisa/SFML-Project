#include "particle.h"

Particle::Particle(float life, float speed, sf::Vector2f dir, float radius) 
: lifetime(life), lifetimeTimer(life), speed(speed), direction(dir) {
    shape = new sf::CircleShape(100);
    shape->setPosition(sf::Vector2f(0,0));
    shape->setFillColor(sf::Color::White);
}
Particle::~Particle() {
    delete shape;
}

void Particle::Update(float deltaTime) {
    UpdateTimer(deltaTime);
}

void Particle::UpdateTimer(float deltaTime) {
    lifetimeTimer -= deltaTime;
}

bool Particle::DeleteParticle() {
    return lifetimeTimer <= 0;
}

void Particle::Render(sf::RenderTarget& target) {
    std::cout << "render" << std::endl;
    target.draw(*shape);
}
