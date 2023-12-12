#include "suika.h"

Suika::Suika() {
    Init();
}

void Suika::InitGame() {
    Game::InitGame();
    GameObject* obj = Instantiate("balls", "circle", sf::Vector2f(windowWidth/2.f, windowHeight/2.f), PhysicsBody::Static);
    obj->SetScale(sf::Vector2f(3.f, 3.f));
}

void Suika::InitAssets() {
    Game::InitAssets();
    AddAsset("circle", "textures/circle.png");
}