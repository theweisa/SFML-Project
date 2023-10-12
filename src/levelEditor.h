#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "game/game.h"

class LevelEditor : public Game {
    enum ToolState { Brush, Erase, Eyedrop };
    protected:
        sf::VertexArray grid;
        sf::Vector2i gridDimensions;
        sf::Vector2f unitDimensions;
        std::vector<GameObject*> currentTiles;
        sf::Vector2f xMargins;
        sf::Vector2f yMargins;
        int brushSize = 1;
        ToolState toolState=Brush;
    public:
        LevelEditor();
        ~LevelEditor();
        void InitGrid(int, int, sf::Vector2f, sf::Vector2f);
        void InitAssets() override;
        sf::Vector2f GetRelativeGridPosition(sf::Vector2f pos);
        void PlaceTiles();
        void EraseTiles();
        void EyedropTile();
        void SetCurrentTileTextures(sf::Texture*, std::string newId="");
        void ChangeBrushSize(int);
        void Render() override;
        void Update() override;
        void UpdatePollEvents() override;
        void UpdateInputs() override;
};

#endif