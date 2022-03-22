#include "ChatWindow.hpp"
#include "./ui_ChatWindow.h"
#include <QDebug>
#include "Protocoll.hpp"


namespace
{
    const char* IP { "127.0.0.1" };
    const qint64 PORT { 1234 };
}

ChatWindow::ChatWindow(QWidget *parent)
    :
      QMainWindow(parent),
      ui(new Ui::ChatWindow),
      m_socket(nullptr)
{
    ui->setupUi(this);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_push_button_send_message_clicked()
{
    std::string message = ui->lineEditMessage->text().toStdString();
    std::string data = Protocol::encode_message( Protocol::Message{ m_name_client, message });

    m_socket->write(data.c_str());
}

void ChatWindow::on_push_button_connect_clicked()
{    
    if(m_socket == nullptr)
    {
        m_socket = new QTcpSocket;
        m_name_client = ui->lineEditLogin->text().toStdString();
        connect(m_socket, &QTcpSocket::connected, this, &ChatWindow::socket_connected);
        connect(m_socket, &QTcpSocket::readyRead, this, &ChatWindow::socket_ready_read);

        m_socket->connectToHost(::IP, ::PORT);
    }
}

void ChatWindow::socket_connected()
{
    qDebug() << "Server connected";
    std::string data = Protocol::encode_handshake(Protocol::Hanshake{m_name_client}).c_str();

    m_socket->write(data.c_str());
}

void ChatWindow::socket_ready_read()
{
    std::string data = m_socket->readAll().data();
    Protocol::Type type = Protocol::switch_protocol_type(data);

    switch (type)
    {
        case Protocol::Type::HANDSHAKE:
        {
            Protocol::Hanshake handshake = Protocol::decode_handshake(data);
            ui->textEditChat->append(handshake.user_name.c_str());
            break;
        }
        case Protocol::Type::MESSAGE:
        {
            Protocol::Message message = Protocol::decode_message(data);
            ui->textEditChat->append(QString::fromStdString(message.user_name + " " + message.message));
            break;
        }
        case Protocol::Type::INFO:
        {
            Protocol::Info info = Protocol::decode_info(data);
            ui->textEditChat->append(QString::fromStdString(info.user_name + " " + info.info));
            break;
        }
    }
}

void ChatWindow::on_push_button_disconnect_clicked()
{
    ui->textEditChat->append("Disconected");
    std::string info = Protocol::encode_info(Protocol::Info{ m_name_client, "Disconected"});

    m_socket->write(info.c_str());
    m_socket->disconnectFromHost();

    m_socket = nullptr;
}

