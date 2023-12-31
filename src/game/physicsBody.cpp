#include "physicsBody.h"

PhysicsBody::PhysicsBody(GameObject* _host, BodyType _type, sf::FloatRect& _hitbox) 
: host(_host), type(_type), hitbox(_hitbox) 
{
    baseHitboxDimensions = sf::Vector2f(_hitbox.width, _hitbox.height);
    baseHitboxPosition = sf::Vector2f(_hitbox.left, _hitbox.top);
    velocity = sf::Vector2f(0.f,0.f);
    trigger = false;
    linearDrag = 0;
}

void PhysicsBody::Update(float deltaTime) {
    if (host == nullptr || (type == Static || type == Kinematic)) return;
    host->sprite.move(velocity.x*deltaTime, velocity.y*deltaTime);
}

const sf::FloatRect PhysicsBody::GetGlobalHitbox() const {
    return host->sprite.getTransform().transformRect(hitbox);
}

void PhysicsBody::SetSpeed(float newSpeed) {
    velocity = GetDirection() * newSpeed;
}
void PhysicsBody::SetDirection(float angle) {
    float speed = GetVelocityMagnitude();
    angle *= 3.14159265 / 180.f;
    velocity = speed * sf::Vector2f(cos(angle), sin(angle));
}
void PhysicsBody::SetDirection(sf::Vector2f newDir) {
    float speed = GetVelocityMagnitude();
    velocity = Normalize(newDir) * speed;
}

void PhysicsBody::SetVelocity(sf::Vector2f newVelocity) {
    velocity = newVelocity;
}
float PhysicsBody::GetVelocityMagnitude() {
    return std::abs(std::hypot(velocity.x, velocity.y));
}
sf::Vector2f PhysicsBody::GetDirection() {
    return Normalize(velocity);
}

sf::Vector2f PhysicsBody::Normalize(sf::Vector2f vec) {
    float mag = std::hypot(vec.x, vec.y);
    if (mag == 0) {
        vec = sf::Vector2f(0, 0);
        return vec;
    }
    vec /= mag;
    return vec;
}