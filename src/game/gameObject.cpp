#include "gameObject.h"

GameObject::GameObject(sf::Texture objectTexture) {
    std::cout << "construct game object w/ texture" << std::endl;
    //set the animations sprite to the texture
	sprite.setTexture(objectTexture, true);
}
GameObject::GameObject(sf::Texture objectTexture, sf::Vector2f pxScale) {
    //set the animations sprite to the texture
    std::cout << "construct game object w/ pixelsize" << std::endl;
	sprite.setTexture(objectTexture, true);
    sprite.setScale(sf::Vector2f(pxScale.x/sprite.getLocalBounds().width, pxScale.y/sprite.getLocalBounds().height));
}
GameObject::GameObject(Animation* defaultAnim) {
    animations["default"] = defaultAnim;
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

void GameObject::SetPosition(sf::Vector2f newPos) {
    sprite.setPosition(newPos);
}

void GameObject::SetPosition(float newX, float newY) {
    sprite.setPosition(newX, newY);
}