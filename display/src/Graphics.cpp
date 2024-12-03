
#include "Graphics.hpp"

Graphics::Graphics(Aquarium& aquarium):
    aquarium(aquarium),
    window(nullptr)
{}

#ifdef ENSEIRB
#define close Close
#define clear Clear
#define isOpen IsOpened
#define draw Draw
#define setFramerateLimit SetFramerateLimit
#define display Display
#define type Type
#define pollEvent GetEvent
#define setPosition SetPosition
#define setTexture SetImage
#define loadFromFile LoadFromFile
#define Texture Image
#define SPRITE(name, texture, x, y, w, h)\
        sf::Sprite name;\
        name.SetImage(texture);\
        name.Resize(w, h);\
        name.SetPosition(x, y)
#else
#define SPRITE(name, texture, x, y, w, h)\
        sf::RectangleShape name(sf::Vector2f(w, h));\
        name.setTexture(&texture);\
        name.setPosition(x, y)
#endif

void Graphics::loop(){
    if(!bg_texture.loadFromFile("./res/aquarium.png")){}
    SPRITE(bg, bg_texture, 0, 0, WIDTH, HEIGHT);

    #ifdef ENSEIRB
    window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, 32), "Aquarium");
    #else
    window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Aquarium");
    #endif

    
    window->setFramerateLimit(FPS);

    sf::Texture fish_texture[NIMAGES];
    for(int i=0; i < NIMAGES; i++) if(!fish_texture[i].loadFromFile("./res/p"+to_string(i+1)+".png")){}

    long unsigned int fps = 0;
    long unsigned int fps_counter = 0;
    long unsigned int time = utils::curtime();

    running = true;
    while(window->isOpen() && running){
        sf::Event event;
        while(window->pollEvent(event)){
            if(event.type == sf::Event::Closed) window->close();
        }

        window->clear(sf::Color::White);
        window->draw(bg);

        {
            list<Fish> list = aquarium.getAllFishes();
               
            FOR_EACH(it, list){
                int w=0;   
                int h=0;
                Fish f = *it;
                Fish* p = aquarium.getFishByName(f.getName());
                if(p->getReceived() == 1){
                    w = f.getWidth();
                    h = f.getHeight();
                }

                SPRITE(fish, fish_texture[p->getImageNumber()], (int) f.getX(), (int) f.getY(), w, h);
                
                   
                #ifdef ENSEIRB
                if(!(f.isGoingLeft())) fish.FlipX(true);
                #else
                if(!(p->isGoingLeft())){
                    fish.setScale(-1, 1);
                    fish.setPosition(fish.getPosition() + sf::Vector2f(fish.getSize().x, 0));
                }
                #endif
                
                window->draw(fish);
            }
        }
        

        window->display();

        long unsigned int epsilon = utils::curtime()-time;
        if(epsilon < 1000) fps_counter++;
        else{
            fps = fps_counter;
            //cout << "FPS=" << fps << endl;
            //PRINT("FPS="<<fps);
            fps_counter=0;
            time=utils::curtime();
        }

        aquarium.tick();
    }
    running = false;
    if(window->isOpen()) window->close();
    delete window;
}

void Graphics::stop(){ running = false; }