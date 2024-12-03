#include <iostream>
#include <string>
#include <stdlib.h>

#include "Fish.hpp"


Fish::Fish(string n, float xpos, float ypos, int w, int h) {
  std::hash<std::string> hasher;
  auto hashed = hasher(n);
  Fish::name = n;
  setPosition(xpos,ypos);
  width = w;
  height = h;
  started = false;
  tx = xpos;
  ty = ypos;
  frames_to_target = 1;
  
  received = 2;
  image_number = hashed%NIMAGES;
};
// Define getter and setter functions for each fish attribute
string Fish::getName() { return name; }

float Fish::getX() { return x; }
float Fish::getY() { return y; }
void Fish::setX(float xpos) { x = xpos; }
void Fish::setY(float ypos) { y = ypos; }
void Fish::setPosition(float xpos, float ypos) {x = xpos; y = ypos;} 
int Fish::getReceived(){return received;}
void Fish::setReceived(int n){received = n;}
/*float Fish::getVx() { return vx; }
float Fish::getVy() { return vy; }
void Fish::setVx(float Vx) { vx = Vx; }
void Fish::setVy(float Vy) { vy = Vy; }
void Fish::setSpeed(float Vx, float Vy) {vx = Vx; vy = Vy;}*/

int Fish::getWidth() { return width; }
int Fish::getHeight() { return height; }

float Fish::getTargetX(){ return tx; }
float Fish::getTargetY(){ return ty; }
void Fish::setTarget(float tx, float ty, int frames){ this->tx = tx; this->ty = ty; setFramesTarget(frames); }
void Fish::decrementFramesTarget(){ frames_to_target--; }
void Fish::setFramesTarget(int frames){ frames_to_target = frames; }
int Fish::getFramesTarget(){ return frames_to_target; }

bool Fish::isStarted() { return started; }
void Fish::setStarted(bool value) { started = value; }

unsigned Fish::getImageNumber(){ return image_number; }
bool Fish::isGoingLeft(){ return left; }
void Fish::setGoingLeft(bool v){ left = v; }

bool Fish::operator==(const Fish &d1) { return d1.name.compare(this->name) == 0;};
