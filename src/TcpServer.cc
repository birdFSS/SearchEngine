#include"../include/TcpServer.h"

namespace wd
{

TcpServer::TcpServer(const string& ip, unsigned short port) :
    m_acceptor(ip, port),
    m_loop(m_acceptor)
{

}
void TcpServer::start()
{
    m_acceptor.ready();
    m_loop.loop();
}

void TcpServer::setConnectionCallBack(TcpConnectionCallBack && cb)
{
    m_loop.setConnectionCallBack(std::move(cb));
}

void TcpServer::setMessageCallBack(TcpConnectionCallBack && cb)
{
    m_loop.setMessageCallBack(std::move(cb));
}

void TcpServer::setCloseCallBack(TcpConnectionCallBack && cb)
{
    m_loop.setCloseCallBack(std::move(cb));

}








}//wd
