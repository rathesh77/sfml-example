#include <SFML/Graphics.hpp>
#include "Mario.hpp"
#include "Constants.hpp"
#include "Map.hpp"

class Game
{
public:
    int frameCount = 0;
    
    Game(sf::RenderWindow*);

    int getCurrentMap();
    void tick(sf::Clock *);
    void shiftSceneBackward();
    void shiftElementsBackward();
    void drawElements();

    void loadMap(Map*);
    void generateSpritesInMemory();

private:
    int currentMap;
    int NB_GRIDS = 30;
    int SINGLE_BACKGROUND_WIDTH = TILE_DIMENSION*TILE_DIMENSION;

    std::string backgroundPath = "sprites/background2.png";
    std::string brickPath = "sprites/bricks.png";

    sf::Sprite *s_background;
    sf::Sprite *s_elements;

    sf::Texture t_background;
    sf::Texture t_brick;

    sf::RenderWindow *window;

    Mario *mario;

    Map *map;

    void drawSprites();

    int NB_SPRITES = 0;
    int current_grid = 0;
};