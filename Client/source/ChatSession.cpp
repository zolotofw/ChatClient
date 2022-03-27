#include "ChatSession.hpp"
#include "Protocoll.hpp"
#include <QDebug>


bool is_good_write_to_socket(QTcpSocket* socket, const std::string& data);

namespace
{
    const char* IP { "127.0.0.1" };
    const qint64 PORT { 1234 };
}

ChatSession::ChatSession(QObject* parent)
    :
      m_socket(new QTcpSocket())
{    
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(sl_read()));
    m_socket->connectToHost(::IP, ::PORT);
    moveToThread(&m_worker_thread);
    m_socket->moveToThread(&m_worker_thread);

    m_worker_thread.start();
}

void ChatSession::sl_write_handshake(const std::string& user_name)
{    
    std::string data = Protocol::encode_handshake(Protocol::Hanshake{ user_name });

    if (!is_good_write_to_socket(m_socket, data))
    {
        qDebug() << m_socket->errorString();
    }
}

void ChatSession::sl_disconected(const std::string &user_name)
{
    std::string data = Protocol::encode_info(Protocol::Info{ user_name, "disconected" });

    if (!is_good_write_to_socket(m_socket, data))
    {
        qDebug() << m_socket->errorString();
    }
}

void ChatSession::sl_write_message(const std::string &user_name, const std::string &message)
{
    std::string data = Protocol::encode_message(Protocol::Message{ user_name, message});

    if (!is_good_write_to_socket(m_socket, data))
    {
        qDebug() << m_socket->errorString();
    }
}

void ChatSession::sl_write_info(const std::string &user_name, const std::string &info)
{
    std::string data = Protocol::encode_info(Protocol::Info{ user_name, info});

    if(!is_good_write_to_socket(m_socket, data))
    {
        qDebug() << m_socket->errorString();
    }

}

void ChatSession::sl_read()
{
    std::string data = m_socket->readAll().data();
    Protocol::Type type = Protocol::switch_protocol_type(data);

    switch (type)
    {
//        case Protocol::Type::HANDSHAKE:
//        {
//            Protocol::Hanshake handshake = Protocol::decode_handshake(data);
//            break;
//        }
        case Protocol::Type::MESSAGE:
        {
            Protocol::Message message = Protocol::decode_message(data);
            emit sg_read_message(message.user_name, message.message);
            break;
        }
        case Protocol::Type::INFO:
        {
            Protocol::Info info = Protocol::decode_info(data);
            emit sg_read_info(info.user_name, info.info);
            break;
        }
        default:
        break;
    }
}

bool is_good_write_to_socket(QTcpSocket* socket, const std::string& data)
{
    qint64 size = socket->write(data.c_str());

    return size < 0;
}
