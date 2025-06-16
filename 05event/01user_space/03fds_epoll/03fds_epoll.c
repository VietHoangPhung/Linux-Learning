/**
 * Write a C program to monitor multiple input sources using `epoll()`. 
 * Simulate two sources: stdin and atemporary file descriptor (can be a pipe or socketpair).
 * Handle events from both.
 * 
 * 
 * use epoll to manage stdin and client's connfd. 
 * print the available data from connfds
 * write available data in stdin to all client connfd 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX_EVENTS  10
#define BUFFER_SIZE 1024

#define HOST        "127.0.0.1"
#define PORT        10001
#define MAX_CLIENT  10

typedef struct {
    int clients[10];
    int client_count;
} client_list;

void set_nonblocking(int fd);
int add_client(int, client_list*);
void remove_client(int, client_list*);
void sigint_handler(int);

int server_fd, epfd;


int main() {
    client_list clist = {0};

    int conn_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);    // tcp, ipv4

    /** bind server_fd */
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;      
    addr.sin_addr.s_addr = inet_addr(HOST);
    addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    /** client specs but never used, just to fit the params */
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // listen for connection
    listen(server_fd, 100);

    set_nonblocking(server_fd);

    epfd = epoll_create1(0);

    struct epoll_event ev, events[MAX_EVENTS];
    /** add stdin to epoll list */
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    /** add server_fd to epoll*/
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

    printf("server listening on port %d...\n", PORT);

    while(1) {
        // wait
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

        for(int i = 0; i < nfds; ++i) {
            /** data available in stdin */
            if(events[i].data.fd == STDIN_FILENO) {
                char buffer[BUFFER_SIZE];
                ssize_t count = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
                if(count > 0) {
                    buffer[count] = '\0'; 
                    printf("STDIN: %s\n", buffer);
                    for(int i = 0; i < clist.client_count; i++) {
                        write(clist.clients[i], buffer, strlen(buffer));
                    }
                } else if(count == 0) {
                    printf("?\n");
                } else {
                    perror("stdin read");
                }
            }
            /** new connection */
            else if(events[i].data.fd == server_fd) {    
                conn_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if(add_client(conn_fd, &clist)) {
                    printf("new client, conn_fd: %d\n", conn_fd);
                    set_nonblocking(conn_fd);

                    /** add new conn_fd to epoll */
                    ev.events = EPOLLIN;
                    ev.data.fd = conn_fd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
                } else {
                    printf("new client but reached limit\n");
                    continue;
                }
            
            /** available data in conn_fd*/
            } else {
                char buffer[BUFFER_SIZE];
                ssize_t count = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
                if(count < 0) {
                    perror("read");

                /** client disconnect */
                } else if(count == 0) {
                    printf("client with conn_fd %d disconnected\n", events[i].data.fd);   
                    close(events[i].data.fd); // close conn_fd
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);    // delete fd from epoll list
                    remove_client(events[i].data.fd, &clist); // remove client from client list
                
                /** data available */
                } else {
                    buffer[count] = '\0';
                    printf("Received from %d: %s", events[i].data.fd, buffer);
                }
            }
        }
    }

    close(server_fd);
    close(epfd);
    return 0;
}

/**
 * @brief   set a file to nonblock, mean to exit if fd aint available to read / write
 * @param   int fd the file descriptor
 * @return  none
 */
void set_nonblocking(int fd) {
    /* get the current file's flag*/
    int file_flags = fcntl(fd, F_GETFL);

    /* add O_NONBLOCK to the flags*/
    fcntl(fd, F_SETFL, file_flags | O_NONBLOCK);
}


/**
 * @brief   add a client connection file descriptor to a list
 * @param   fd (int)              the connection file descriptor 
 * @param   clist (client_list*)  pointer to the client list
 * @return  1 if success, 0 if fail (reach max list limit)    
 */
int add_client(int fd, client_list* clist) {
    if(clist->client_count >= MAX_CLIENT) {
        return 0;
    }

    clist->clients[clist->client_count] = fd;
    clist->client_count++;
    return 1;
}

/**
 * @brief   remove a client from a list
 * @param   fd (int)              the connection file descriptor 
 * @param   clist (client_list*)  pointer to the client list
 * @return  none
 */
void remove_client(int fd, client_list* clist) {
    for(int i = 0; i < clist->client_count; i++) {
        if(clist->clients[i] == fd) {
            /** move last client to the deleted client place*/
            clist->clients[i] = clist->clients[clist->client_count - 1];
            clist->client_count--;
            return;
        }
    }
}


// /**
//  * @brief   handle SIGINT to exit gracefully
//  * @param   sig int signo
//  * @return  none
//  */
// void sigint_handler(int sig) {
//     close(server_fd);
//     close(epfd);
    
//     exit(0);
// }



