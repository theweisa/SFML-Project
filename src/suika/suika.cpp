#include "suika.h"

Suika::Suika() {
    Init();
}

void Suika::InitGame() {
    Game::InitGame();
    yLimitPos = windowHeight * 0.2f;
    ballDataTable = {
        {0, BallData(0, "circle", 0.5, 1)},
        {1, BallData(1, "circle", 0.91, 3)},
        {2, BallData(2, "circle", 1.32, 6)},
        {3, BallData(3, "circle", 1.73, 10)},
        {4, BallData(4, "circle", 2.14, 15)},
        {5, BallData(5, "circle", 2.55, 21)},
        {6, BallData(6, "circle", 2.96, 28)},
        {7, BallData(7, "circle", 3.37, 36)},
        {8, BallData(8, "circle", 3.78, 45)},
        {9, BallData(9, "circle", 4.19, 55)},
        {10, BallData(10, "circle", 5, 66)}
    };
    ballDropList = {0, 1, 2, 3};
    ballToDrop = RandomRange(0, ballDropList.size()-1);
    nextBallToDrop = RandomRange(0, ballDropList.size()-1);
    InitConstraints();
    Instantiate("displaySprite", ballDataTable[ballToDrop].assetName, sf::Vector2f(0,0));
    Instantiate("nextSprite", ballDataTable[nextBallToDrop].assetName, sf::Vector2f(windowWidth*0.7f,windowHeight*0.2f));
    //obj->SetScale(sf::Vector2f(3.f, 3.f));
}

void Suika::InitConstraints() {
    float boxWidth = windowWidth * 0.4;
    boundingBox = sf::FloatRect((windowWidth-boxWidth)/2, 0, boxWidth, windowHeight);
    // sf::FloatRect box = sf::FloatRect(4, 4, 8, 8);
}

void Suika::UpdatePollEvents() {
    while (window->pollEvent(ev)) {
		switch (ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::MouseButtonPressed:
                if (dropTimer > 0) break;
                dropTimer = dropInterval;
                gameObjects["balls"].push_back(InstantiateBall(ballToDrop, dropPos));
                ballToDrop = nextBallToDrop;
                nextBallToDrop = RandomRange(0, ballDropList.size()-1);
                gameObjects["displaySprite"][0]->SetTexture(assets[ballDataTable[ballToDrop].assetName]);
                gameObjects["nextSprite"][0]->SetTexture(assets[ballDataTable[nextBallToDrop].assetName]);
                break;
        }
	}
}

void Suika::Update() {
    Game::Update();
    if (dropTimer > 0) {
        dropTimer -= deltaTime;
    }
    gameObjects["displaySprite"][0]->SetPosition(dropPos);
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
    if (ball != nullptr && objPos.y < yLimitPos) {
        ball->aboveLine = true;
        if (ball->loseTimer <= 0) {
            std::cout << "game over!";
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

    // std::cout << objPos.y+radius << " vs " << boundingBox.getPosition().y+boundingBox.height << std::endl;
    // const sf::Vector2 dir = my bfs glaceno
    // get the distance
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
            for (int i = 0; i < updateSteps; i++) {
                UpdateObjectPhysics(obj);
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
    //object->collidedObjects.clear();
    for (auto& objVec : gameObjects) {
        auto itr = objVec.second.begin();
        while (itr != objVec.second.end()) {
            GameObject * objTwo = *itr;
            if (object->body == nullptr || object == objTwo) {
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
                    const float massRatio = object->body->radius / (object->body->radius + objTwo->body->radius);
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
    if (ballDataTable.find((ballOne->type+1)) == ballDataTable.end()) {
        return nullptr;
    }
    sf::Vector2f midPoint = sf::Vector2f((ballOne->GetPosition().x+ballTwo->GetPosition().x)/2, (ballOne->GetPosition().y+ballTwo->GetPosition().y)/2);
    SuikaBall* newBall = InstantiateBall(type+1, midPoint);
    return newBall;
}

void Suika::InitAssets() {
    Game::InitAssets();
    AddAsset("circle", "textures/circle.png");
}