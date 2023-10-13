#include "gameObject.h"

GameObject::GameObject() {
    localPosition = sf::Vector2f(0.f,0.f);
}
GameObject::GameObject(std::string newId, sf::Texture* objectTexture, sf::Vector2f pos) : id(newId) {
    //set the animations sprite to the texture
    texture = *objectTexture;
	sprite.setTexture(*objectTexture, true);
    sprite.setPosition(pos);
    //SetSpriteDimensions(pxScale);
}
GameObject::GameObject(const GameObject& rhObj) {
    this->id = rhObj.id;
    this->texture = rhObj.texture;
    this->sprite = rhObj.sprite;
    this->dimensions = rhObj.dimensions;
    this->scale = rhObj.scale;
    this->animations = rhObj.animations;
    this->localPosition = rhObj.localPosition;
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
    sprite.setPosition(newPos+localPosition);
}

void GameObject::SetPosition(float newX, float newY) {
    sprite.setPosition(newX+localPosition.x, newY+localPosition.y);
}

void GameObject::SetSpriteDimensions(sf::Vector2f pxScale) {
    //sprite.rect
    dimensions = pxScale;
    scale = sf::Vector2f(pxScale.x/sprite.getLocalBounds().width, pxScale.y/sprite.getLocalBounds().height);
    sprite.setScale(scale);
}

void GameObject::SetLocalPosition(sf::Vector2f newLocal) {
    localPosition = newLocal;
    SetPosition(GetPosition()+localPosition);
}

void GameObject::SetTexture(sf::Texture* txtr, sf::Vector2f pxScale) {
    SetTexture(txtr);
    sprite.setScale(sf::Vector2f(pxScale.x/sprite.getLocalBounds().width, pxScale.y/sprite.getLocalBounds().height));
}

void GameObject::SetTexture(const sf::Texture * txtr) {
    texture = *txtr;
    sprite.setTexture(*txtr, true);
    SetSpriteDimensions(dimensions);
}

void GameObject::SetTexture(sf::Texture* txtr) {
    texture = *txtr;
    sprite.setTexture(*txtr, true);
    SetSpriteDimensions(dimensions);
}

const bool GameObject::InsideBounds(sf::Vector2f pos) {
    return (sprite.getGlobalBounds().contains(pos));
}