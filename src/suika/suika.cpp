#include "suika.h"

Suika::Suika() {
    Init();
}

void Suika::InitGame() {
    Game::InitGame();
    yLimitPos = windowHeight * 0.25f;
    ballDataTable = {
        {0, BallData(0, "ball0", 0.5, 1)},
        {1, BallData(1, "ball1", 0.91, 3)},
        {2, BallData(2, "ball2", 1.32, 6)},
        {3, BallData(3, "ball3", 1.73, 10)},
        {4, BallData(4, "ball4", 2.14, 15)},
        {5, BallData(5, "ball5", 2.55, 21)},
        {6, BallData(6, "ball6", 2.96, 28)},
        {7, BallData(7, "ball7", 3.37, 36)},
        {8, BallData(8, "ball8", 3.78, 45)},
        {9, BallData(9, "ball9", 4.19, 55)},
        {10, BallData(10, "ball10", 5, 66)}
    };
    ballDropList = {0, 1, 2, 3};
    ballToDrop = RandomRange(0, ballDropList.size()-1);
    nextBallToDrop = RandomRange(0, ballDropList.size()-1);
    GameObject * player = Instantiate("player", "player", sf::Vector2f(0,0));
    player->SetScale(sf::Vector2f(2,2));
    player->sprite.setOrigin(player->sprite.getGlobalBounds().width*0.2, player->sprite.getGlobalBounds().height*0.4);
    //sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().width/2.f, sprite.getGlobalBounds().height/2.f));
    InitConstraints();
    SetDisplaySprites();
    InitText();
    
}

void Suika::InitText() {
    Game::InitText();
    CreateText("POINTS", 24, sf::Vector2f(windowWidth*0.15f, windowHeight*0.4f), "points");
    CreateText("0", 24, sf::Vector2f(windowWidth*0.15f, windowHeight*0.5f), "numPoints");
    CreateText("NEXT", 24, sf::Vector2f(windowWidth*0.83f, windowHeight*0.4f), "next");
    CenterText(text["numPoints"]);
    CenterText(text["points"]);
    CenterText(text["next"]);
}

void Suika::InitConstraints() {
    // 858 x 525
    // 301 x 368
    float boxWidth = windowWidth * 0.35;
    boundingBox = sf::FloatRect((windowWidth-boxWidth)/2, 0, boxWidth, windowHeight*0.95);
    loseLine = sf::VertexArray(sf::Lines, 2);
    loseLine[0].position = sf::Vector2f(boundingBox.getPosition().x, yLimitPos);
    loseLine[1].position = sf::Vector2f(boundingBox.getPosition().x+boxWidth, yLimitPos);
    //GameObject * bg = Instantiate("bg", "container", sf::Vector2f(boundingBox.getPosition().x + boundingBox.width/2, yLimitPos+boundingBox.height*0.4));
    GameObject * bg = Instantiate("bg", "container", sf::Vector2f(boundingBox.getPosition().x, yLimitPos));
    bg->sprite.setOrigin(0,0);
    // sf::FloatRect box = sf::FloatRect(4, 4, 8, 8);
}

void Suika::Render() {
    Game::Render();
    window->draw(loseLine);
}

void Suika::UpdatePollEvents() {
    while (window->pollEvent(ev)) {
		switch (ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::MouseButtonPressed:
                if (dropTimer > 0 || gameOver) break;
                dropTimer = dropInterval;
                gameObjects["balls"].push_back(InstantiateBall(ballToDrop, dropPos));
                ballToDrop = nextBallToDrop;
                nextBallToDrop = RandomRange(0, ballDropList.size()-1);
                SetDisplaySprites();
                break;
            case sf::Event::KeyPressed:
                break;
        }
	}
}

void Suika::Update() {
    if (gameOver) {
        UpdatePollEvents();
        return;
    }
    Game::Update();
    if (!gameObjects["displaySprite"].empty()) {
        gameObjects["displaySprite"][0]->SetPosition(dropPos);
    }
    gameObjects["player"][0]->SetPosition(dropPos);
    if (dropTimer > 0) {
        dropTimer -= deltaTime;
    }
}

void Suika::UpdateMousePos() {
    Game::UpdateMousePos();
    dropPos = sf::Vector2f(
        std::clamp(mousePosView.x, boundingBox.getPosition().x+16*ballDataTable[ballToDrop].scale, boundingBox.getPosition().x+boundingBox.width-16*ballDataTable[ballToDrop].scale),
        windowHeight * 0.25f
    );
}

void Suika::ApplyConstraints(GameObject* obj) {
    if (obj->body == nullptr) return;
    sf::Vector2f objPos = obj->GetPosition();
    SuikaBall* ball = dynamic_cast<SuikaBall*>(obj);
    float radius = obj->body->radius;
    if (ball != nullptr && objPos.y-radius < yLimitPos) {
        ball->aboveLine = true;
        if (ball->loseTimer <= 0) {
            std::cout << "game over!" << std::endl;
            gameOver = true;
        }
    }
    else if (ball != nullptr) {
        ball->aboveLine = false;
    }
    if (objPos.x-radius < boundingBox.getPosition().x) {
        obj->SetPosition(sf::Vector2f(boundingBox.getPosition().x+radius, objPos.y));
        objPos = obj->GetPosition();
        // obj->body->prevPos = sf::Vector2f(obj->GetPosition().x, obj->body->prevPos.y);
    }
    if (objPos.x+radius > boundingBox.getPosition().x+boundingBox.width) {
        obj->SetPosition(sf::Vector2f(boundingBox.getPosition().x+boundingBox.width-radius, objPos.y));
        objPos = obj->GetPosition();
        //obj->body->prevPos = sf::Vector2f(obj->GetPosition().x, obj->body->prevPos.y);
    }
    
    if (objPos.y-radius < boundingBox.getPosition().y) {
        obj->SetPosition(sf::Vector2f(objPos.x, boundingBox.getPosition().y+radius));
        obj->body->prevPos = sf::Vector2f(obj->body->prevPos.x, obj->GetPosition().y);
        objPos = obj->GetPosition();
    }
    if (objPos.y+radius > boundingBox.getPosition().y+boundingBox.height) {
        obj->SetPosition(sf::Vector2f(objPos.x, boundingBox.getPosition().y+boundingBox.height-radius));
        obj->body->prevPos = sf::Vector2f(obj->body->prevPos.x, obj->GetPosition().y);
        objPos = obj->GetPosition();
    }
}

void Suika::SetDisplaySprites() {
    DeleteGameObject("displaySprite", 0);
    DeleteGameObject("nextSprite", 0);
    GameObject * displaySprite = Instantiate("displaySprite", ballDataTable[ballToDrop].assetName, dropPos);
    GameObject * nextSprite = Instantiate("nextSprite", ballDataTable[nextBallToDrop].assetName, sf::Vector2f(windowWidth*0.83f, windowHeight*0.5f));//sf::Vector2f(text["next"]->getPosition().x, text["next"]->getPosition().y+40));
    displaySprite->SetScale(sf::Vector2f(ballDataTable[ballToDrop].scale, ballDataTable[ballToDrop].scale));
    nextSprite->SetScale(sf::Vector2f(ballDataTable[nextBallToDrop].scale, ballDataTable[nextBallToDrop].scale));
}

void Suika::UpdateGameObjects() {
    createdBalls.clear();
    for (auto it = gameObjects.begin(); it != gameObjects.end(); it++) {
        auto itr = it->second.begin();
        while (itr != it->second.end()) {
            GameObject* obj = *itr;
            if (!obj->active) {
                itr = it->second.erase(itr);
                continue;
            }
            obj->Update(deltaTime);
            if (renderGameObjects) obj->Render(*window);
            if (obj->body != nullptr) {
                for (int i = 0; i < updateSteps; i++) {
                    UpdateObjectPhysics(obj);
                }
            }
            if (!obj->active) {
                itr = it->second.erase(itr);
            }
            else {
                itr++;
            }
        }
    }
    for (auto& ball : createdBalls) {
        if (ball == nullptr) continue;
        gameObjects["balls"].push_back(ball);
    }
}

void Suika::UpdateObjectPhysics(GameObject *object) {
    if (object->body == nullptr || !object->active) return;
    object->body->Update(deltaTime);
    for (auto& objVec : gameObjects) {
        auto itr = objVec.second.begin();
        while (itr != objVec.second.end()) {
            GameObject * objTwo = *itr;
            if (object->body == nullptr || objTwo->body == nullptr || object == *itr) {
                itr++;
                continue;
            }
            if (objTwo->body->trigger) {
                if (objTwo->sprite.getGlobalBounds().intersects(object->sprite.getGlobalBounds())) {
                    object->OnTriggerEnter(*(objTwo->body));
                }
            }
            else {
                const sf::Vector2f collAxis = object->GetPosition() - objTwo->GetPosition();
                const float dist = std::hypot(collAxis.x, collAxis.y);
                if (dist < object->body->radius + objTwo->body->radius) {
                    const sf::Vector2 n = collAxis / dist;
                    const float delta = object->body->radius + objTwo->body->radius - dist;
                    const float massRatio = objTwo->body->radius / (object->body->radius + objTwo->body->radius);
                    object->SetPosition(object->GetPosition() + massRatio * delta * n); 
                    object->OnCollisionEnter(*(objTwo->body));
                    SuikaBall* ball = dynamic_cast<SuikaBall*>(object);
                    if (ball != nullptr && ball->transform) {
                        SuikaBall * newBall = MergeBalls(ball, dynamic_cast<SuikaBall*>(objTwo));
                        if (newBall) {
                            createdBalls.push_back(newBall);
                        }
                        objTwo->active = false;
                        object->active = false;
                    }
                }
            } // end else coll
            itr++;
        }// end itr loop
    } // end for loop
    ApplyConstraints(object);
}

SuikaBall * Suika::InstantiateBall(int type, sf::Vector2f pos) {
    SuikaBall* ball = new SuikaBall("yomama", assets[ballDataTable[type].assetName], pos, type, ballDataTable[type].points);
    ball->SetScale(sf::Vector2f(ballDataTable[type].scale, ballDataTable[type].scale));
    return ball;
}

SuikaBall* Suika::MergeBalls(SuikaBall* ballOne, SuikaBall* ballTwo) {
    int type = ballOne->type;
    points += ballDataTable[type].points;
    text["numPoints"]->setString(std::to_string(points));
    if (ballDataTable.find((ballOne->type+1)) == ballDataTable.end()) {
        return nullptr;
    }
    float radius = ballDataTable[type+1].scale * 16;
    sf::Vector2f midPoint = sf::Vector2f((ballOne->GetPosition().x+ballTwo->GetPosition().x)/2, (ballOne->GetPosition().y+ballTwo->GetPosition().y)/2);
    midPoint = sf::Vector2f(
        std::clamp(midPoint.x, boundingBox.getPosition().x+radius, boundingBox.getPosition().x+boundingBox.width-radius),
        std::clamp(midPoint.y, boundingBox.getPosition().y+radius, boundingBox.getPosition().y+boundingBox.height-radius)
    );
    SuikaBall* newBall = InstantiateBall(type+1, midPoint);
    return newBall;
}

void Suika::InitAssets() {
    Game::InitAssets();
    AddFont("tomorrow", "fonts/Tomorrow-Medium.ttf");
    AddAsset("circle", "textures/circle.png");
    AddAsset("container", "textures/container.png");
    AddAsset("ball0", "textures/ball0.png");
    AddAsset("ball1", "textures/ball1.png");
    AddAsset("ball2", "textures/ball2.png");
    AddAsset("ball3", "textures/ball3.png");
    AddAsset("ball4", "textures/ball4.png");
    AddAsset("ball5", "textures/ball5.png");
    AddAsset("ball6", "textures/ball6.png");
    AddAsset("ball7", "textures/ball7.png");
    AddAsset("ball8", "textures/ball8.png");
    AddAsset("ball9", "textures/ball9.png");
    AddAsset("ball10", "textures/ball10.png");
    AddAsset("player", "textures/player.png");
}