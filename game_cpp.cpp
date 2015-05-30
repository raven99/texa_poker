#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;
int main ( int argc, char* argv[] )
{
    if(argc != 6) 
    {
        cout << "Usage:./" << argv[0] << " server_ip server_port my_ip my_port my_id" 
            << endl;
        return -1;
    }    

    string server_ip = argv[1];
    int server_port = atoi( argv[2] );
    string my_ip = argv[3];
    int my_port = atoi( argv[4]);
    int my_id  = atoi( argv[5] );


    ClientSocket client_socket ( server_ip, server_port, my_ip, my_port);
    string reply;
    stringstream ss;
    ss << "reg: " << my_id << " Killer\n";
    reply = ss.str();
    client_socket << reply;
    while(true)
    { 
        client_socket >> reply;
        if(reply.find("game-over") != string::npos)
        {
            cout << "game is over" << endl;
            break;
        }
        if(reply.find("hold") != string::npos)
        {
             
        }
        if(reply.find("inquire") != string::npos)
            client_socket << "all_in\n";
    }
  return 0;
}
