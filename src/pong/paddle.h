#ifndef PADDLE_H
#define PADDLE_H

#include "../game/gameObject.h"

class Paddle : public GameObject {
public:
    float speed;

    Paddle(std::string id, sf::Texture* texture, sf::Vector2f pos, float speed);
    void OnTriggerEnter(PhysicsBody&) override;
};

#endif