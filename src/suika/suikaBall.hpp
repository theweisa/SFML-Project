#ifndef SUIKA_BALL_H
#define SUIKA_BALL_H

#include "../game/gameObject.h"

class SuikaBall : public GameObject {
public:
    // enum BallType {test1, test2};
    int type;
    int points;
    bool transform = false;
    float maxYVel = 0.3f;
    float loseTimer = 3;
    float baseLoseTimer;
    bool aboveLine = false;

    SuikaBall(std::string id, sf::Texture* texture, sf::Vector2f pos, int _type, int _points) :
        GameObject(id, texture, pos), type(_type), points(_points) 
    {
        sf::FloatRect box = sf::FloatRect(4, 4, 8, 8);
        AddPhysicsBody(new PhysicsBody(this, PhysicsBody::Dynamic, box));
        body->SetBaseRadius(16);
        baseLoseTimer = loseTimer;
    }

    void Update(float deltaTime) override {
        if (aboveLine) {
            loseTimer -= deltaTime;
        }
        else {
            loseTimer = baseLoseTimer;
        }
        GameObject::Update(deltaTime);
    }

    void UpdatePhysicsBody(float deltaTime) override {
        if (body == nullptr || !active) return;
        // stop things from launching upwards
        sf::Vector2f vel = body->currentPos-body->prevPos;
        // std::cout << "velocity: " << vel.y << std::endl;
        if (vel.y < -maxYVel) {
            //std::cout << "above max vel" << std::endl;
            body->currentPos = sf::Vector2f(body->currentPos.x, body->prevPos.y);//+std::min(vel.y, -maxYVel));
        }
        body->Update(deltaTime);
    }

    void OnCollisionEnter(PhysicsBody& coll) override {
        SuikaBall* ball = dynamic_cast<SuikaBall*>(coll.host);
        if (ball != nullptr && ball->type == type) {
            transform = true;
        }
    }
};

#endif