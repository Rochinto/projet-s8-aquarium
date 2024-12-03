#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config_parser.h"
#include "socket.h"
#include "logger.h"
#include "prompt.h"
#include "controller.h"
#include "Aquarium.h"

#define CONFIG_FILE_NAME "controller.cfg"
#define DEFAULT_AQUARIUM_CONFIG_FILE "aquarium.txt"

int main(int argc, char *argv[])
{
    puts("Welcome to Aquarium Socket API v0.3 !");
    log_message(INFO, "Welcome to Aquarium Socket API v0.3 !");

    read_controller_cfg(CONFIG_FILE_NAME);
    uint16_t port = get_controller_port();

    parser(DEFAULT_AQUARIUM_CONFIG_FILE, 1);
    init_controller();
    init_socket(port);

    prompt();

    end_controller();
    end_socket();
    log_message(SUCCESS, "Server halted !");
    return 0;
}