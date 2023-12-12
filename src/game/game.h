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
    const float PI = 3.14159265;
    

    // assets
    std::string assetsPath;
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

    bool renderGameObjects = true;

public:
    Game();
    ~Game();
    void Run();
    virtual void Init();
    virtual void InitAssets();
    virtual void Update();
    virtual void Render();
    virtual void RenderText();
    void InitWindow(float, float);
    virtual void InitGame() {};
    const bool Running() const;
    //time it takes to update and render one frame
    virtual void UpdateDeltaTime();
    virtual void UpdatePollEvents();
    virtual void UpdateGameObjects();
    virtual void UpdateObjectPhysics(GameObject&);
    void UpdateMousePos();
    virtual void UpdateInputs();
    bool ExitedScreen(GameObject&, sf::Vector2f=sf::Vector2f(0,0));
    virtual void OnScreenExit(GameObject&) {};
    void AddAsset(const std::string, const std::string);
    void AddFont(const std::string, const std::string);
    const float RandomRange(float min, float max) const;
    const int RandomRange(int min, int max) const;
    sf::Text* CreateText(const std::string initialText, unsigned charSize, sf::Vector2f initialPos, const std::string key, sf::Color color=sf::Color::White);
    GameObject* Instantiate(const std::string parent, const std::string textureName, sf::Vector2f pos, PhysicsBody::BodyType bodyType=PhysicsBody::Static);

    std::vector<GameObject*> PositionOverGameObjects(sf::Vector2f);
    void CenterText(sf::Text*);
    bool DeleteText(std::string);
    void ClearText();
    bool DeleteGameObject(GameObject*);
    bool DeleteGameObject(std::string, int);
    void DeleteAllGameObjects();

    sf::Vector2f GetMousePosView() { return mousePosView; }
};


#endif