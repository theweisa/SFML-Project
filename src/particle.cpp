#include "particle.h"

Particle::Particle(sf::Vector2f pos, float life, float speed, sf::Vector2f dir, float radius) 
: lifetime(life), lifetimeTimer(life), speed(speed), direction(dir) {
    shape = new sf::CircleShape(radius);
    shape->setPosition(pos);
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
    target.draw(*shape);
}
