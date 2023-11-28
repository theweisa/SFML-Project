#include "pong.h"

Pong::Pong() {
    Game::Game();
    //gameObjects["ball"].push_back(new GameObject())
}

void Pong::InitAssets() {
    Game::InitAssets();
    AddAsset("ball", "textures/pong_ball.png");
}

void Pong::Update() {
    Game::Update();
}