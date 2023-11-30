#ifndef PADDLE_H
#define PADDLE_H

#include "../game/gameObject.h"

class Paddle : public GameObject {
public:
    float speed;
    float baseSpeed;
    int points=0;
    bool player=true;
    const float ballSpeedUp=20.f;
    const float trackOffset=5.f;
    const float speedOffset=30.f;
    sf::Text* scoreText;
    GameObject* ballRef;

    Paddle(std::string id, sf::Texture* texture, sf::Vector2f pos, bool isPlayer, sf::Text* txt, float speed);
    void OnTriggerEnter(PhysicsBody&) override;
    void Update(float) override;
    void UpdateAI(float);
    void SetBallRef(GameObject* ball) {ballRef=ball;}
};

#endif