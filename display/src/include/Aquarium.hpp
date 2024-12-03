
#ifndef __AQUARIUM_HPP__
#define __AQUARIUM_HPP__

#include "Utils.hpp"
#include "Fish.hpp"

using namespace std;

class Aquarium {

private:
  string name;
  int width, height;
  int view_id;
  unsigned fps;
  list<Fish> fish_list;
  

public:
  
  Aquarium(string n, int w, int h, unsigned fps);
  // Define getter and setter functions for each fish attribute
  string getName();

  int getViewId();
  void setViewId(int id);

  list<Fish> getAllFishes();
  list<Fish> getAllRelativeFishes();
  Fish* getFishByName(string n);
  bool hasFishWithName(string name);
  void createFish(string name, int xpos, int ypos, int w, int h);
  void setFishPosition(string name, int x, int y);
  void setFishTarget(string name, int tx, int ty, int time);
  /*int getFishX(Fish f);
  int getFishY(Fish f);
  void setFishX(Fish f, int xpos);
  void setFishY(Fish f, int ypos);
  void setFishPosition(Fish f, int xpos, int ypos);
  int getFishVx(Fish f);
  int getFishVy(Fish f);
  void setFishVx(Fish f, int Vx);
  void setFishVy(Fish f, int Vy);
  void setFishSpeed(Fish f, int Vx, int Vy);*/
  void delFish(string name);
  void startFish(string name);
  void killNotReceivedFish();
  void tick();
};

#endif