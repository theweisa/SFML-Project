#include "levelEditor.h"

LevelEditor::LevelEditor() {
    InitGrid(64, 64, sf::Vector2f(25.f, 25.f), sf::Vector2f(25.f, 25.f));
}
LevelEditor::~LevelEditor() {
    for (int i = grid.getVertexCount()-1; i >= 0; i--) {
        delete &grid[i];
    }
    grid.clear();
}
void LevelEditor::InitGrid(int xDim, int yDim, sf::Vector2f newXMargins, sf::Vector2f newYMargins) {
    gridDimensions = sf::Vector2i(xDim, yDim);
    xMargins = newXMargins;
    yMargins = newYMargins;
    grid = sf::VertexArray(sf::Lines, gridDimensions.x*gridDimensions.y*2);
    unitDimensions = sf::Vector2f((windowWidth-(xMargins.x+xMargins.y))/(gridDimensions.x-1), (windowHeight-(yMargins.x+yMargins.y))/(gridDimensions.y-1));
    int verticePos = 0;

    for (int i = 0; i < gridDimensions.y; i++) {
        grid[verticePos].position = sf::Vector2f(xMargins.x, i*unitDimensions.y+yMargins.x);
        grid[verticePos+1].position = sf::Vector2f(windowWidth-xMargins.y, i*unitDimensions.y+yMargins.x);
        verticePos += 2;
    }
    for (int i = 0; i < gridDimensions.x; i++) {
        grid[verticePos].position = sf::Vector2f(i*unitDimensions.x+xMargins.x, yMargins.x);
        grid[verticePos+1].position = sf::Vector2f(i*unitDimensions.x+xMargins.x, windowHeight-yMargins.y);
        verticePos += 2;
    }
}
sf::Vector2f LevelEditor::GetRelativeGridPosition(sf::Vector2f pos) {
    sf::Vector2i unitPos = sf::Vector2i((pos.x-xMargins.x) / unitDimensions.x, (pos.y-yMargins.x) / unitDimensions.y);
    sf::Vector2f gridPos = sf::Vector2f(unitPos.x * unitDimensions.x, unitPos.y * unitDimensions.y);
    //std::cout << "Unit pos: (" << unitPos.x << ", " << unitPos.y << ")" << std::endl;
    // std::cout << "Grid pos: (" << gridPos.x << ", " << gridPos.y << ")" << std::endl;
    return gridPos;
    
}

void LevelEditor::Update() {
    Game::Update();
    GetRelativeGridPosition(mousePosView);
}

void LevelEditor::Render() {
    Game::Render();
    window->draw(grid);
}