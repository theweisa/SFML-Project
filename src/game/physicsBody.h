#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <unordered_map>

class GameObject;
#include "gameObject.h"

class PhysicsBody {
public:
    enum BodyType { Dynamic, Static, Kinematic };
    GameObject* host;
    int layer;
    std::unordered_map<int, bool> interactableLayers;
    bool trigger = true;
    sf::FloatRect hitbox;
    sf::Vector2f hitboxOffset;
    sf::Vector2f baseHitboxDimensions;
    sf::Vector2f baseHitboxPosition;
    BodyType type;
    sf::Vector2f velocity;
    float linearDrag;
    
    PhysicsBody(GameObject* host, BodyType type, sf::FloatRect& hitbox);
    ~PhysicsBody() {};
    virtual void Update(float);
    const sf::FloatRect GetGlobalHitbox() const;
    virtual void SetVelocity(sf::Vector2f);
    virtual void SetSpeed(float);
    virtual void SetDirection(sf::Vector2f);
    virtual void SetDirection(float);
    sf::Vector2f Normalize(sf::Vector2f);
    virtual float GetVelocityMagnitude();
    virtual sf::Vector2f GetDirection();
    
    
};

#endif