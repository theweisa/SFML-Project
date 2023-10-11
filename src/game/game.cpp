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
    Render();
    window->display();
}
void Game::Render() {
    window->clear();
}
void Game::InitWindow() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    //set the resolution of the game
    videoMode.height = 512;
    videoMode.width = 512;
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
void Game::UpdateInputs() {
    return;
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