#include <WinSock2.h>
#include <WS2tcpip.h>
#include "UDPServer.h"
#include <string.h>
#include <io.h>
#include <stdio.h>


namespace udp_server
{
	/**
	 *Here we need to initialize the server object.
	 *We'll use a fixed port and try first doing client-server protocol on the same computer.
	 *If that works we can try moving code afterwards.
	 *The port specified is host side. Let's use something like 59,200.
	 *The addr parameter represents a textual address, either IPv6 or v6. 
	 * It can either directly be the host name or just the numbers- let's use numbers. 
	 *On failure to resolve the address, the server will error and the constructor will throw an exception.
	 * Don't fork() or exec() in this, it'll close the socket!
	 *
	 */
	udp_server::udp_server(const std::string& addr, int port) : f_port(port), f_addr(addr)
	{
		char decimal_port[16];
		sprintf_s(decimal_port, sizeof(decimal_port), "%d", f_port);
		decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';

		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
	}
}