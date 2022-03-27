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

signals:
    void sg_send_connect(const std::string &user_name);
    void sg_send_disconnect(const std::string& user_name);
    void sg_send_new_message(const std::string user_name, const std::string& message);

public slots:
    void sl_show_text(const std::string& user_name, const std::string& message);

private slots:
    void on_push_button_send_message_clicked();
    void on_push_button_connect_clicked();
    void on_push_button_disconnect_clicked();

private:
    Ui::ChatWindow *ui;
    std::string m_user_name;
};
#endif // CHATWINDOW_HPP
