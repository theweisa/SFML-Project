#ifndef PADDLE_H
#define PADDLE_H

#include "../game/gameObject.h"

class Paddle : public GameObject {
public:
    float speed;
    int points=0;
    bool player=true;
    const float ballSpeedUp=20.f;
    sf::Text* scoreText;

    Paddle(std::string id, sf::Texture* texture, sf::Vector2f pos, sf::Text* txt, float speed);
    void OnTriggerEnter(PhysicsBody&) override;
    void Update(float) override;
};

#endif