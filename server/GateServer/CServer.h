#pragma once

#include "const.h"

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	// windows的lcp模型，事件回调
	CServer(net::io_context& ioc, unsigned short& port);
	void Start();

private:
	tcp::acceptor _acceptor;
	net::io_context& _ioc;
	tcp::socket _socket;
};

