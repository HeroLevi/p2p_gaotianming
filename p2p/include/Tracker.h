#ifndef _TRACKER_H
#define _TRACKER_H

#include <iostream>
#include <string>
#include <list>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace std;

class Tracker
{
	private:
		struct endpoint
		{
			string host_addr;
			uint32_t host_addr_binary;
			uint32_t host_port;
			endpoint(string addr,uint32_t addr_binary,uint32_t port):host_addr(addr),host_addr_binary(addr_binary),host_port(port)
			{
			}
		};
		list<endpoint> endpoints;
		boost::asio::io_service io_service;
		void start_listen_loop();
		unique_ptr<ip::tcp::acceptor> p_accept;
	public:
		void start();
};

#endif //_TRACKER_H
