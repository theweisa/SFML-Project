#include "paddle.h"

Paddle::Paddle(std::string id, sf::Texture* texture, sf::Vector2f pos, float _speed) 
: GameObject(id, texture, pos), speed(_speed) { }

void Paddle::OnTriggerEnter(PhysicsBody& coll) {
    if (coll.host->id != "ball") return;
    std::cout << "collision!" << std::endl;
    sf::Vector2f dir = body->Normalize(coll.host->GetPosition() - GetPosition());
    coll.SetVelocity(coll.GetVelocityMagnitude()*dir);
}