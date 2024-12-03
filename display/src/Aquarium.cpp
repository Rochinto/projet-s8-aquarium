
#include "Aquarium.hpp"
#include <ostream>

mutex rwm;

Aquarium::Aquarium(string n, int w, int h, unsigned fps) {
  Aquarium::name = n;
  width = w;
  height = h;
  this->fps = fps;
};


string Aquarium::getName() { return name; }
int Aquarium::getViewId(){ return view_id; }
void Aquarium::setViewId(int id){ view_id = id; }


//private
Fish* Aquarium::getFishByName(string n){
  FOR_EACH(it, fish_list) if(n == it->getName()) return &(*it);
  return nullptr;
}



list<Fish> Aquarium::getAllFishes(){
  rwm.lock();
  list<Fish> r;
  FOR_EACH(it, fish_list) r.push_back(*it);
  rwm.unlock();
  return r;
}


list<Fish> Aquarium::getAllRelativeFishes(){
  rwm.lock();
  list<Fish> r;
  FOR_EACH(it, fish_list){
    Fish f = *it;
    r.push_back(Fish(f.getName(), (f.getX()*100)/width, (f.getY()*100)/height, (f.getWidth()*100)/width, (f.getHeight()*100)/height));
  }
  rwm.unlock();
  return r;
}

bool Aquarium::hasFishWithName(string name){
  rwm.lock();
  bool r = (getFishByName(name) != nullptr);
  rwm.unlock();
  return r;
}


struct Fish_size_cmp
{
 bool operator()(Fish &f,  Fish &f2) 
{
    return f.getWidth()*f.getHeight() > f2.getWidth()*f2.getHeight(); // assumes that the implementation uses a flat address space
}
};

void Aquarium::createFish(string name, int xpos, int ypos, int w, int h){
  rwm.lock();
  if(getFishByName(name) == nullptr)
    fish_list.push_back(Fish(name, (xpos*width)/100, (ypos*height)/100, (int) (w*width)/100, (int) (h*height)/100));
    fish_list.sort(Fish_size_cmp());
  rwm.unlock();
}

void Aquarium::tick(){
  rwm.lock();
  FOR_EACH(it, fish_list){
    Fish* f = &(*it);
    
    float dx = (f->getTargetX() - f->getX())/ f->getFramesTarget();
    float dy = (f->getTargetY() - f->getY())/ f->getFramesTarget();

    if(dx > 0) f->setGoingLeft(false);
    else f->setGoingLeft(true);
    
    f->setPosition(f->getX()+dx, f->getY()+dy);
    f->decrementFramesTarget();
    if(f->getFramesTarget() <= 0) f->setFramesTarget(1);
  }
  rwm.unlock();
}

// void Aquarium::killNotReceivedFish(){
//   rwm.lock();
//   FOR_EACH(it, fish_list){
//       Fish f = *it;
//       Fish * p = getFishByName(f.getName());
//       if(p->getReceived() == 0){
//           Aquarium::delFish(p->getName());
//       }
//   }
//   rwm.unlock();
// }

void Aquarium::setFishPosition(string name, int x, int y){
  rwm.lock();
  Fish* f = getFishByName(name);
  f->setPosition((x*width)/100, (y*height)/100);
  rwm.unlock();
}

void Aquarium::setFishTarget(string name, int tx, int ty, int time){
  rwm.lock();
  Fish* f = getFishByName(name);
  f->setTarget((tx*width)/100, (ty*height)/100, time*fps);
  rwm.unlock();
}

/*int Aquarium::getFishX(Fish f){ return f.getX(); }
int Aquarium::getFishY(Fish f){ return f.getY(); }
void Aquarium::setFishX(Fish f, int xpos){ f.setX(xpos); }
void Aquarium::setFishY(Fish f, int ypos){ f.setX(ypos); }
void Aquarium::setFishPosition(Fish f, int xpos, int ypos){ f.setPosition(xpos, ypos); }
int Aquarium::getFishVx(Fish f){return f.getVx();}
int Aquarium::getFishVy(Fish f){return f.getVy();}
void Aquarium::setFishVx(Fish f, int Vx){ f.setVx(Vx); }
void Aquarium::setFishVy(Fish f, int Vy){ f.setVy(Vy); }
void Aquarium::setFishSpeed(Fish f, int Vx, int Vy){ f.setSpeed(Vx,Vy); }*/
void Aquarium::delFish(string name){
  rwm.lock();
  fish_list.remove(*getFishByName(name));
  rwm.unlock();
}

void Aquarium::startFish(string name){
  rwm.lock();
  Fish* f = getFishByName(name);
  f->setStarted(true);
  rwm.unlock();
}
