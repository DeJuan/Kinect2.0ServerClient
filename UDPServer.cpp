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
	 * Constructor time!
	 * \param addr : The address to convert to a numeric IP.
	 * \param port : The port number to use for the connection.
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
		int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo)); //automatically get the info for the address given, error # stored in r
		if (r != 0 || f_addrinfo == NULL){ //if we couldn't get the info or we got an error return (-1), didn't work.
			throw udp_server_runtime_error(("Invalid address or port given: " + addr + ":" + decimal_port).c_str());
		}

		f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM, IPPROTO_UDP); //make socket according to protocol
		if (f_socket == -1){
			freeaddrinfo(f_addrinfo);
			throw udp_server_runtime_error(("Could not create a socket for " + addr + ":" + decimal_port).c_str());
		}

		r = bind(f_socket, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen); //bind the socket we just made
		if (r != 0)
		{
			freeaddrinfo(f_addrinfo);
			close(f_socket);
			throw udp_server_runtime_error(("Could not bind the socket to " + addr + ":" + decimal_port).c_str());
		}
	}

	/**
	 *\brief This is the destructor for the UPD server. It'll free the address info struct and close the socket before
	 * it returns.
	 */
	udp_server::~udp_server()
	{
		freeaddrinfo(f_addrinfo);
		close(f_socket);
	}

	/**
	 * \brief This will return a copy of the socket identifier, which you can use to change flags if you want. 
	 */
	int udp_server::get_socket() const
	{
		return f_socket;
	}

	/**
	* \brief This will return the port being used by the server.
	*/
	int udp_server::get_port() const
	{
		return f_port;
	}

	/** \brief Retrieve a copy of the address being used as a string.
	*
	* This function returns a copy of the address as it was specified in the
	* constructor. This does not return a canonalized version of the address.
	*
	* The address cannot be modified. If you need to send data on a different
	* address, create a new UDP server.
	*
	* \return A string with a copy of the constructor input address.
	*/
	std::string udp_server::get_addr() const
	{
		return f_addr;
	}

	/** \brief Send a message from this UDP server.
	*
	* This function sends a message through the UDP socket. The function
	* cannot be used to change the destination as it was defined when creating
	* the udp_server object.
	*
	* The size must be large enough for the message to fit. 
	*
	* \param[in] msg  The message to send.
	* \param[in] size  The number of bytes representing this message.
	*
	* \return -1 if an error occurs, otherwise the number of bytes sent. The error number
	* is set accordingly on error.
	*/
	int udp_server::send(const char *msg, size_t size)
	{
		return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
	}


}