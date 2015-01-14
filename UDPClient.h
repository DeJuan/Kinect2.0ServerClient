#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <Windows.h>
#include <io.h>
#include <stdexcept>

namespace udp_client
{
	class udp_client_runtime_error : public std::runtime_error
	{
	public:
		udp_client_runtime_error(const char *w) : std::runtime_error(w){}
	};

	class udp_client
	{
	public:
		udp_client(const std::string& addr, int port);
		~udp_client();

		int get_socket() const;
		int get_port() const;
		std::string get_addr() const;
		int recv(char *msg, size_t max_size);
		int timed_recv(char *msg, size_t max_size, int max_wait_time_millisecs);

	private:
		int f_socket;
		int f_port;
		std::string f_addr;
		struct addrinfo* f_addrinfo;
	};
}
#endif