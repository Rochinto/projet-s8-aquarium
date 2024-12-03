
#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <clocale>
#include <cmath>
#include <cstring>
#include <string.h>
#include <unistd.h>

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

#define LEN(str) strlen(str)
#define FOR_EACH(it,list) for(auto it = list.begin(); it != list.end(); it++)


namespace utils{
  bool debug();
  void setDebug();
  size_t generatePing();

  void setup();
  void close();

  void printlog(string s);
  void error(string s);
  string get_helper_msg();

  bool hasPrefix(string str, string prefix);
  bool isDigit(string str);
  vector<string> split(string str, string spliter);
  
  bool strToPair(string str, int& x, int& y);
  long double curtime();

  struct option{ string name; string str_v; int int_v; };
  list<option> getConfigs(string config_file_name);
}


#endif