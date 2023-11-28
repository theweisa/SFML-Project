#include "pong.h"

Pong::Pong() {
    Init();
}

void Pong::InitGame() {
    Game::InitGame();
    GameObject * ball = new GameObject("ball", assets["ball"], sf::Vector2f(windowWidth/2, windowHeight/2));
    ball->AddPhysicsBody(new PhysicsBody(ball, PhysicsBody::Dynamic, sf::FloatRect(4, 4, 8, 8)));
    ball->body->SetVelocity(sf::Vector2f(-100, -10));
    gameObjects["ball"].push_back(ball);
    
    line = sf::VertexArray(sf::Lines, 2);
    line[0].position = sf::Vector2f(windowWidth/2.f, 0);
    line[1].position = sf::Vector2f(windowWidth/2.f, windowHeight);

    playerOne = new Paddle("playerOne", assets["ball"], sf::Vector2f(windowWidth*0.2f, windowHeight/2), 50.f);
    playerOne->AddPhysicsBody(new PhysicsBody(playerOne, PhysicsBody::Dynamic, sf::FloatRect(4, 4, 8, 8)));
    playerOne->SetScale(sf::Vector2f(2, 8));

    gameObjects["players"].push_back(playerOne);

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
        //obj->RenderHitbox(*window);
    }
    playerOne->Render(*window);
    //playerOne->RenderHitbox(*window);
    window->draw(line);
}

void Pong::Update() {
    Game::Update();
}

void Pong::UpdateInputs() {
    Game::UpdateInputs();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        playerOne->body->SetVelocity(playerOne->speed*sf::Vector2f(0,-1));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        playerOne->body->SetVelocity(playerOne->speed*sf::Vector2f(0,1));
    }
    else {
        playerOne->body->SetVelocity(sf::Vector2f(0,0));
    }
}

void Pong::OnScreenExit(GameObject& obj) {
    if (obj.id == "ball") {
        if (obj.GetPosition().y < 0.f || obj.GetPosition().y >= windowHeight) {
            obj.body->SetDirection(sf::Vector2f(obj.body->GetDirection().x, obj.body->GetDirection().y * -1));
	    }
        //left wall
        if (obj.GetPosition().x < 0.f) {
            //std::cout << "p2 wins" << std::endl;
        }
        //right wall
        else if (obj.GetPosition().x >= windowWidth) {
            //std::cout << "p1 wins" << std::endl;
        }
    }
    else if (obj.id == "player") {

    }
}