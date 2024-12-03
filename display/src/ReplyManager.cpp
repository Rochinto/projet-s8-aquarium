
#include "ReplyManager.hpp"
#include <tuple>

ReplyManager::ReplyManager():
    aquarium(nullptr)
{}

ReplyManager::ReplyManager(Aquarium* aquarium):
    aquarium(aquarium)
{}

bool ReplyManager::parseAll(list<string> replies, string expected){
    bool found = false;
    FOR_EACH(it,replies){
        if(*it == expected) found = true;
        manage_reply(*it);
    }
    return found;
}

void ReplyManager::manage_reply(string reply){
    if(utils::hasPrefix(reply, "greeting")) greeting(reply);
    else if(reply == "no greeting") no_greeting();
    else if(utils::hasPrefix(reply, "list")) list_received(reply);
}





void ReplyManager::greeting(string msg){
    if( utils::hasPrefix(msg, "greeting N") && utils::isDigit(msg.substr(LEN("greeting N"))) )\
    aquarium->setViewId(stoi(msg.substr(LEN("greeting N"))));
}

void ReplyManager::no_greeting(){
    aquarium->setViewId(-1);
}

void ReplyManager::list_received(string msg){
    vector<string> splited = utils::split(msg, " [");
    if( splited.at(0) != "list" ) return;
    splited.erase(splited.begin());

    list<Fish> list = aquarium->getAllFishes();
    FOR_EACH(it, list){
            Fish f = *it;
            Fish * p = aquarium->getFishByName(f.getName());
            p->setReceived(0);
    }

    FOR_EACH(it, splited){
        if(it->back() != ']') return;
        manage_fish(it->substr(0, it->size()-1));
    }
    
    FOR_EACH(it, list){
            Fish f = *it;
            Fish * p = aquarium->getFishByName(f.getName());
            if(p->getReceived() == 0)
                aquarium->delFish(p->getName());
    }

}

void ReplyManager::manage_fish(string str){
    vector<string> splited = utils::split(str, " ");
    if(splited.size() != 3 || splited.at(1) != "at") return;

    string fish_name = splited.at(0);
    vector<string> data = utils::split(splited.at(2), ",");
    if(splited.size() != 3) return;

    int w = 0, h = 0, fx = 0, fy = 0;
    if(!utils::strToPair(data.at(0), fx, fy) || !utils::strToPair(data.at(1), w, h) || !utils::isDigit(data.at(2))) return;
    int time = stoi(data.at(2));
    
    aquarium->createFish(fish_name, fx, fy, w, h);
    Fish* fish = aquarium->getFishByName(fish_name);
    if(fish->getReceived() != 2)
    fish->setReceived(1);
    //fish->setPosition(x, y);
    //int dx = fx-x, dy = fy-y;
    //fish->setSpeed((int) dx/time, (int) dy/time);
    //fish->setSpeed(1,1);
    
    //aquarium->setFishPosition(fish_name, x, y);
    aquarium->setFishTarget(fish_name, fx, fy, time);
    //aquarium->setFishTarget(fish_name, 0, 0, 10);
}





void ReplyManager::set_last_send(string msg){ if(utils::debug()) debug_last_send = msg; }
string ReplyManager::get_next_debug(){
  if(!utils::debug()) return "";
  if(utils::hasPrefix(debug_last_send, "log out")) return "bye\n";
  if(utils::hasPrefix(debug_last_send, "hello")) return "greeting N42\n";
  if(utils::hasPrefix(debug_last_send, "ping")) return "pong "+debug_last_send.substr(5)+"\n";
  if(utils::hasPrefix(debug_last_send, "addFish")) return "OK\n";
  if(utils::hasPrefix(debug_last_send, "delFish")) return "OK\n";
  if(utils::hasPrefix(debug_last_send, "startFish")) return "OK\n";
  if(utils::hasPrefix(debug_last_send, "getFishes")){
    string r("list");
    list<Fish> list = aquarium->getAllRelativeFishes();
    FOR_EACH(it, list){
        Fish f = *it;
        r = r+" ["+f.getName()+" at "+to_string( (rand()%100)-5 )+"x"+to_string( (rand()%100)-5 )\
        +","+to_string( f.getWidth() )+"x"+to_string( f.getHeight() )+","+to_string((rand()%5)+1)+"]";
    }
    return r+"\n";
  }
  return "";
}