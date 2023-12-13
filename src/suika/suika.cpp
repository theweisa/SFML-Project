#include "suika.h"

Suika::Suika() {
    Init();
}

void Suika::InitGame() {
    Game::InitGame();
    InitConstraints();
    GameObject* obj = Instantiate("balls", "circle", sf::Vector2f(windowWidth/2.f, windowHeight/2.f), PhysicsBody::Dynamic);
    obj->body->SetBaseRadius(16);
    //obj->SetScale(sf::Vector2f(3.f, 3.f));
}

void Suika::InitConstraints() {
    boundingBox = sf::FloatRect(0, 0, windowWidth, windowHeight);
    // sf::FloatRect box = sf::FloatRect(4, 4, 8, 8);
}

void Suika::UpdatePollEvents() {
    while (window->pollEvent(ev)) {
		switch (ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::MouseButtonPressed:
                GameObject* obj = Instantiate("balls", "circle", GetMousePosView(), PhysicsBody::Dynamic);
                obj->body->SetBaseRadius(16);
                float randomScale = RandomRange(0.8f, 1.2f);
                obj->SetScale(sf::Vector2f(randomScale, randomScale));
                break;
        }
	}
}

void Suika::ApplyConstraints(GameObject* obj) {
    if (obj->body == nullptr) return;
    sf::Vector2f objPos = obj->GetPosition();
    float radius = obj->body->radius;
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

void Suika::UpdateObjectPhysics(GameObject *object) {
    if (object->body == nullptr) return;
    object->body->Update(deltaTime);
    // this inefficient as hell but im curious if there's better ways to do it
    object->collidedObjects.clear();
    for (auto objVec : gameObjects) {
        for (auto& objTwo : objVec.second) {
            if (object->body == nullptr || object == objTwo) {// || !(object.body->GetGlobalHitbox().intersects(obj->body->GetGlobalHitbox()))) {// || !object.body->interactableLayers[obj.body.layer]) {
                continue;
            }
            object->collidedObjects.push_back(objTwo);
            if (objTwo->body->trigger) {
                if (objTwo->sprite.getGlobalBounds().intersects(object->sprite.getGlobalBounds())) {
                    object->OnTriggerEnter(*(objTwo->body));
                }
                // doesn't work for now teehee
                /*if (obj->body->GetGlobalHitbox().intersects(object.body->GetGlobalHitbox())) {
                    object.OnTriggerEnter(*(obj->body));
                }*/
            }
            else {
                const sf::Vector2f collAxis = object->GetPosition() - objTwo->GetPosition();
                const float dist = std::hypot(collAxis.x, collAxis.y);
                if (dist < object->body->radius + objTwo->body->radius) {
                    const sf::Vector2 n = collAxis / dist;
                    const float delta = object->body->radius + objTwo->body->radius - dist;
                    const float massRatio = object->body->radius / (object->body->radius + objTwo->body->radius);
                    object->SetPosition(object->GetPosition() + massRatio * delta * n); 
                }
            }
            
        }
    }
    ApplyConstraints(object);
}

void Suika::InitAssets() {
    Game::InitAssets();
    AddAsset("circle", "textures/circle.png");
}