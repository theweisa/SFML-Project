#include "physicsBody.h"

PhysicsBody::PhysicsBody(GameObject* _host, BodyType _type, sf::FloatRect& _hitbox) 
: host(_host), type(_type), hitbox(_hitbox) 
{
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
    sf::Vector2f dir = GetDirection();
    velocity = speed * sf::Vector2f(
        dir.x * (cos(angle)) + dir.y * (sin(angle)),
        dir.y * (cos(angle)) - dir.x * (sin(angle))
    );
}
void PhysicsBody::SetDirection(sf::Vector2f newDir) {
    float speed = GetVelocityMagnitude();
    velocity = Normalize(newDir) * speed;
}

void PhysicsBody::SetVelocity(sf::Vector2f newVelocity) {
    velocity = newVelocity;
}
float PhysicsBody::GetVelocityMagnitude() {
    return std::hypot(velocity.x, velocity.y);
}
sf::Vector2f PhysicsBody::GetDirection() {
    sf::Vector2f dir = velocity;
    return Normalize(dir);
}

sf::Vector2f PhysicsBody::Normalize(sf::Vector2f& vec) {
    float mag = std::hypot(vec.x, vec.y);
    if (mag == 0) {
        vec = sf::Vector2f(0, 0);
        return vec;
    }
    vec /= mag;
    return vec;
}