/*
 * Attention:
 * To keep things simple, do not handle socket/bind/listen/.../epoll_create/epoll_wait API error
 */
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "socket.h"
#include "middleware.h"
#include "logger.h"

#define MAX_CONNEXIONS 16
#define MAX_CLIENTS_EVENTS 32
#define SOCKET_BUF_SIZE 128
#define NB_THREAD 3

static int is_on = 0;
static int listen_sock; // listen socket fd
static int conn_sock;   // connection socket fd
pthread_t thread_socket;

static struct sockaddr_in srv_addr;                           // server address
static struct sockaddr_in cli_addr;                           // client address
static int cli_len;                                           // client address length
static struct epoll_event clients_events[MAX_CLIENTS_EVENTS]; // client event pool

static int ep_connections_fd; // epoll fd for new connections
static int ep_clients_fd;     // epoll fd for clients

/*
 * register clients_events of fd to epfd
 */
static void epoll_ctl_add(int epoll_fd, int fd, uint32_t events_params)
{
    struct epoll_event ev;
    ev.events = events_params;
    ev.data.fd = fd;
    exit_if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1, "Error control failed");
}

static void set_sockaddr(struct sockaddr_in *addr, uint16_t port)
{
    memset((char *)addr, '\0', sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

static int setnonblocking(int sockfd)
{
    return (fcntl(sockfd, F_SETFD, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1) ? -1 : 0;
}

static void *connections_handler(void *args)
{
    (void)args;
    int ev;   // inspected event index
    int nfds; // number of current events
    char buf[SOCKET_BUF_SIZE];

    while (is_on == 1)
    {
        /* Epoll waiting for an event */
        nfds = epoll_wait(ep_connections_fd, clients_events, MAX_CLIENTS_EVENTS, 1000);

        /* Foreach triggered event */
        for (ev = 0; ev < nfds; ev++)
        {
            memset(buf, '\0', SOCKET_BUF_SIZE);
            /* New connection */
            if (clients_events[ev].data.fd == listen_sock)
            {
                conn_sock = accept(listen_sock, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_len);
                inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr), buf, sizeof(cli_addr));
                log_message(INFO, "New incoming connection from %s", buf);
                setnonblocking(conn_sock); // set non-blocking I/O for new connection
                epoll_ctl_add(ep_connections_fd, conn_sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
                epoll_ctl_add(ep_clients_fd, conn_sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
            }
            /* Closing connection */
            if (clients_events[ev].events & (EPOLLRDHUP | EPOLLHUP))
            {
                epoll_ctl(ep_clients_fd, EPOLL_CTL_DEL, clients_events[ev].data.fd, NULL);
                epoll_ctl(ep_connections_fd, EPOLL_CTL_DEL, clients_events[ev].data.fd, NULL);
                close(clients_events[ev].data.fd);
                inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr), buf, sizeof(cli_addr));
                log_message(INFO, "Connection to %s closed", buf);
                continue;
            }
        }
    }
    pthread_exit(NULL);
}

static void *active_connection(void *args)
{
    (void)args;
    int ev;   // inspected event index
    int nfds; // number of current events

    while (is_on == 1)
    {
        /* Epoll waiting for an event */
        nfds = epoll_wait(ep_clients_fd, clients_events, MAX_CLIENTS_EVENTS, 1000);

        /* Foreach triggered event */
        for (ev = 0; ev < nfds; ev++)
        {
            /* Active connection */
            // TODO: generize this part to accept all kind of client handling
            if (clients_events[ev].events & EPOLLIN && !(clients_events[ev].events & (EPOLLRDHUP | EPOLLHUP)))
            {
                exit_if(authenticate(clients_events[ev].data.fd) == -1, "Middleware error");
            }
        }
    }
    pthread_exit(NULL);
}

pthread_t threads[NB_THREAD]; // tableau des
void *functions[NB_THREAD] = {&connections_handler, &active_connection, &active_connection};

void init_socket(uint16_t port)
{
    is_on = 1;

    log_message(INFO, "Socket starting...");
    listen_sock = socket(AF_INET, SOCK_STREAM, 0); // start socket listening

    set_sockaddr(&srv_addr, port); // setup server address and port
    exit_if(bind(listen_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0, "bind failed");

    setnonblocking(listen_sock); // non-blocking I/O to accept multiple connections and multithreading
    exit_if(listen(listen_sock, MAX_CONNEXIONS) < 0, "listen failed");

    ep_connections_fd = epoll_create1(0); // Epoll instanciation and setup
    ep_clients_fd = epoll_create1(0);     // Epoll instanciation and setup
    epoll_ctl_add(ep_connections_fd, listen_sock, EPOLLIN | EPOLLOUT | EPOLLET);

    cli_len = sizeof(cli_addr); // save client address length
    for (int i = 0; i < NB_THREAD; i++)
    {
        pthread_create(&threads[i], NULL, functions[i], NULL);
    }
    log_message(SUCCESS, "Socket started ! Listening to new connections.");
}

void end_socket()
{
    is_on = 0;
    log_message(INFO, "Socket stopping...");
    void **res[NB_THREAD] = {}; // résultat à récupérer
    for (int i = 0; i < NB_THREAD; i++)
    {
        pthread_join(threads[i], res[i]);
    }
    close(listen_sock); // Close listening socket
    log_message(SUCCESS, "Socket ended !");
}