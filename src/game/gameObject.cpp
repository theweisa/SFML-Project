#include "gameObject.h"

GameObject::GameObject(std::string newId, sf::Texture* objectTexture) : id(newId) {
    //set the animations sprite to the texture
    texture = *objectTexture;
	sprite.setTexture(*objectTexture, true);
}
GameObject::GameObject(std::string newId, sf::Texture* objectTexture, sf::Vector2f pxScale) : id(newId) {
    //set the animations sprite to the texture
    texture = *objectTexture;
	sprite.setTexture(*objectTexture, true);
    sprite.setScale(sf::Vector2f(pxScale.x/sprite.getLocalBounds().width, pxScale.y/sprite.getLocalBounds().height));
}
GameObject::GameObject(std::string newId, Animation* defaultAnim) : id(newId) {
    animations["default"] = defaultAnim;
}
GameObject::GameObject(const GameObject& rhObj) {
    this->id = rhObj.id;
    this->sprite = rhObj.sprite;
    this->animations = rhObj.animations;
}

GameObject::~GameObject() {
    for (auto itr = animations.begin(); itr != animations.end(); itr++) {
        delete itr->second;
    }
}

void GameObject::AddAnimation(std::string key, sf::Texture & spriteSheet, float newSpeed, int startFrameX, int startFrameY, int endFramesX, int endFramesY, int newWidth, int newHeight)
{
    animations[key] = new Animation(sprite, spriteSheet, newSpeed, startFrameX, startFrameY, endFramesX, endFramesY, newWidth, newHeight);
}

//check if a certain animation has reached the last frame
const bool GameObject::GetEndOfAnimation(const std::string key)
{
    return animations[key]->getEndOfAnimation();
}

void GameObject::Render(sf::RenderTarget& target) {
    target.draw(sprite);
}

void GameObject::Update(float deltaTime) {
    
}

void GameObject::SetPosition(sf::Vector2f newPos) {
    sprite.setPosition(newPos);
}

void GameObject::SetPosition(float newX, float newY) {
    sprite.setPosition(newX, newY);
}
void GameObject::SetTexture(sf::Texture* txtr, sf::Vector2f pxScale) {
    SetTexture(txtr);
    sprite.setScale(sf::Vector2f(pxScale.x/sprite.getLocalBounds().width, pxScale.y/sprite.getLocalBounds().height));
}

void GameObject::SetTexture(const sf::Texture * txtr) {
    texture = *txtr;
    sprite.setTexture(*txtr);
}

void GameObject::SetTexture(sf::Texture* txtr) {
    texture = *txtr;
    sprite.setTexture(*txtr);
}

const bool GameObject::InsideBounds(sf::Vector2f pos) {
    return (sprite.getGlobalBounds().contains(pos));
}