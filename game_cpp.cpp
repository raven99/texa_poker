#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;
bool flush_psb(const string &);
bool high_card(const string &);

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

    int flag = 0; //the deal method

    while(true)
    { 
        client_socket >> reply;
        if(reply.find("game-over") != string::npos)
        {
            cout << "game is over" << endl;
            break;
        }
        //两张底牌信息处理
        if(reply.find("hold/") && reply.find("/hold"))
        {
             //test if there is a complete hold message 
            if(flush_psb(reply))
                flag = 1;
            else if(high_card(reply))
                flag = 1;
        }
        if(reply.find("inquire") != string::npos)
        {  
            if(flag)
            client_socket << "check\n";
            else
            client_socket << "fold\n";
        }
    }
  return 0;
}

bool flush_psb(const string &flush )
{
    if(flush.find("SPADES") != flush.rfind("SPADES"))
        return true;
    else if(flush.find("HEARTS") != flush.rfind("HEARTS"))
        return true;
    else if(flush.find("CLUBS") != flush.rfind("CLUBS"))
        return true;
    else if(flush.find("DIAMONDS") != flush.rfind("DIAMONDS"))
        return true;
    else 
        return false;
}

bool high_card(const string &high)
{
    if(high.find("K")||high.find("Q")||high.find("J")||high.find("A"))
        return true;
    else
        return false;
}
