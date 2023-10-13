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
    videoMode.width = 1024;
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

void Game::AddFont(const std::string key, const std::string fileName) {
    //fonts[key] = new sf::Font();
	if (font.loadFromFile(fileName)) {
		std::cout << "ERROR: failed to load " << fileName << std::endl;
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