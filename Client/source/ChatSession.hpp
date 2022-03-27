#ifndef CHATSESSION_HPP
#define CHATSESSION_HPP

#include <string>
#include <QObject>
#include <QThread>
#include <QTcpSocket>


class ChatSession : public QObject
{
    Q_OBJECT
public:
    explicit ChatSession(QObject* parent = nullptr);
    ~ChatSession() = default;

signals:
    void sg_read_message(const std::string& user_name, const std::string& message);
    void sg_read_info(const std::string& user_name, const std::string& message);

public slots:
   void sl_write_handshake(const std::string& user_name);
   void sl_disconected(const std::string& user_name);
   void sl_write_message(const std::string& user_name, const std::string& message);
   void sl_write_info(const std::string& user_name, const std::string& info);

private slots:
   void sl_read();

private:
    QTcpSocket* m_socket;
    QThread m_worker_thread;
};

#endif // CHATSESSION_HPP
