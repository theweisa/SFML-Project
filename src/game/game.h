#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ostream>

#include "gameObject.h"

class Game {
protected:
    // window variables
    sf::RenderWindow* window;
    sf::Event ev;
	sf::VideoMode videoMode;
    float windowWidth;
    float windowHeight;

    // assets
    std::unordered_map<std::string, sf::Texture*> assets;

    // mouse pos
    sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

    // containers
    std::unordered_map<std::string, std::vector<GameObject*>> gameObjects;

    // time vars
    sf::Clock deltaClock;
    float deltaTime;

public:
    Game();
    ~Game();
    void Run();
    virtual void InitAssets() = 0;
    virtual void Update();
    virtual void Render();
    void InitWindow();
    const bool Running() const;
    //time it takes to update and render one frame
    void UpdateDeltaTime();
    void UpdatePollEvents();
    void UpdateGameObjects();
    void UpdateMousePos();
    virtual void UpdateInputs();
    void AddAsset(const std::string, const std::string);
    std::vector<GameObject*> PositionOverGameObjects(sf::Vector2f);
    bool DeleteGameObject(GameObject*);
    bool DeleteGameObject(std::string, int);

    sf::Vector2f GetMousePosView() { return mousePosView; }
};


#endif