#include "gameObject.h"

GameObject::GameObject(sf::Texture objectTexture) {

}
GameObject::GameObject(Animation* defaultAnim) {
    
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