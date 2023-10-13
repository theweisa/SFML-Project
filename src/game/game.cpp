#include "game.h"

Game::Game() {
    InitWindow();
}

Game::~Game() {
    delete window;
    //delete textures
	for (auto& texture : assets) delete texture.second;
    // delete game objects
    for (auto objVec : gameObjects) {
        for (auto& obj : objVec.second) delete obj;
    }
}
void Game::Run() {
    while(Running()) {
        Update();
    }
}
void Game::Update() {
    UpdateDeltaTime();
    UpdateMousePos();
    UpdatePollEvents();
    UpdateInputs();
    UpdateGameObjects();
    Render();
    window->display();
}
void Game::Render() {
    window->clear();
}
void Game::InitWindow() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    //set the resolution of the game
    videoMode.height = 960;
    videoMode.width = 960;
    window = new sf::RenderWindow(videoMode, "Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize); //dynamically allocate mem for window

    windowHeight = static_cast<float>(window->getSize().y);
    windowWidth = static_cast<float>(window->getSize().x);

    //set the max frame rate
    window->setFramerateLimit(60);
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
void Game::UpdateGameObjects() {
    for (auto objVec : gameObjects) {
        for (auto& obj : objVec.second) {
            obj->Update(deltaTime);
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
	if (!assets[key]->loadFromFile(fileName)) {
		std::cout << "ERROR: failed to load " << fileName << std::endl;
	}
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
    delete gameObjects[key][index];
    auto itr = gameObjects[key].erase(gameObjects[key].begin()+index);
    if (itr != gameObjects[key].end()) {
        return true;
    }
    return false;
}