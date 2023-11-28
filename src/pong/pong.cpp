#include "pong.h"

Pong::Pong() {
    Init();
}

void Pong::InitGame() {
    Game::InitGame();
    GameObject * ball = new GameObject(assets["ball"], sf::Vector2f(windowWidth/2, windowHeight/2));
    ball->id = "ball";
    ball->AddPhysicsBody(new PhysicsBody(ball, PhysicsBody::Dynamic, sf::FloatRect(8, 8, 16, 16)));
    ball->body->SetVelocity(sf::Vector2f(20, 100));
    gameObjects["ball"].push_back(ball);
    
    line[0].position = sf::Vector2f(windowWidth/2.f, 0);
    line[1].position = sf::Vector2f(windowWidth/2.f, windowHeight);

    //playerOne = new GameObject()
}

void Pong::InitAssets() {
    Game::InitAssets();
    AddAsset("ball", "textures/pong_ball.png");
}

void Pong::Render() {
    Game::Render();
    for (auto& obj : gameObjects["ball"]) {
        obj->Render(*window);
    }
    
}

void Pong::Update() {
    Game::Update();
}

void Pong::OnScreenExit(GameObject& obj) {
    if (obj.id == "ball") {
        if (obj.GetPosition().y < 0.f || obj.GetPosition().y >= windowHeight) {
            obj.body->SetDirection(sf::Vector2f(obj.body->GetDirection().x, obj.body->GetDirection().y * -1));
	    }
        //left wall
        if (obj.GetPosition().x < 0.f) {
            std::cout << "p2 wins" << std::endl;
        }
        //right wall
        else if (obj.GetPosition().x >= windowWidth) {
            std::cout << "p1 wins" << std::endl;
        }
    }
    else if (obj.id == "player") {

    }
}