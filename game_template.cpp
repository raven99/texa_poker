#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <memory.h>
using namespace std;

int m_socket_id = -1;

int game_process(int);
void send_action(int, string);

int main(int argc, char *argv[])
{
    //if there is no enough parameters
    if(argc != 6) 
    {
        cout << "Usage:./" << argv[0] << " server_ip server_port my_ip my_port my_id" 
            << endl;
        return -1;
    }    

    in_addr_t server_ip = inet_addr( argv[1] );
    in_port_t server_port = htons( atoi( argv[2]) );
    in_addr_t my_ip = inet_addr( argv[3]);
    in_port_t my_port = htons( atoi( argv[4]) );
    int my_id  = atoi( argv[5] );
    //open socket
    m_socket_id = socket(AF_INET, SOCK_STREAM, 0);
    //if there is any error m_socket_id = -1
    if(m_socket_id < 0)
    {
        cout << "init socket failed" << endl;
        return -1;
    }

    int is_reuse_addr = 1;
    setsockopt( m_socket_id, SOL_SOCKET, SO_REUSEADDR, 
            (const char*)&is_reuse_addr, sizeof(is_reuse_addr));
    
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = my_ip;
    my_addr.sin_port = my_port;

    if(bind(m_socket_id, (struct sockaddr*)&my_addr, sizeof(my_addr)))
    {
        cout << "bind failed!" << endl;
        return -1;
    }

    //connect to server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = server_ip;
    my_addr.sin_port = server_port;
    
    while(connect(m_socket_id, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        usleep( 100 * 1000);
    }
    
    string reg_msg;
    stringstream ss;
    ss << "reg:" << my_id << "I am Grandet:)";
    ss >> reg_msg;
    send_action(m_socket_id, reg_msg);

    while(1)
    {
        if( game_process(m_socket_id) == 0)      
        break;
    }
    close(m_socket_id);

    return 0;

}

int game_process(int socketid)
{
    char buffer[1024];
    recv(socketid, buffer, sizeof(buffer) - 1, 0);
    string recvbuff = buffer;
    if( recvbuff.size() > 0)
    {
        if(recvbuff.find("game-over"))
        { 
            cout << "game is over" << endl;
            return 0;
        }
        if(recvbuff.find("inquire"))
        {
            send_action(socketid, "fold");
        }
    }
}

void send_action(int socketid, string action)
{
    
    send(socketid, action.c_str(), action.length(), 0);
}
