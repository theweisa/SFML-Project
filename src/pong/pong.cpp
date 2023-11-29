#include "pong.h"

Pong::Pong() {
    Init();
}

void Pong::InitGame() {
    Game::InitGame();
    StartGame();
    gameState = Play;
    
}

void Pong::StartGame() {
    gameState = Play;
    text["p1 score"] = new sf::Text("0", font, 42);
    text["p1 score"]->setPosition(windowWidth*0.4f, windowHeight*0.1f);
    text["p1 score"]->setOrigin(text["p1 score"]->getLocalBounds().width/2, text["p1 score"]->getLocalBounds().height/2);
    text["p2 score"] = new sf::Text("0", font, 42);
    text["p2 score"]->setPosition(windowWidth*0.6f, windowHeight*0.1f);
    text["p2 score"]->setOrigin(text["p2 score"]->getLocalBounds().width/2, text["p2 score"]->getLocalBounds().height/2);
    
    line = sf::VertexArray(sf::Lines, 2);
    line[0].position = sf::Vector2f(windowWidth/2.f, 0);
    line[1].position = sf::Vector2f(windowWidth/2.f, windowHeight);

    playerOne = new Paddle("playerOne", assets["ball"], sf::Vector2f(windowWidth*0.2f, windowHeight/2), true, text["p1 score"], 150.f);
    sf::FloatRect playerOneHitbox = sf::FloatRect(4, 4, 8, 8);
    playerOne->AddPhysicsBody(new PhysicsBody(playerOne, PhysicsBody::Dynamic, playerOneHitbox));
    playerOne->SetScale(sf::Vector2f(1.2, 4));
    gameObjects["players"].push_back(playerOne);

    playerTwo = new Paddle("playerTwo", assets["ball"], sf::Vector2f(windowWidth*0.8f, windowHeight/2), false, text["p2 score"], 150.f);
    sf::FloatRect playerTwoHitbox = sf::FloatRect(4, 4, 8, 8);
    playerTwo->AddPhysicsBody(new PhysicsBody(playerTwo, PhysicsBody::Dynamic, playerTwoHitbox));
    playerTwo->SetScale(sf::Vector2f(1.2, 4));
    gameObjects["players"].push_back(playerTwo);

    CreateBall();
    ball->body->SetVelocity(sf::Vector2f(1, 1));
    ball->body->SetDirection(0.f);
    ball->body->SetSpeed(baseBallSpeed);
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
    playerOne->SetBallRef(ball);
    playerTwo->SetBallRef(ball);
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
    playerOne->SetBallRef(nullptr);
    playerTwo->SetBallRef(nullptr);
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
        case 0: {
            std::string winner = "P1";
            if (playerTwo->points >= winningPoints) winner = "P2";
            text["win text"] = new sf::Text(winner + " Wins!", font, 64);
            text["win text"]->setPosition(windowWidth/2, windowHeight*0.4);
            text["win text"]->setOrigin(text["win text"]->getLocalBounds().width/2, text["win text"]->getLocalBounds().height/2);
            routineStep++;
            break;
        }
        case 1: {
            if (routineTimer < 1) break;
            text["menu text"] = new sf::Text("Press 'M' to go to the Menu", font, 48);
            text["menu text"]->setPosition(windowWidth/2, windowHeight*0.6);
            text["menu text"]->setOrigin(text["menu text"]->getLocalBounds().width/2, text["menu text"]->getLocalBounds().height/2);
            routineStep++;
            routineTimer = 0;
        }
        default: {
            FinishRoutine();
            break;
        }
    }
}

void Pong::FinishRoutine() {
    routineTimer = 0;
    routineStep = 0;
}

void Pong::UpdateInputs() {
    Game::UpdateInputs();
    switch (gameState) {
        case Start: {
            break;
        }
        case Play: {
            if (playerOne->player) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    playerOne->body->SetVelocity(sf::Vector2f(0,-playerOne->speed));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    playerOne->body->SetVelocity(sf::Vector2f(0,playerOne->speed));
                }
                else {
                    playerOne->body->SetVelocity(sf::Vector2f(0,0));
                }
            }
            if (playerTwo->player) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    playerTwo->body->SetVelocity(sf::Vector2f(0,-playerTwo->speed));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    playerTwo->body->SetVelocity(sf::Vector2f(0,playerTwo->speed));
                }
                else {
                    playerTwo->body->SetVelocity(sf::Vector2f(0,0));
                }
            }
            break;
        }
        case Score: {
            playerOne->body->SetVelocity(sf::Vector2f(0,0));
            playerTwo->body->SetVelocity(sf::Vector2f(0,0));
            break;
        }
        case GameOver: {
            playerOne->body->SetVelocity(sf::Vector2f(0,0));
            playerTwo->body->SetVelocity(sf::Vector2f(0,0));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                std::cout << "to main menu!" << std::endl;
            }
            break;
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