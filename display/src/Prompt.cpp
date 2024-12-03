
#include "Prompt.hpp"


Prompt::Prompt(SocketAPI* api, Aquarium* aquarium):
    api(api),
    aquarium(aquarium)
{}


bool Prompt::prompt_user(){
    string prompt("");
    cout << "$ ";

    getline(cin, prompt);
    if(cin.eof()) return false;

    if(prompt == "log out") return false;
    else if(prompt == "help") cout << utils::get_helper_msg();
    else if(prompt == "status") status();
    else if(utils::hasPrefix(prompt, "addFish")) addFish(prompt);
    else if(utils::hasPrefix(prompt, "delFish")) delFish(prompt);
    else if(utils::hasPrefix(prompt, "startFish")) startFish(prompt);
    else if(utils::debug() && prompt == "getFishes") api->sendCommand("getFishes");
    
    else cout << "-> NOK : commande introuvable (entrez: help)" << endl;
    return true;
}

bool Prompt::addFish(string msg){
    vector<string> splited_msg = utils::split(msg, " ");

    if( splited_msg.size() != 6 ||\
        splited_msg.at(0) != "addFish" ||\
        splited_msg.at(2) != "at" ||\
        splited_msg.at(3).back() != ',' ||\
        splited_msg.at(4).back() != ','
    ){
        cout << "-> NOK : Format incorrect, il doit être de la forme\n"\
        << "-> addFish <name> at <pair_pos>, <pair_size>, <mobility_type>\n"\
        << "-> Les paires sont de la forme <int>x<int>\n"\
        << "-> ! N'oubliez pas les virgules et les espaces !" << endl;
        return false;
    }

    string name = splited_msg.at(1);
    int x = 0, y = 0;
    if(!utils::strToPair(splited_msg.at(3).substr(0, splited_msg.at(3).size()-1), x, y)){
        cout << "-> NOK : Format incorrect pour le paramètre <pos>, son format doit être de la forme: <int_pos_x>x<int_pos_y>" << endl;
        return false;
    }

    int width = 0, height = 0;
    if(!utils::strToPair(splited_msg.at(4).substr(0, splited_msg.at(4).size()-1), width, height)){
        cout << "-> NOK : Format incorrect pour le paramètre <size>, son format doit être de la forme: <int_size_x>x<int_size_y>" << endl;
        return false;
    }

    if(aquarium->hasFishWithName(name)){
        cout << "-> NOK : Poisson déjà existant" << endl;
        return false;
    }

    string send("");
    send = "addFish "+name+" at "+to_string(x)+"x"+to_string(y)+","+to_string(width)+"x"+to_string(height)+", "+splited_msg.at(5);
    if(!api->sendCommand(send, "OK")){
        cout << "-> NOK : Le serveur n'a pas renvoyé de message de confirmation (NOK ou Invalide)" << endl;
        return false;
    }

    aquarium->createFish(name, x, y, width, height);
    cout << "-> OK" << endl;
    return true;
}

bool Prompt::delFish(string msg){
    vector<string> splited_msg = utils::split(msg, " ");

    if( splited_msg.size() != 2 || splited_msg.at(0) != "delFish" ){
        cout << "-> NOK : Format incorrect, il doit être de la forme: delFish <name>" << endl;
        return false;
    }

    string name = splited_msg.at(1);
    if(!aquarium->hasFishWithName(name)){
        cout << "-> NOK : Poisson inexistant" << endl;
        return false;
    }

    if(!api->sendCommand(msg, "OK")){
        cout << "-> NOK : Le serveur n'a pas renvoyé de message de confirmation (NOK ou Invalide)" << endl;
        return false;
    }

    aquarium->delFish(name);
    cout << "-> OK" << endl;
    return true;
}

bool Prompt::startFish(string msg){
    vector<string> splited_msg = utils::split(msg, " ");

    if( splited_msg.size() != 2 || splited_msg.at(0) != "startFish" ){
        cout << "-> NOK : Format incorrect, il doit être de la forme: startFish <name>" << endl;
        return false;
    }

    string name = splited_msg.at(1);
    if(!aquarium->hasFishWithName(name)){
        cout << "-> NOK : Poisson inexistant" << endl;
        return false;
    }

    if(!api->sendCommand(msg, "OK")){
        cout << "-> NOK : Le serveur n'a pas renvoyé de message de confirmation (NOK ou Invalide)" << endl;
        return false;
    }

    aquarium->startFish(name);
    cout << "-> OK" << endl;
    return true;
}

bool Prompt::status(){
    size_t ping = utils::generatePing();
    if(!api->sendCommand("ping "+to_string(ping), "pong "+to_string(ping))){
        cout << "-> NOK : TIME OUT server has not responded" << endl;
        return false;
    }
    list<Fish> list = aquarium->getAllRelativeFishes();
    cout << "-> OK : Connecté au contrôleur, " << list.size() << " poisson(s) trouvé(s)" << endl;
    
    FOR_EACH(it, list){
        Fish fish = *it;
        string s = "-> Fish "+fish.getName()+" at "+to_string((int) fish.getX())+"x"+to_string((int) fish.getY())\
        +","+to_string(fish.getWidth())+"x"+to_string(fish.getHeight())+" ";
        if(fish.isStarted()) s = s + "started";
        else s = s + "notStarted";
        cout << s << endl;
    }
    return true;
}