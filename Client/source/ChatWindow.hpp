#ifndef CHATWINDOW_HPP
#define CHATWINDOW_HPP

#include <QMainWindow>
#include <QTcpSocket>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void on_push_button_send_message_clicked();
    void on_push_button_connect_clicked();
    void on_push_button_disconnect_clicked();
    void socket_connected();
    void socket_ready_read();

private:
    Ui::ChatWindow *ui;
    QTcpSocket* m_socket;
    std::string m_name_client;
};
#endif // CHATWINDOW_HPP
