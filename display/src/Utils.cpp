
#include "Utils.hpp"

bool debug_mode = false;
size_t ping_number = 0;

string logfile_name = "displayer.log";
ofstream logfile;

bool utils::debug(){ return debug_mode; }
void utils::setDebug(){ debug_mode = true; }

size_t utils::generatePing(){
  return ++ping_number;
}

void utils::setup(){
  logfile.open(logfile_name);
  logfile << "[LOG FILE]\n";
}

void utils::close(){
  logfile.close();
}

void utils::printlog(string s){
  logfile << s << endl;
}

void utils::error(string s){
  s = "[ERROR] "+s;
  utils::printlog(s+" ("+strerror(errno)+")");
  utils::close();
  cout << s << endl;
  cout << "You may check the log file ("+logfile_name+")" << endl;
  exit(1);
}

string utils::get_helper_msg(){
  string msg =  "### Liste des commandes ###\n"
                "- help      : affiche la liste des commandes\n"
                "- log out   : quitte l'afficheur (avec demande au controleur)\n"
                "- status    : affiche la liste des poissons disponibles (visibles)\n"
                "- addFish   : demande l'ajout d'un poisson au controleur\n"
                "- delFish   : demande la suppression d'un poisson au controleur\n"
                "- startFish : demande le départ d'un poisson au controleur\n";
  if(debug())
    msg = msg + "- getFishes : demande la prochaine destination des poissons (DEBUG UNIQUEMENT)\n";
  msg = msg +   "###########################\n";
  return msg;
}


bool utils::hasPrefix(string str, string prefix){
  return (str.find(prefix, 0) == 0);
}

bool utils::isDigit(string str){
  int d=0;
  if(str.front()=='-') d=1;

  for(int i=d; i < str.size(); i++)
    if(!isdigit(str[i])) return false;
  return true;
}

vector<string> utils::split(string str, string spliter){
  vector<string> r;
  string temp = str;
  size_t index_found = temp.find(spliter, 0);
  size_t spliter_len = spliter.size();

  while(index_found != string::npos){
    r.push_back(temp.substr(0, index_found));
    temp = temp.substr(index_found+spliter_len);
    index_found = temp.find(spliter, 0);
  }

  r.push_back(temp);
  return r;
}

bool utils::strToPair(string str, int& x, int& y){
  vector<string> splited = utils::split(str, "x");
  if(splited.size() != 2) return false;
  if(!utils::isDigit(splited.at(0)) || !utils::isDigit(splited.at(1))) return false;
  x = stoi(splited.at(0));
  y = stoi(splited.at(1));
  return true;
}

long double utils::curtime(){
  return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}



list<utils::option> utils::getConfigs(string config_file_name){
  ifstream config_file(config_file_name);

  if( !config_file.is_open() ) error("Problème lors de la lecture du fichier de configuration : "+config_file_name);
  string line;
  while( config_file ) {
    getline(config_file, line);
    cout << line << ": " << config_file.tellg() << '\n';
  }

  config_file.close();
  list<utils::option> r;
  return r;
}