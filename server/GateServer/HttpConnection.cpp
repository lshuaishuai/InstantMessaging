#include "HttpConnection.h"
#include "LogicSystem.h"

HttpConnection::HttpConnection(tcp::socket socket)
	: _socket(std::move(socket))
{

}

void HttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec, std::size_t bytes_transforred) {
		try
		{
			if (ec)
			{
				std::cout << "http read err is " << ec.what() << std::endl;
				return;
			}

			boost::ignore_unused(bytes_transforred);
			self->HandleReq();
			self->CheckDeadline();

		}
		catch (std::exception& exp)
		{
			std::cout << "exception is " << exp.what() << std::endl;
		}
		});
}

void HttpConnection::CheckDeadline()
{
	auto self = shared_from_this();
	_deadline.async_wait([self](beast::error_code ec) {
		if (!ec)
		{
			// TODO: 会造成timewait，可以先self->_socket.shutdown(tcp::socket::shutdown_both, ec); 
			// 服务端向客户端发送 FIN 包，告诉客户端「我这边不再收发数据了」 客户端会成为「主动关闭连接的一方」，客户端会回复服务端 FIN+ACK，最后由客户端完成四次挥手的最后一步；
			self->_socket.close(ec);
		}
		});
}

void HttpConnection::WriteResponse()
{
	auto self = shared_from_this();
	_response.content_length(_response.body().size());
	http::async_write(_socket, _response, [self](beast::error_code ec, std::size_t bytes_transforred) {
		self->_socket.shutdown(tcp::socket::shutdown_send, ec);
		self->_deadline.cancel();
		});
}

void HttpConnection::HandleReq()
{
	// 设置版本
	_response.version(_request.version());
	_response.keep_alive(false);
	if (_request.method() == http::verb::get)
	{
		bool success = LogicSystem::GetInstance()->HandleGet(_request.target(), shared_from_this());
		if (!success)
		{
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}
}
