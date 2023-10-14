#include "levelEditor.h"

LevelEditor::LevelEditor() {
    // 1024, 960
    // 1024-(250+25) vs 960-(25+25)
    // choose the smaller one, then set the opposing margins to even ones
    InitGrid(32, 32, sf::Vector2f(25.f, 250.f), sf::Vector2f(10.f, 40.f), true);
    InitAssets();
    InitPallete();
    InitText();
    SetBrush("1");
}

LevelEditor::~LevelEditor() {
    
}
void LevelEditor::InitGrid(int xDim, int yDim, sf::Vector2f newXMargins, sf::Vector2f newYMargins, bool squareGrid) {
    if (squareGrid) {
        float xMag = windowWidth-(newXMargins.x+newXMargins.y);
        float yMag = windowHeight-(newYMargins.x+newYMargins.y);
        if (xMag < yMag) {
            newYMargins = sf::Vector2f(
                (windowHeight-xMag)*(newYMargins.x/(newYMargins.x+newYMargins.y)), 
                (windowHeight-xMag)*(newYMargins.y/(newYMargins.x+newYMargins.y))
            );
        }
        else if (yMag < xMag) {
            newXMargins = sf::Vector2f(
                (windowWidth-yMag)*(newXMargins.x/(newXMargins.x+newXMargins.y)), 
                (windowWidth-yMag)*(newXMargins.x/(newXMargins.x+newXMargins.y))
            );
        }
    }
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

void LevelEditor::InitText() {
    //sf::Font test;
    //test.loadFromFile("../../../assets/fonts/04B_30__.TTF");
    CenterText(CreateText("Tile Pallete", 22, sf::Vector2f(windowWidth-(xMargins.y/2), yMargins.x+unitDimensions.y), "palleteTxt"));
    sf::Vector2f newPos = sf::Vector2f(gameObjects["pallete"].back()->GetPosition().x+10, gameObjects["pallete"].back()->GetPosition().y+200);
    CenterText(CreateText("Brush: 1-8\n\nEraser: E\n\nEyedrop: I\n\nBrush Size: Scroll\n\nSave: Ctrl+S\n\nLoad: Ctrl+L", 14, newPos, "yo mama"));

}

void LevelEditor::InitAssets() {
    #if _WIN32
        assetPath = "../../../assets/";
    #elif __APPLE__
        assetPath = "../../assets/";
    #endif
    // cwd: "C:\\Users\\Andrew Wei\\Documents\\SFML-Project\\build\\bin\\Debug" lmao
    // mac: "/Users/andrewwei/Documents/SFML-Project/build/bin" idk
    //AddAsset("testTile.png", assetPath+"textures/testTile.png");
    //AddAsset("testTile2.png", assetPath+"textures/testTile2.png");
    AddAsset("dirt", assetPath+"textures/dirt.png");
    AddAsset("water", assetPath+"textures/water.png");
    AddAsset("grassTree", assetPath+"textures/grassTree.png");
    AddAsset("dirtTree", assetPath+"textures/dirtTree.png");
    AddAsset("shroom", assetPath+"textures/shroom.png");
    AddAsset("ship", assetPath+"textures/ship.png");
    AddAsset("blueShroom", assetPath+"textures/blueShroom.png");
    AddAsset("grass", assetPath+"textures/grass.png");
    AddAsset("eraser tile", assetPath+"textures/eraser.png");
    AddFont("default", assetPath+"fonts/04B_30__.TTF");
}

void LevelEditor::InitPallete() {
    int palleteId = 1;
    int numCols = 1;
    for (auto& asset : assets) {
        if (asset.first == "eraser tile") continue; // teehee
        sf::Vector2f newPos = sf::Vector2f(
            windowWidth-(xMargins.y/(numCols+1))-unitDimensions.x/2, yMargins.x+(1+palleteId)*unitDimensions.y
        );
        CreateText(std::to_string(palleteId), 12, sf::Vector2f(newPos.x-unitDimensions.x, newPos.y), std::to_string(palleteId)+"Pallete");
        gameObjects["pallete"].push_back(new GameObject(std::to_string(palleteId), asset.second, newPos));
        gameObjects["pallete"].back()->SetSpriteDimensions(unitDimensions);
        palleteId++;
    }
}

sf::Vector2f LevelEditor::GetRelativeGridPosition(sf::Vector2f pos) {
    sf::Vector2i unitPos = sf::Vector2i((pos.x-xMargins.x) / unitDimensions.x, (pos.y-yMargins.x) / unitDimensions.y);
    sf::Vector2f gridPos = sf::Vector2f(
        std::clamp(xMargins.x+unitPos.x * unitDimensions.x, xMargins.x, windowWidth-xMargins.y-unitDimensions.x),
        std::clamp(yMargins.x+unitPos.y * unitDimensions.y, yMargins.x, windowHeight-yMargins.y-unitDimensions.y)
    );
    //std::cout << "Unit pos: (" << unitPos.x << ", " << unitPos.y << ")" << std::endl;
    //std::cout << "Grid pos: (" << gridPos.x << ", " << gridPos.y << ")" << std::endl;
    return gridPos; 
    
}

void LevelEditor::UpdateInputs() {
    Game::UpdateInputs();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        switch(toolState) {
            case Brush:
                PlaceTiles();
                break;
            case Erase:
                EraseTiles();
                break;
            case Eyedrop:
                EyedropTile();
                break;
            default:
                break;
        }
    }
    // there has to be a better way of doing this
    // edit: idc im too lazy idc anymore, yandere dev here i come
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        toolState = Brush;
        SetBrush("1");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        toolState = Brush;
        SetBrush("2");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
        toolState = Brush;
        SetBrush("3");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
        toolState = Brush;
        SetBrush("4");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
        toolState = Brush;
        SetBrush("5");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
        toolState = Brush;
        SetBrush("6");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
        toolState = Brush;
        SetBrush("7");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
        toolState = Brush;
        SetBrush("8");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        toolState = Erase;
        SetCurrentTileTextures(assets["eraser tile"]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
        toolState = Brush;
        SetCurrentTileTextures("1");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        toolState = Eyedrop;
        ChangeBrushSize(1);
        SetCurrentTileTextures(assets["eraser tile"]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        SaveToFile("map");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        LoadFile("map");
    }
}

void LevelEditor::PlaceTiles() {
    EraseTiles();
    for (auto tile : gameObjects["brush"]) {
        if (tile->GetPosition().x < xMargins.x-unitDimensions.x/2 
            || tile->GetPosition().x > windowWidth-xMargins.y-unitDimensions.x/2 
            || tile->GetPosition().y < yMargins.x-unitDimensions.y/2 
            || tile->GetPosition().y > windowHeight-yMargins.y-unitDimensions.y/2) 
        {
            continue;
        }
        gameObjects["tiles"].push_back(new GameObject(*tile));
    }
}

// idk ill work on you more later
void LevelEditor::EraseTiles() {
    //std::cout << "tiles: " << gameObjects["brush"].size() << std::endl;
    int numDeleted = 0;
    int numTiles = 0;
    for (auto tile : gameObjects["brush"]) {
        numTiles++;
        int objIndex = 0;
        for (auto itr = gameObjects["tiles"].begin(); itr != gameObjects["tiles"].end(); itr++) {
            if ((*itr)->InsideBounds(tile->GetPosition()+(unitDimensions/2.f))) {
                delete (*itr);
                numDeleted++;
                gameObjects["tiles"].erase(itr);
                break;
            }
        }
    }
    //std::cout << "total tiles: " << numTiles << std::endl;
    //std::cout << "tiles deleted: " << numDeleted << std::endl;
}

void LevelEditor::EyedropTile() {
    for (auto *obj : gameObjects["tiles"]) {
        if (obj->InsideBounds(mousePosView)) {
            //std::cout << "eyedrop" << std::endl;
            SetBrush(obj->id);
            toolState = Brush;
            return;
        }
    }
}

void LevelEditor::SetCurrentTileTextures(sf::Texture* newTexture, std::string newId) {
    for (auto tile : gameObjects["pallete"]) {
        if (newId == tile->id) {
            for (auto brush : gameObjects["brush"]) {
                brush->SetTexture(tile->sprite.getTexture());
                brush->id = newId;
            }
            return;
        }
    }
    for (auto tile : gameObjects["brush"]) {
        tile->SetTexture(newTexture, unitDimensions);
        if (newId != "") tile->id = newId;
    }
}

void LevelEditor::SetCurrentTileTextures(std::string newId) {
    for (auto tile : gameObjects["pallete"]) {
        if (newId == tile->id) {
            for (auto brush : gameObjects["brush"]) {
                brush->SetTexture(tile->sprite.getTexture());
                brush->id = newId;
            }
            return;
        }
    }
}

void LevelEditor::ChangeBrushSize(int newSize) {
    if (newSize < 1 || newSize > std::min(gridDimensions.x, gridDimensions.y) || gameObjects["brush"].size() <= 0) {
        return;
    }
    brushSize = newSize;
    //std::cout << "new size: " << brushSize << std::endl;
    GameObject * tile = new GameObject(*gameObjects["brush"][0]);
    for (auto& t : gameObjects["brush"]) {
        delete t;
    }
    gameObjects["brush"].clear();
    for (int i = -brushSize/2; i < std::ceil(float(brushSize)/2.f); i++) {
        for (int j = -brushSize/2; j < std::ceil(float(brushSize)/2.f); j++) {
            GameObject * newTile = new GameObject(*tile);
            newTile->SetLocalPosition(sf::Vector2f(unitDimensions.x * i, unitDimensions.y * j));
            newTile->SetPosition(GetRelativeGridPosition(mousePosView));
            gameObjects["brush"].push_back(newTile);
        }
    }
    delete tile;

    // set relative positions based on the size
}

void LevelEditor::Update() {
    Game::Update();
    for (auto tile : gameObjects["brush"]) {
        tile->SetPosition(GetRelativeGridPosition(mousePosView));
    }
}

void LevelEditor::UpdatePollEvents() {
    while (window->pollEvent(ev)) {
		switch (ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::MouseWheelScrolled:
                //std::cout << "scrolled: " << ev.mouseWheelScroll.delta << std::endl;
                if (ev.mouseWheelScroll.delta > 0) {
                    ChangeBrushSize(brushSize+1);
                }
                else if (ev.mouseWheelScroll.delta < 0) {
                    ChangeBrushSize(brushSize-1);
                }
                break;
            /*case sf::Event::TextEntered {
                int num = ev.text.unicode-48;
                std::cout << ev.text.unicode << std::endl;
                break;
                if (num-48 >= 0 && num-48 < 58) {
                    toolState = Brush;
                    //std::cout << "switch to " << num << std::endl;
                    SetBrush(std::to_string(num));
                }
                break;
            }*/
            default:
                break;
        }
	}
}

void LevelEditor::UpdatePallete() {
    if (gameObjects["brush"].size() <= 0) return;
    GameObject* curBrush = gameObjects["brush"].back();
    // find what pallete is being used currently thgouh curBrush id
    for (auto& tile : gameObjects["pallete"]) {
        if (tile->id == curBrush->id) {
            // current pallete
            break;
        }
    }
}

void LevelEditor::SetBrush(std::string id) {
    if (gameObjects["brush"].empty()) {
        gameObjects["brush"].push_back(new GameObject(*gameObjects["pallete"].front()));
        return;
    }
    for (auto& tile : gameObjects["pallete"]) {
        if (tile->id == id) {
            std::cout << "brush: " << id << std::endl;
            // highlight the tile idk how for now
            for (auto& brush : gameObjects["brush"]) {
                brush->SetTexture(tile->sprite.getTexture());
                brush->id = tile->id;
            }
            return;
        }
    }
    std::cout << "could not find id " << id << std::endl;
}

void LevelEditor::Render() {
    Game::Render();
    window->draw(grid);
    /*for (auto objKey : gameObjects) {
        for (auto& obj : objKey.second) {
            obj->Render(*window);
        }
    }*/
    for (auto& tile : gameObjects["pallete"]) {
        tile->Render(*window);
    }
    for (auto& tile : gameObjects["tiles"]) {
        tile->Render(*window);
    }
    for (auto& tile : gameObjects["brush"]) {
        tile->Render(*window);
    }
    for (auto txt : text) {
        window->draw(*txt.second);
    }
}

void LevelEditor::SaveToFile(std::string fileName) {
    std::string filePath = assetPath + "../saveFiles/" + fileName + ".txt";
    std::ofstream saveFile;
    saveFile.open(filePath, std::ios::out);
    if (!saveFile) {
        std::cout << "fuck" << std::endl;
        return;
    }
    for (auto tile : gameObjects["tiles"]) {
        //std::cout << "tile id: " << tile->id << std::endl;
        saveFile << std::to_string(tile->GetPosition().x) << "," << std::to_string(tile->GetPosition().y) << "," << tile->id << "\n";
    }
    saveFile.close();
}

// holy mother of pearl im sorry for the abominations I commited in this function
void LevelEditor::LoadFile(std::string fileName) {
    std::string filePath = assetPath + "../saveFiles/" + fileName + ".txt";
    std::ifstream saveFile;
    for (auto& tile : gameObjects["tiles"]) {
        delete tile;
    }
    gameObjects["tiles"].clear();
    saveFile.open(filePath, std::ios::in);
    if (!saveFile) {
        std::cout << "fuck" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(saveFile, line)) {
        std::string token;
        size_t pos;
        std::vector<std::string> elems;
        while((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            elems.push_back(token);
            line.erase(0, pos+1);
            //std::cout << token << ", ";
        }
        elems.push_back(line);
        //std::cout << "new id: " << elems[2] << std::endl;
        for (auto& p : gameObjects["pallete"]) {
            if (p->id != elems[2]) continue;
            GameObject* obj = new GameObject(*p);
            obj->SetPosition(sf::Vector2f(std::stof(elems[0]), std::stof(elems[1])));
            gameObjects["tiles"].push_back(obj);
            break;
        }
    }
}