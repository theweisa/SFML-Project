#include "pong.h"

Pong::Pong() {
    Init();
}

void Pong::InitGame() {
    Game::InitGame();
    ShowMenu();
}

void Pong::StartGame(bool isPlayerOne, bool isPlayerTwo) {
    gameState = Score;
    ClearText();
    
    text["p1 score"] = new sf::Text("0", font, 42);
    text["p1 score"]->setPosition(windowWidth*0.4f, windowHeight*0.1f);
    CenterText(text["p1 score"]);
    text["p2 score"] = new sf::Text("0", font, 42);
    text["p2 score"]->setPosition(windowWidth*0.6f, windowHeight*0.1f);
    CenterText(text["p2 score"]);

    line = sf::VertexArray(sf::Lines, 2);
    line[0].position = sf::Vector2f(windowWidth/2.f, 0);
    line[1].position = sf::Vector2f(windowWidth/2.f, windowHeight);

    playerOne = new Paddle("playerOne", assets["ball"], sf::Vector2f(windowWidth*0.2f, windowHeight/2), isPlayerOne, text["p1 score"], 150.f);
    sf::FloatRect playerOneHitbox = sf::FloatRect(4, 4, 8, 8);
    playerOne->AddPhysicsBody(new PhysicsBody(playerOne, PhysicsBody::Dynamic, playerOneHitbox));
    playerOne->SetScale(sf::Vector2f(1.2, 4));
    gameObjects["players"].push_back(playerOne);

    playerTwo = new Paddle("playerTwo", assets["ball"], sf::Vector2f(windowWidth*0.8f, windowHeight/2), isPlayerTwo, text["p2 score"], 150.f);
    sf::FloatRect playerTwoHitbox = sf::FloatRect(4, 4, 8, 8);
    playerTwo->AddPhysicsBody(new PhysicsBody(playerTwo, PhysicsBody::Dynamic, playerTwoHitbox));
    playerTwo->SetScale(sf::Vector2f(1.2, 4));
    gameObjects["players"].push_back(playerTwo);

    if (RandomRange(0, 1) == 0) sendLeft = true;
    else sendLeft = false;
}

void Pong::UpdateDeltaTime() {
    Game::UpdateDeltaTime();
    if (gameState == Pause) {
        deltaTime = 0;
    }
}

void Pong::RandomizeBallDirection() {
    float angle = RandomRange(-20.f, 20.f);
    if (sendLeft) {
        angle += 180;
    }
    ball->body->SetDirection(angle);
}

void Pong::TogglePause(bool state) {
    if (state) {
        text["pause"] = new sf::Text("PAUSED", font, 64);
        text["pause"]->setPosition(windowWidth*0.5f, windowHeight*0.4f);
        CenterText(text["pause"]);
        previousState = gameState;
        gameState = Pause;
    }
    else {
        DeleteText("pause");
        gameState = previousState;
    }

}

void Pong::ShowMenu() {
    gameState = PreStart;
    DeleteAllGameObjects();
    ClearText();
    line.clear();
    text["pong"] = new sf::Text("PONG", font, 72);
    text["pong"]->setPosition(windowWidth*0.5f, windowHeight*0.3f);
    CenterText(text["pong"]);

    text["start"] = new sf::Text("ANY BUTTON TO START", font, 32);
    text["start"]->setPosition(windowWidth*0.5f, windowHeight*0.6f);
    CenterText(text["start"]);
}

void Pong::StartMenu() {
    gameState = Start;
    DeleteText("start");

    text["1p"] = new sf::Text("'1' for 1 Player", font, 28);
    text["1p"]->setPosition(windowWidth*0.5f, windowHeight*0.65f);
    CenterText(text["1p"]);

    text["2p"] = new sf::Text("'2' for 2 Player", font, 28);
    text["2p"]->setPosition(windowWidth*0.5f, windowHeight*0.75f);
    CenterText(text["2p"]);
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
    sendLeft = winningPlayer != playerOne;
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

void Pong::UpdateGameObjects() {
    if (gameState == Pause) return;
    Game::UpdateGameObjects();
}

void Pong::UpdatePollEvents() {
    while (window->pollEvent(ev)) {
		switch (ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                switch(gameState) {
                    case PreStart:
                        StartMenu();
                    case Play:
                    case Score:
                        if (ev.key.code == sf::Keyboard::Escape || ev.key.code == sf::Keyboard::P) {
                            TogglePause(true);
                        }
                        break;
                    case Pause:
                        if (ev.key.code == sf::Keyboard::Escape || ev.key.code == sf::Keyboard::P) {
                            TogglePause(false);
                        }
                        break;
                    default:
                        break;
                }
        }
    }
}

void Pong::ScoreRoutine() {
    routineTimer += deltaTime;
    switch (routineStep) {
        case 0: {
            for (auto player : gameObjects["players"]) {
                sf::Color c = dynamic_cast<Paddle*>(player)->scoreText->getFillColor();
                c.a = 0;
                dynamic_cast<Paddle*>(player)->scoreText->setFillColor(c);
            }
            routineTimer = 0;
            routineStep++;
            break;
        }
        case 1: {
            if (routineTimer < 0.3) break;
            for (auto player : gameObjects["players"]) {
                sf::Color c = dynamic_cast<Paddle*>(player)->scoreText->getFillColor();
                c.a = 255;
                dynamic_cast<Paddle*>(player)->scoreText->setFillColor(c);
            }
            routineTimer = 0;
            routineStep++;
            break;
        }
        case 2: {
            if (routineTimer < 1) break;
            CreateBall();
            for (auto& obj : gameObjects["players"]) {
                obj->SetPosition(sf::Vector2f(obj->GetPosition().x, windowHeight/2.f));
            }
            routineTimer = 0;
            routineStep++;
            break;
        }
        case 3: {
            if (routineTimer < 1) break;
            ball->body->SetVelocity(sf::Vector2f(1, 1));
            RandomizeBallDirection();
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
            CenterText(text["win text"]);
            routineStep++;
            break;
        }
        case 1: {
            if (routineTimer < 1) break;
            text["menu text"] = new sf::Text("Press 'M' to go to the Menu", font, 48);
            text["menu text"]->setPosition(windowWidth/2, windowHeight*0.6);
            CenterText(text["menu text"]);
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                StartGame(true, false);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                StartGame(true, true);
            }
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
                ShowMenu();
                //std::cout << "to main menu!" << std::endl;
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