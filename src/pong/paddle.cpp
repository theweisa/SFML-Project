#include "paddle.h"

Paddle::Paddle(std::string id, sf::Texture* texture, sf::Vector2f pos, sf::Text* txt, float _speed) 
: GameObject(id, texture, pos), scoreText(txt), speed(_speed) {

}

void Paddle::OnTriggerEnter(PhysicsBody& coll) {
    if (coll.host->id != "ball") return;
    std::cout << "collision!" << std::endl;
    sf::Vector2f dir = body->Normalize(coll.host->GetPosition() - GetPosition());
    coll.SetVelocity((coll.GetVelocityMagnitude()+ballSpeedUp)*dir);
}

void Paddle::Update(float deltaTime) {
    if (!player) {
        // ai stuff here
    }
    GameObject::Update(deltaTime);
}