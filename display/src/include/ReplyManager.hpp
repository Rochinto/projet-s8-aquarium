
#ifndef __REPLY_MANAGER_HPP__
#define __REPLY_MANAGER_HPP__

#include "Aquarium.hpp"

class ReplyManager{

private:
    Aquarium* aquarium;
    string debug_last_send;

    void greeting(string);
    void no_greeting();
    void list_received(string);
    void manage_fish(string);

    int getFishes(const struct line *l);
    int ls(const struct line *l);
    int getFishesContinuously(const struct line *l);
    int logout(const struct line *l);
    int ping(const struct line *l);
    int addFish(const struct line *l);
    int delFish(const struct line *l);
    int startFish(const struct line *l);

    void manage_reply(string command_sent);

public:
    ReplyManager();
    ReplyManager(Aquarium* aquarium);
    bool parseAll(list<string> replies, string expected);

    void set_last_send(string msg);
    string get_next_debug();
};

#endif