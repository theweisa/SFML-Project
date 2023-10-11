#include "levelEditor.h"

LevelEditor::LevelEditor() {
    InitGrid(64, 64, sf::Vector2f(25.f, 25.f), sf::Vector2f(25.f, 25.f));
    InitAssets();
    GameObject* test = new GameObject(*assets["test tile"], unitDimensions);
    currentTiles.push_back(test);
    //currentTiles.push_back(sf::RectangleShape(unitDimensions));
}

LevelEditor::~LevelEditor() {
    
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
void LevelEditor::InitAssets() {
    std::cout << "init assets" << std::endl;
    AddAsset("test tile", "assets/textures/testTile.png");
}
sf::Vector2f LevelEditor::GetRelativeGridPosition(sf::Vector2f pos) {
    sf::Vector2i unitPos = sf::Vector2i((pos.x-xMargins.x) / unitDimensions.x, (pos.y-yMargins.x) / unitDimensions.y);
    sf::Vector2f gridPos = sf::Vector2f(
        std::clamp(xMargins.x+unitPos.x * unitDimensions.x, xMargins.x, windowWidth-xMargins.x-unitDimensions.x),
        std::clamp(yMargins.x+unitPos.y * unitDimensions.y, yMargins.x, windowHeight-yMargins.y-unitDimensions.y)
    );
    //std::cout << "Unit pos: (" << unitPos.x << ", " << unitPos.y << ")" << std::endl;
    //std::cout << "Grid pos: (" << gridPos.x << ", " << gridPos.y << ")" << std::endl;
    return gridPos; 
    
}

void LevelEditor::UpdateInputs() {
    Game::UpdateInputs();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

    }
}

void LevelEditor::Update() {
    Game::Update();
    //if (currentTiles.size() > 0)
    //currentTiles[0]->SetPosition(GetRelativeGridPosition(mousePosView));
}

void LevelEditor::Render() {
    Game::Render();
    window->draw(grid);
    for (auto& tile : currentTiles) {
        tile->Render(*window);
    }
    //window->draw(currentTiles[0]);
}