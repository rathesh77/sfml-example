#include "Game.hpp"
#include <iostream>

Game::Game(sf::RenderWindow *window)
{

    this->currentMap = 0;
    this->window = window;

    this->mario = new Mario();
}

void Game::loadMap(Map *map)
{
    this->map = map;
    this->NB_GRIDS = map->getNumberOfGrids();
    std::cout << this->backgroundPath;
    if (!this->t_background.loadFromFile(this->backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    if (!this->t_brick.loadFromFile(this->brickPath))
        throw std::invalid_argument("Could not load background texture");

    this->s_background = new sf::Sprite[NB_GRIDS];

    for (int i = 0; i < NB_GRIDS; i++)
    {
        auto sprite = sf::Sprite();
        this->s_background[i] = sprite;
        this->s_background[i].setTexture(this->t_background);
        this->s_background[i].setTextureRect(sf::IntRect(0, 40, SINGLE_BACKGROUND_WIDTH, WINDOW_HEIGHT));
        this->s_background[i].setPosition(i * SINGLE_BACKGROUND_WIDTH, 0);
    }

    generateSpritesInMemory();
}

void Game::generateSpritesInMemory() {
    const int nbElements = map->getNthGrid(current_grid)->NB_SPRITES + map->getNthGrid(current_grid+1)->NB_SPRITES; 
    //if (this->s_elements)
    //delete this->s_elements; // memory freed
    //this->s_elements = NULL; not necessary since we reallocate some space below
    this->s_elements = new sf::Sprite[nbElements];
    auto *save_ptr = this->s_elements;
    for (int i = current_grid; i < current_grid + 2; i++) {
        auto *ptr = map->getNthGrid(i)->element;
        while (ptr) {
            if (ptr->type == "brick") {
                this->s_elements->setTexture(t_brick);
                this->s_elements->setTextureRect(sf::IntRect(272, 112, TILE_DIMENSION, TILE_DIMENSION));
                this->s_elements->setPosition(ptr->position.x + (16 * 16  * (i - current_grid)), ptr->position.y );
            }
            ptr = ptr->next;
            this->s_elements++;
        }
    }
    this->s_elements = save_ptr;
}

int Game::getCurrentMap()
{
    return this->currentMap;
}

void Game::tick(sf::Clock *clock)
{

    sf::Event event;

    // std::cout << "mario velocity: " + std::to_string(this->mario->getVelocity()) << std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->mario->setDirection(1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->mario->setDirection(-1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        this->mario->jump();
    }

    while (this->window->pollEvent(event))
    {

        switch (event.type)
        {
        case sf::Event::Closed:
            this->window->close();
        case sf::Event::KeyReleased:
            // std::cout << "Key released:" + std::to_string(event.key.code) << std::endl;
            switch (event.key.code)
            {
            case sf::Keyboard::Key::Right:
            case sf::Keyboard::Key::Left:
                this->mario->setDirection(0);
                break;
            }
            break;
        }
    }

    if (clock->getElapsedTime().asSeconds() >= 0.1f)
    {
        this->frameCount++;
        clock->restart();
        this->mario->loadSpriteForward(this->frameCount);
    }

    this->mario->updateVelocity();

    if (this->mario->marioIsFreezed())
    {

        this->shiftSceneBackward();
    }

    this->mario->moveX();
    this->mario->moveY();
    std::cout << std::to_string(this->mario->realCoordinates.x) << std::endl;
    this->drawSprites();
}

void Game::drawSprites()
{

    for (int i = 0; i < NB_GRIDS; i++)
    {
        this->window->draw(this->s_background[i]);
    }

    this->drawElements();
    this->window->draw(this->mario->getSprite());
}

void Game::shiftSceneBackward()
{
    for (int i = 0; i < NB_GRIDS; i++)
        this->s_background[i].move(sf::Vector2f(-this->mario->getVelocity(), 0));

    this->shiftElementsBackward();
    
}

void Game::shiftElementsBackward() {
    auto *save_ptr = this->s_elements;
    for (int i = current_grid; i < current_grid + 2; i++) {
        int nb = 0;
        while (nb < map->getNthGrid(i)->NB_SPRITES) {
            this->s_elements->move(sf::Vector2f(-this->mario->getVelocity(), 0));
            this->s_elements++;
            nb++;
        }
    }
    this->s_elements = save_ptr;
}

void Game::drawElements() {
    auto *save_ptr = this->s_elements;
    for (int i = current_grid; i < current_grid + 2; i++) {
    int nb = 0;
        while (nb < map->getNthGrid(i)->NB_SPRITES ) {
                this->window->draw(*this->s_elements);
            nb++;
            this->s_elements++;
        }
    }
    this->s_elements = save_ptr;
}