
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>

#include "logger.h"
#include "config_parser.h"

#define NB_PARAMS 3
struct options_int
{
    char *name;
    char *long_name;
    int value;
};

struct options_int config[NB_PARAMS] = {
    {"controller-port", "Port", 8080},
    {"display-timeout-value", "Display time value", 45},
    {"fish-update-interval", "Fish update interval", 1}};

void read_controller_cfg(char *config_file_name)
{
    config_t cfg;

    config_init(&cfg);

    if (!config_read_file(&cfg, config_file_name))
    {
        log_message(CRITIC, "\n%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return;
    }

    for (int i = 0; i < NB_PARAMS; i++)
    {
        if (config_lookup_int(&cfg, config[i].name, &config[i].value))
        {
            log_message(INFO, "%s set to %d", config[i].long_name, config[i].value);
        }
        else
        {
            log_message(WARN, "No '%s' setting in configuration file", config[i].name);
        }
    }
    config_destroy(&cfg);
}

uint16_t get_controller_port()
{
    return (uint16_t)(config[0].value);
}

int get_display_timeout()
{
    return config[1].value;
}

int get_fish_update_interval()
{
    return config[2].value;
}