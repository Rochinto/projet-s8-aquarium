
#ifndef ENSEIRB
#ifndef __CONFIG_HPP__
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <libconfig.h>

struct options_int {
    const char * name;
    int value;
};

struct options_str {
    const char * name;
    const char * value;
};

struct options_res {
    struct options_str * str_;
    struct options_int * int_;
};

struct options {
    std::string address;
    std::string id;
    int port;
    int timeout;
    std::string resources;
};


options get_options_config(std::string config_file_name);

#endif
#else
struct options {
    std::string address;
    std::string id;
    int port;
    int timeout;
    std::string resources;
};
#endif