#include "gameObject.h"

GameObject::GameObject() {
    localPosition = sf::Vector2f(0.f,0.f);
    body = nullptr;
}
GameObject::GameObject(std::string id, sf::Texture* objectTexture, sf::Vector2f pos) {
    this->id = id;
    //set the animations sprite to the texture
    texture = *objectTexture;
	sprite.setTexture(*objectTexture, true);
    sprite.setPosition(pos);
    sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().width/2.f, sprite.getGlobalBounds().height/2.f));
    scale = sf::Vector2f(1,1);
    body = nullptr;
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
    delete body;
}

void GameObject::AddPhysicsBody(PhysicsBody* body) {
    this->body = body;
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
    if (!active) return;
    target.draw(sprite);
}

void GameObject::RenderHitbox(sf::RenderTarget& target) {
    if (body == nullptr) return;
    sf::RectangleShape rect(sf::Vector2f(body->hitbox.width, body->hitbox.height));
    rect.setFillColor(sf::Color(255, 255, 255, 100));
    rect.setPosition(sf::Vector2f(GetPosition().x-(body->hitbox.left), GetPosition().y-(body->hitbox.top)));
    target.draw(rect);
}

void GameObject::Update(float deltaTime) {
    UpdatePhysicsBody(deltaTime);
}

void GameObject::UpdatePhysicsBody(float deltaTime) {
    if (body == nullptr || !active) return;
    body->Update(deltaTime);
}

void GameObject::SetScale(sf::Vector2f newScale) {
    sf::Vector2f prevScale = scale;
    scale = newScale;
    sprite.setScale(newScale);
    //sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().width/2.f, sprite.getGlobalBounds().height/2.f));
    if (body != nullptr) {
        body->hitbox.width = body->baseHitboxDimensions.x * newScale.x;
        body->hitbox.height = body->baseHitboxDimensions.y * newScale.y;
        body->hitbox.left = body->baseHitboxPosition.x + (body->hitbox.width-body->baseHitboxDimensions.x)/2.f;
        body->hitbox.top = body->baseHitboxPosition.y + (body->hitbox.height-body->baseHitboxDimensions.y)/2.f;
        body->radius = body->baseRadius * newScale.x;
    }
}

void GameObject::SetPosition(sf::Vector2f newPos) {
    sprite.setPosition(newPos+localPosition);
    if (body == nullptr) return;
    body->currentPos = newPos;
    // body->prevPos = newPos;
}

void GameObject::SetPosition(float newX, float newY) {
    SetPosition(sf::Vector2f(newX, newY));
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
    //SetSpriteDimensions(dimensions);
}

void GameObject::SetTexture(sf::Texture* txtr) {
    texture = *txtr;
    sprite.setTexture(*txtr, true);
    //SetSpriteDimensions(dimensions);
}

const bool GameObject::InsideBounds(sf::Vector2f pos) {
    return (sprite.getGlobalBounds().contains(pos));
}