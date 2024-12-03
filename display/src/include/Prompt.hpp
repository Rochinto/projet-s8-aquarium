
#ifndef __PROMPT_HPP__
#define __PROMPT_HPP__

#include "SocketAPI.hpp"

class Prompt{

private:
    SocketAPI* api;
    Aquarium* aquarium;

    int hello(string);
    bool addFish(string);
    bool delFish(string);
    bool startFish(string);
    bool status();

public:
    Prompt(SocketAPI* api, Aquarium* aquarium);
    bool prompt_user();

};

#endif