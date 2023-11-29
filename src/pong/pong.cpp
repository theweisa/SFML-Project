#include "pong.h"

Pong::Pong() {
    Init();
}

void Pong::InitGame() {
    Game::InitGame();
    CreateBall();
    text["p1 score"] = new sf::Text("0", font, 36);
    text["p1 score"]->setPosition(windowWidth*0.35f, windowHeight*0.1f);
    text["p2 score"] = new sf::Text("0", font, 36);
    text["p2 score"]->setPosition(windowWidth*0.65f, windowHeight*0.1f);

    ball->body->SetVelocity(sf::Vector2f(1, 1));
    ball->body->SetDirection(0.f);
    ball->body->SetSpeed(baseBallSpeed);
    
    line = sf::VertexArray(sf::Lines, 2);
    line[0].position = sf::Vector2f(windowWidth/2.f, 0);
    line[1].position = sf::Vector2f(windowWidth/2.f, windowHeight);

    playerOne = new Paddle("playerOne", assets["ball"], sf::Vector2f(windowWidth*0.2f, windowHeight/2), text["p1 score"], 100.f);
    sf::FloatRect playerOneHitbox = sf::FloatRect(4, 4, 8, 8);
    playerOne->AddPhysicsBody(new PhysicsBody(playerOne, PhysicsBody::Dynamic, playerOneHitbox));
    playerOne->SetScale(sf::Vector2f(1.2, 4));
    gameObjects["players"].push_back(playerOne);

    playerTwo = new Paddle("playerTwo", assets["ball"], sf::Vector2f(windowWidth*0.8f, windowHeight/2), text["p2 score"], 100.f);
    sf::FloatRect playerTwoHitbox = sf::FloatRect(4, 4, 8, 8);
    playerTwo->AddPhysicsBody(new PhysicsBody(playerTwo, PhysicsBody::Dynamic, playerTwoHitbox));
    playerTwo->SetScale(sf::Vector2f(1.2, 4));
    gameObjects["players"].push_back(playerTwo);
}

void Pong::InitAssets() {
    Game::InitAssets();
    AddAsset("ball", "textures/pong_ball.png");
    AddFont("font", "fonts/Minecraft.ttf");
}

void Pong::CreateBall() {
    ball = new GameObject("ball", assets["ball"], sf::Vector2f(windowWidth/2, windowHeight/2));
    sf::FloatRect ballHitbox = sf::FloatRect(4, 4, 8, 8);
    ball->AddPhysicsBody(new PhysicsBody(ball, PhysicsBody::Dynamic, ballHitbox));
    //ball->body->SetVelocity(sf::Vector2f(-100, -10));
    gameObjects["ball"].push_back(ball);
}

void Pong::Render() {
    Game::Render();
    RenderGameObjects();
    RenderText();
    window->draw(line);
}

void Pong::PlayerScore(Paddle* winningPlayer) {
    delete ball;
    ball = nullptr;
    gameObjects["ball"].clear();
    winningPlayer->points += 1;
    winningPlayer->scoreText->setString(std::to_string(winningPlayer->points));
    if (winningPlayer->points >= winningPoints) {
        gameState = GameOver;
    }
    else {
        gameState = Score;
    }
}

void Pong::Update() {
    Game::Update();
    switch (gameState) {
        case Start:
            break;
        case Play:
            break;
        case Score:
            ScoreRoutine();
            break;
        case GameOver:
            WinRoutine();
            break;
        default:
            break;
    }
}

void Pong::ScoreRoutine() {
    routineTimer += deltaTime;
    switch (routineStep) {
        case 0: {
            if (routineTimer < 2) break;
            CreateBall();
            for (auto& obj : gameObjects["players"]) {
                obj->SetPosition(sf::Vector2f(obj->GetPosition().x, windowHeight/2.f));
            }
            routineTimer = 0;
            routineStep++;
            break;
        }
        case 1: {
            if (routineTimer < 1) break;
            ball->body->SetVelocity(sf::Vector2f(1, 1));
            ball->body->SetDirection(0.f);
            ball->body->SetSpeed(baseBallSpeed);
        }
        default:
            gameState = Play;
            FinishRoutine();
            break;
    }
}

void Pong::WinRoutine() {
    routineTimer += deltaTime;
    switch (routineStep) {
        
    }
    FinishRoutine();
}

void Pong::FinishRoutine() {
    routineTimer = 0;
    routineStep = 0;
}

void Pong::UpdateInputs() {
    Game::UpdateInputs();
    if (gameState != Play) {
        playerOne->body->SetVelocity(sf::Vector2f(0,0));
        playerTwo->body->SetVelocity(sf::Vector2f(0,0));
        return;
    }
    if (playerOne->player) {
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
    if (playerTwo->player) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            playerTwo->body->SetVelocity(playerTwo->speed*sf::Vector2f(0,-1));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            playerTwo->body->SetVelocity(playerTwo->speed*sf::Vector2f(0,1));
        }
        else {
            playerTwo->body->SetVelocity(sf::Vector2f(0,0));
        }
    }
}

void Pong::OnScreenExit(GameObject& obj) {
    if (obj.id == "ball") {
        if (obj.GetPosition().y < 0.f || obj.GetPosition().y >= windowHeight) {
            obj.body->SetDirection(sf::Vector2f(obj.body->GetDirection().x, obj.body->GetDirection().y * -1));
	    }
        //left wall
        if (obj.GetPosition().x < 0.f) {
            PlayerScore(playerTwo);            
        }
        //right wall
        else if (obj.GetPosition().x >= windowWidth) {
            PlayerScore(playerOne);
        }
    }
    else {
        obj.SetPosition(
            obj.GetPosition().x,
            std::clamp(obj.GetPosition().y, obj.sprite.getGlobalBounds().height/2.f, windowHeight-obj.sprite.getGlobalBounds().height/2.f)
        );
    }
}