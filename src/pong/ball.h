#ifndef BALL_H
#define BALL_H

#include "../game/gameObject.h"
#include "pong.h"

class Ball : public GameObject {
public:
    Pong gameRef;
    sf::Vector2f direction;
    float speed;

    Ball(Pong ref);
    void Move();
};

#endif