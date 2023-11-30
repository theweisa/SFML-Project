#include "paddle.h"

Paddle::Paddle(std::string id, sf::Texture* texture, sf::Vector2f pos, bool isPlayer, sf::Text* txt, float _speed) 
: GameObject(id, texture, pos), player(isPlayer), scoreText(txt), baseSpeed(_speed), speed(_speed) {
    
}

void Paddle::OnTriggerEnter(PhysicsBody& coll) {
    if (coll.host->id != "ball") return;
    std::cout << "collision!" << std::endl;
    // change move speed on coll for AIs
    if (!player) {
        float speedOff = -speedOffset + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(speedOffset-(-speedOffset))));
        speed = baseSpeed + speedOff;
        std::cout << "new speed: " << speed << std::endl;
    }
    sf::Vector2f dir = body->Normalize(coll.host->GetPosition() - GetPosition());
    coll.SetVelocity((coll.GetVelocityMagnitude()+ballSpeedUp)*dir);
}

void Paddle::Update(float deltaTime) {
    //Game *game = Game::GetInstance();
    //std::cout << game->windowWidth << std::endl;
    if (!player) {
        UpdateAI(deltaTime);
    }
    GameObject::Update(deltaTime);
}

void Paddle::UpdateAI(float deltaTime) {
    // ai stuff here
    if (ballRef == nullptr) return;
    if (ballRef->GetPosition().y < GetPosition().y-trackOffset) {
        body->SetVelocity(sf::Vector2f(0, -speed));
    }
    else if (ballRef->GetPosition().y > GetPosition().y+trackOffset) {
        body->SetVelocity(sf::Vector2f(0, speed));
    }
    else {
        body->SetVelocity(sf::Vector2f(0, 0));
    }
}