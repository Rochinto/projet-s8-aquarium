
#include "SocketAPI.hpp"

mutex socket_mutex;

SocketAPI::SocketAPI(string host_name, int port, Aquarium *aquarium) : server(gethostbyname(host_name.c_str())),
                                                                       port(port),
                                                                       parser(ReplyManager(aquarium)),
                                                                       aquarium(aquarium)
{
    this->setup(host_name);
}

SocketAPI::SocketAPI(char *host_name, int port, Aquarium *aquarium) : server(gethostbyname(host_name)),
                                                                      port(port),
                                                                      parser(ReplyManager(aquarium)),
                                                                      aquarium(aquarium)
{
    this->setup(host_name);
}

SocketAPI::SocketAPI() : server(nullptr),
                         port(0),
                         parser(ReplyManager()),
                         aquarium(nullptr)
{
}

void SocketAPI::setup(string host_name)
{
    if (!utils::debug())
    {
        if (server == nullptr)
            utils::error("no such host (" + host_name + ")");

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            utils::error("opening socket");

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(port);

        const sockaddr *serv_addr_const = (const sockaddr *)&serv_addr;
        if (connect(sockfd, serv_addr_const, sizeof(serv_addr)) < 0)
            utils::error("connecting");
    }
}

/*int SocketAPI::send(const char* data, unsigned int len){
    n = write(sockfd,data,len);
    if (n < 0) error("ERROR writing to socket");
    return 0;
}*/

int SocketAPI::send(string data)
{
    string data_formatted(data);
    data_formatted.append("\n");
    utils::printlog("> " + data);

    if (utils::debug())
    {
        parser.set_last_send(data);
        return 0;
    }

    n = write(sockfd, data_formatted.c_str(), data_formatted.length());
    if (n < 0)
        utils::error("writing to socket");
    return 0;
}

void SocketAPI::loopRead()
{
    running = true;
    while (running)
    {
        this->readAll();
        parser.parseAll(this->getListReplies(), "list");
        this_thread::sleep_for(chrono::milliseconds(3));
    }
}

void SocketAPI::readAll()
{
    char buffer[BUFSIZE];
    if (utils::debug())
    {
        strcpy(buffer, parser.get_next_debug().c_str());
        parser.set_last_send("");
    }
    else
    {
        bzero(buffer, BUFSIZE);
        n = read(sockfd, buffer, BUFSIZE - 1);
        if (n < 0)
            utils::error("reading from socket");
    }

    unsigned long int len = strlen(buffer);
    if ((len >= BUFSIZE - 1) && (buffer[BUFSIZE - 2] != '\n'))
        utils::error("too much data handled");

    string r("");
    socket_mutex.lock();
    for (int i = 0; i < len; i++)
    {
        if (buffer[i] == '\n')
        {
            utils::printlog("< " + r);
            msg_list.push_back(r);
            r = "";
        }
        else
        {
            r = r + buffer[i];
        }
    }
    socket_mutex.unlock();
}

void SocketAPI::close(thread *loop)
{
    running = false;
    loop->join();
    ::close(sockfd);
}

list<string> SocketAPI::getListReplies()
{
    socket_mutex.lock();
    list<string> result;
    FOR_EACH(it, msg_list)
    {
        if (utils::hasPrefix(*it, "list"))
        {
            result.push_back(*it);
            it = msg_list.erase(it);
        }
    }
    socket_mutex.unlock();
    return result;
}

list<string> SocketAPI::getReplies()
{
    socket_mutex.lock();
    list<string> result = msg_list;
    msg_list.clear();
    socket_mutex.unlock();
    return result;
}

bool SocketAPI::sendCommand(string command, string expected_reply)
{
    this->send(command);
    this_thread::sleep_for(chrono::milliseconds(10));
    list<string> replies = this->getReplies();
    this->send("getFishesContinuously");
    //while(replies.size() == 0) this_thread::sleep_for(chrono::milliseconds(20));
    //usleep(200);
    // this->readAll();
    // if(!utils::debug() && !parser.parseAll(this->getReplies(), expected_reply))
    return parser.parseAll(replies, expected_reply);
}

void SocketAPI::sendCommand(string command)
{
    this->sendCommand(command, "");
}