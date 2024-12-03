#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

#define UNUSED(x) (void)(x)

#include <stdint.h>
#include <stddef.h>

void read_controller_cfg(char *config_file_name);
uint16_t get_controller_port();
int get_display_timeout();
int get_fish_update_interval();
#endif // __CONFIG_PARSER_H__
