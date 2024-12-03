
#ifndef __FISH_HPP__
#define __FISH_HPP__

#include <iostream>
#include <string>

#define NIMAGES 8

using namespace std;

// Define a Fish class to store information about each fish
class Fish {
  private:
    string name;
    float x, y, vx, vy;
    int width, height;
    float tx, ty;
    int frames_to_target;
    bool started;
    int received;
    bool left = false;
    unsigned image_number;
  
  public:
    Fish(string n, float xpos, float ypos, int w, int h);
    // Define getter and setter functions for each fish attribute
    string getName();
    float getX();
    float getY();
    int getReceived();
    void setReceived(int n);
    void setX(float xpos);
    void setY(float ypos);
    void setPosition(float xpos, float ypos);

    /*float getVx();
    float getVy();
    void setVx(float Vx);
    void setVy(float Vy);
    void setSpeed(float Vx, float Vy);*/

    int getWidth();
    int getHeight();

    void setTarget(float tx, float ty, int frames);
    float getTargetX();
    float getTargetY();
    void decrementFramesTarget();
    void setFramesTarget(int frames);
    int getFramesTarget();
    
    bool isStarted();
    void setStarted(bool value);

    unsigned getImageNumber();
    bool isGoingLeft();
    void setGoingLeft(bool v);

    bool operator==(const Fish&);
};

#endif