
#ifndef ENSEIRB
#include "Config.hpp"

config_t cfg;

struct options_res * read_affichage_cfg(const char *config_file_name) {
    const char * param[] = {"controller-address","id","controller-port","display-timeout-value","resources"};    int param_nbr=5;
    config_setting_t *setting;

    config_init(&cfg);
    struct options_str *res_str = new options_str[3];
    struct options_int *res_int = new options_int[2];

    if (!config_read_file(&cfg, config_file_name)) {
        std::cerr << config_error_file(&cfg) << ":" << config_error_line(&cfg) << " - " << config_error_text(&cfg) << std::endl;
        config_destroy(&cfg);
        return NULL;
    }

    setting = config_lookup(&cfg, "");
    if (setting != NULL) {
        int i_int=0;
        int i_str=0;
        for (int i = 0; i < param_nbr; i++) {
            if (i==2 || i==3){
                res_int[i_int].name = param[i];
                if (config_setting_lookup_int(setting, res_int[i_int].name, &res_int[i_int].value)) {
                    //std::cout << res_int[i_int].name << ": " << res_int[i_int].value << std::endl;
                }
                else {
                    std::cerr << "No '" << res_int[i_int].name << "' setting in configuration file." << std::endl;
                }
                i_int++;
            }
            else{
                res_str[i_str].name = param[i];
                if (config_setting_lookup_string(setting, res_str[i_str].name, &res_str[i_str].value)) {
                    //std::cout << res_str[i_str].name << ": " << res_str[i_str].value << std::endl;
                }
                else {
                    std::cerr << "No '" << res_str[i_str].name << "' setting in configuration file." << std::endl;
                }
                i_str++;
            }
        }
    }
    struct options_res * res = new options_res;
    res->str_=res_str;
    res->int_=res_int;
    return res;
}

options get_options_config(std::string config_file_name){
    options opts;
    options_res * res = read_affichage_cfg(config_file_name.c_str());
    for(int i=0; i<3; i++){
        
        if(res->str_[i].name == "controller-address") opts.address = std::string(res->str_[i].value);
        if(res->str_[i].name == "id") opts.id = std::string(res->str_[i].value);
        if(res->str_[i].name == "resources") opts.resources = std::string(res->str_[i].value);
    }
    for(int i=0; i<2; i++){
        if(res->int_[i].name == "controller-port") opts.port = res->int_[i].value;
        if(res->int_[i].name == "display-timeout-value") opts.timeout = res->int_[i].value;
    }
    delete [] res->str_;
    delete [] res->int_;
    delete res;
    config_destroy(&cfg);
    return opts;
}

#endif