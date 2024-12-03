
#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Aquarium.hpp"

#define WIDTH 900
#define HEIGHT 600
#define FPS 60

class Graphics{
private:
    Aquarium& aquarium;
    bool running = false;

    #ifdef ENSEIRB
    sf::Image bg_texture;
    sf::Sprite bg;
    #else
    sf::Texture bg_texture;
    sf::RectangleShape bg;
    #endif
    sf::RenderWindow* window;

public:
    Graphics(Aquarium& aquarium);
    void loop();
    void stop();
};

#endif