#include <Tracker.h>

void Tracker::start_listen_loop()
{
	//创建通信套接字
	std::shared_ptr<ip::tcp::socket> p_datasocket = make_shared<ip::tcp::socket>(io_service);
	p_accept->async_accept(*p_datasocket,[this,p_datasocket](boost::system::error_code ec)
	{
		if(ec)
		{
			//写到日志内容中 todo
			cout<<"wrong:"<<ec.message().c_str()<<endl;
		}
		else
		{
			cout<<"a new connect is coming"<<endl;
			//客户端地址族
			auto addr_str = p_datasocket->remote_endpoint().address().to_string();

			//ip
			uint32_t addrbuf;
			auto ret_value = inet_pton(AF_INET,addr_str.c_str(),(void*)&addrbuf);
			if(ret_value != 1)
			{
				//写到日志中 todo
				cout<<"wrong:inet_pton failed"<<endl;
				return;
			}
			//port
			auto port_num = p_datasocket->remote_endpoint().port();
			cout<<"ip["<<addr_str.c_str()<<"]"<<endl;
			cout<<"port["<<port_num<<"]"<<endl;

			//将客户端地址族信息，ip，端口号绑定，存到数组中
			endpoints.insert(endpoints.end(),{addr_str,addrbuf,port_num});

			//构造报文，将所有客户端信息发送给用户
			//每条信息8字节，addr[4字节] port[4字节]
			//将用户需要的信息推送过去  todo
			size_t size = sizeof(std::size_t)+endpoints.size()*8;
			using BYTE = unsigned char;
			BYTE* pbuf = new BYTE[size];
			BYTE* pbuf_bak = pbuf;

			//前四个字节：信息列表的大小
			*((size_t*)pbuf) = size;
			pbuf += sizeof(size_t);

			for(const auto &ite:endpoints)
			{
				*((uint32_t*)pbuf) = ite.host_addr_binary;
				pbuf += sizeof(uint32_t);
				*((uint32_t*)pbuf) = ite.host_port;
				pbuf += sizeof(uint32_t);
			}

			boost::asio::async_write(*p_datasocket,boost::asio::buffer((void*)pbuf_bak,size),\
					[pbuf_bak,p_datasocket](const boost::system::error_code& error,size_t size)
					{
						if(error)
						{
							//日志 todo
							cout<<"wrong: write data err:"<<error.message().c_str();
						}
						delete[] pbuf_bak;
					});
			}
			//递归启动循环，等待下一个连接
			this->start_listen_loop();
			});
}

void Tracker::start()
{
	//使用异步对象io_service 初始化一个接收器
	p_accept.reset(new ip::tcp::acceptor(io_service));
	//绑定到本地ip
	ip::tcp::endpoint endpoint(ip::address_v4::from_string("0.0.0.0"),9400);
	p_accept->open(endpoint.protocol());
	p_accept->set_option(ip::tcp::acceptor::reuse_address(true));
	p_accept->bind(endpoint);
	p_accept->listen();
	//接受客户端发来的连接
	start_listen_loop();
	io_service.run();
}

