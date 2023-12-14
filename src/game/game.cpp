#include "game.h"

Game::Game() {
    srand(static_cast<unsigned>(time(0)));
}

Game::~Game() {
    delete window;
    //delete textures
	for (auto& texture : assets) delete texture.second;
    // delete game objects
    DeleteAllGameObjects();
    ClearText();
}
void Game::Run() {
    while(Running()) {
        Update();
    }
}
void Game::Init() {
    InitAssets();
    InitWindow(525, 858);
    InitGame();
}
void Game::Update() {
    UpdateDeltaTime();
    UpdateMousePos();
    UpdatePollEvents();
    UpdateInputs();
    Render();
    UpdateGameObjects();
    window->display();
}
void Game::Render() {
    window->clear(windowColor);
}

void Game::RenderText() {
    for (auto txt : text) {
        window->draw(*txt.second);
    }
}
void Game::InitWindow(float height, float width) {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    windowColor = sf::Color(117, 57, 42, 255);
    //set the resolution of the game
    videoMode.height = height;
    videoMode.width = width;
    window = new sf::RenderWindow(videoMode, "Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize); //dynamically allocate mem for window

    windowHeight = static_cast<float>(window->getSize().y);
    windowWidth = static_cast<float>(window->getSize().x);

    //set the max frame rate
    window->setFramerateLimit(60);
}
void Game::InitAssets() {
    #if _WIN32
        assetsPath = "../../../assets/";
    #elif __APPLE__
        assetsPath = "../../assets/";
    #endif
}
const bool Game::Running() const {
    return window->isOpen();
}
void Game::UpdateDeltaTime() {
    deltaTime = deltaClock.restart().asSeconds();
}
void Game::UpdatePollEvents() {
    while (window->pollEvent(ev)) {
		switch (ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            default:
                break;
        }
	}
}

GameObject* Game::Instantiate(const std::string parent, const std::string textureName, sf::Vector2f pos, PhysicsBody::BodyType bodyType) {
   GameObject* obj = new GameObject("yomama", assets[textureName], pos);
   sf::FloatRect box = sf::FloatRect(4, 4, 8, 8);
   obj->AddPhysicsBody(new PhysicsBody(obj, bodyType, box));
   gameObjects[parent].push_back(obj);
   return obj;
}

GameObject* Game::Instantiate(const std::string parent, const std::string textureName, sf::Vector2f pos) {
    GameObject* obj = new GameObject("yomama", assets[textureName], pos);
    gameObjects[parent].push_back(obj);
    return obj;
}

void Game::UpdateGameObjects() {
    for (auto objVec : gameObjects) {
        for (auto& obj : objVec.second) {
            if (!obj->active) continue;
            obj->Update(deltaTime);
            if (renderGameObjects) obj->Render(*window);
            UpdateObjectPhysics(obj);
            if (ExitedScreen(*obj, sf::Vector2f(obj->sprite.getGlobalBounds().width, obj->sprite.getGlobalBounds().height))) {
                OnScreenExit(*obj);
            }
        }
    }
}

void Game::UpdateText() {
    for (auto* t : text) {
        window->draw(t);
    }
}

bool Game::ExitedScreen(GameObject &object, sf::Vector2f offset) {
    return object.GetPosition().y < offset.y || object.GetPosition().y >= windowHeight - offset.y || object.GetPosition().x < offset.x || object.GetPosition().x >= windowWidth - offset.x;
}

void Game::UpdateObjectPhysics(GameObject* object) {
    if (object->body == nullptr) return;
    object->body->Update(deltaTime);
    object->collidedObjects.clear();
    // this inefficient as hell but im curious if there's better ways to do it
    for (auto objVec : gameObjects) {
        for (auto& obj : objVec.second) {
            if (object->body == nullptr || object == obj) {// || !(object.body->GetGlobalHitbox().intersects(obj->body->GetGlobalHitbox()))) {// || !object.body->interactableLayers[obj.body.layer]) {
                continue;
            }
            if (obj->sprite.getGlobalBounds().intersects(object->sprite.getGlobalBounds())) {
                object->collidedObjects.push_back(obj);
                object->OnTriggerEnter(*(obj->body));
            }
            // doesn't work for now teehee
            /*if (obj->body->GetGlobalHitbox().intersects(object.body->GetGlobalHitbox())) {
                object.OnTriggerEnter(*(obj->body));
            }*/
            //std::cout << "meet?" << std::endl;
        }
    }
}

void Game::UpdateInputs() {
    
}
void Game::UpdateMousePos() {
	//updates mouse position relative to window
	mousePosWindow = sf::Mouse::getPosition(*window);
	mousePosView = window->mapPixelToCoords(mousePosWindow);
}
void Game::AddAsset(const std::string key, const std::string fileName) {
	assets[key] = new sf::Texture();
	if (!assets[key]->loadFromFile(assetsPath+'/'+fileName)) {
		std::cout << "ERROR: failed to load " << assetsPath+'/'+fileName << std::endl;
	}
}

void Game::AddFont(const std::string key, const std::string fileName) {
    //fonts[key] = new sf::Font();
	if (font.loadFromFile(assetsPath+'/'+fileName)) {
		std::cout << "ERROR: failed to load " << assetsPath+'/'+fileName << std::endl;
	}
}

void Game::CenterText(sf::Text* text) {
    //std::cout << text["key"] << std::endl;
    
    sf::FloatRect textRect = text->getLocalBounds();
    text->setOrigin(
        textRect.left + textRect.width/2.0f,
        textRect.top  + textRect.height/2.0f
    );
    return;
    
}

sf::Text* Game::CreateText(const std::string initialText, unsigned charSize, sf::Vector2f initialPos, const std::string key, sf::Color color) {
    sf::Text* newText = new sf::Text;
	text[key] = newText;			        //create new text
	newText->setFont(font);					//set the font
	newText->setCharacterSize(charSize);		//set the text character size
	newText->setFillColor(color);			//set the text color
	newText->setString(initialText);			//set the string
	newText->setPosition(initialPos);		//set the initial position
    return newText;
}


std::vector<GameObject*> Game::PositionOverGameObjects(sf::Vector2f pos) {
    std::vector<GameObject*> hoveredObjs;
    for (auto objVec : gameObjects) {
        for (auto& obj : objVec.second) {
            if (obj->InsideBounds(pos)) {
                hoveredObjs.push_back(obj);
            }
        }
    }
    return hoveredObjs;
}

const float Game::RandomRange(float min, float max) const {
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}
const int Game::RandomRange(int min, int max) const {
    return min + rand() % (max+1-min);
}

bool Game::DeleteText(std::string name) {
    delete text[name];
    size_t val = text.erase(name);
    if (val > 0) return true;
    return false;
}

void Game::ClearText() {
    for (auto &keyval : text) {
        delete keyval.second;
    }
    text.clear();
}

bool Game::DeleteGameObject(GameObject* delObj) {
    for (auto &keyval : gameObjects) {
        int objIndex = 0;
        for (auto *obj : gameObjects[keyval.first]) {
            if (obj == delObj) {
                return DeleteGameObject(keyval.first, objIndex);
            }
            objIndex++;
        }
    }
    return false;
}

bool Game::DeleteGameObject(std::string key, int index) {
    if (gameObjects.find(key) == gameObjects.end() || index >= gameObjects[key].size()) return false;
    delete gameObjects[key][index];
    auto itr = gameObjects[key].erase(gameObjects[key].begin()+index);
    if (itr != gameObjects[key].end()) {
        return true;
    }
    return false;
}

void Game::DeleteAllGameObjects() {
    for (auto &keyval : gameObjects) {
        for (int i = keyval.second.size()-1; i >= 0; i--) {
            delete keyval.second.at(i);
        }
        keyval.second.clear();
    }
    gameObjects.clear();
}