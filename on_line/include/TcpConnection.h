#pragma once
#include "Socket.h"
#include "InetAddress.h"
#include "SocketIO.h"
#include "Noncopyable.h"
#include <functional>
#include <string>
#include <memory>
#define TRAIN_HEAD 4
#define TRAIN_BUF_SIZE 65536

using std::string;
namespace wd
{
class TcpConnection;
class EventLoop;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallBack = std::function<void(const TcpConnectionPtr&)>;

struct Train
{
    int _size;
    char _buf[TRAIN_BUF_SIZE];
};

class TcpConnection :
    Noncopyable,
    public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd, EventLoop *); 
    ~TcpConnection();

    string receive();
    void send(const string& msg);
    void sendInLoop(const string &msg);

    string toString() const;                       
    void shutdown();

    int getPeerFd() const
    { return m_sock.getFd(); }

#if 1
    //这里先传递了EventLoop的成员 函数对象的引用过来，然后又调用std::move
    //从语法上讲std::move后的对象不应该再使用了。也就是说不该使用Eventloop中的三个成员 函数对象
    //但是每建立一个新连接都需要做一次set回调函数，那么这就有问题了
    //所有tcp连接的对象可能跟EventLoop中的函数保持一致，即如果EventLoop改变，其他的也会变
    void setConnectionCallBack(const TcpConnectionCallBack & cb)    //这个传递后，EventLoop那个就不能用了
    { m_onConnection = std::move(cb); }
    void setMessageCallBack(const TcpConnectionCallBack & cb)
    { m_onMessage= std::move(cb); }
    void setCloseCallBack(const TcpConnectionCallBack & cb)
    { m_onClose= std::move(cb); }
#endif
    void handleConnectionCallBack();
    void handleMessageCallBack();
    void handleCloseCallBask();
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket m_sock;
    SocketIO m_socketIO;
    InetAddress m_localAddr;
    InetAddress m_peerAddr;
    bool m_isShutdownWrite;
    EventLoop * m_loop;

    //单个Tcp连接的回调函数
    TcpConnectionCallBack m_onConnection;
    TcpConnectionCallBack m_onMessage;
    TcpConnectionCallBack m_onClose;
};







}//end of namespace wd

