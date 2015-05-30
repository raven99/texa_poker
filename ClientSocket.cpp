// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include <unistd.h>

ClientSocket::ClientSocket ( std::string host, int port, std::string my_ip, int my_port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::bind (my_ip, my_port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

/*
  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }
*/
  while(!Socket::connect ( host, port ) )
    {
        usleep( 100 * 1000 ); 
    }
}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  Socket::send ( s );
  return *this;

}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  Socket::recv ( s );
  return *this;
}
