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
#include <fstream>
#include <cmath>

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
    sf::Font font;
    std::unordered_map<std::string, sf::Text*> text;

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
    virtual void UpdatePollEvents();
    void UpdateGameObjects();
    void UpdateMousePos();
    virtual void UpdateInputs();
    void AddAsset(const std::string, const std::string);
    void AddFont(const std::string, const std::string);
    sf::Text* CreateText(const std::string initialText, unsigned charSize, sf::Vector2f initialPos, const std::string key, sf::Color color=sf::Color::White);

    std::vector<GameObject*> PositionOverGameObjects(sf::Vector2f);
    void CenterText(sf::Text*);
    bool DeleteGameObject(GameObject*);
    bool DeleteGameObject(std::string, int);

    sf::Vector2f GetMousePosView() { return mousePosView; }
};


#endif