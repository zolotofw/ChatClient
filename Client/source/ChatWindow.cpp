#include "ChatWindow.hpp"
#include "./ui_ChatWindow.h"
#include "Protocoll.hpp"
#include <QDebug>
#include <QString>


namespace
{
    const char* IP { "127.0.0.1" };
    const qint64 PORT { 1234 };
}

namespace edit_text
{
    enum class COLOR
    {
        PURPLE,
        BLUE
    };

    QString generate_string_color( COLOR color, const std::string& user_name, const std::string& data = "" )
    {
        switch ( color )
        {
            case COLOR::PURPLE:
                return QString::fromStdString("<font color=\"Purple\">" + user_name + "</font>  " + data);

            case COLOR::BLUE:
                return QString::fromStdString("<font color=\"Blue\">" + user_name + "</font>  " + data);

            default:
                return QString::fromStdString("<font color=\"Purple\">" + user_name + "</font>  " + data);
        }
    }
}  // namespace

ChatWindow::ChatWindow(QWidget *parent)
    :
      QMainWindow(parent),
      ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::sl_show_text(const std::string& user_name, const std::string& message)
{
    QString text = edit_text::generate_string_color(edit_text::COLOR::PURPLE, user_name, message);
    ui->textEditChat->append(text);
}

void ChatWindow::on_push_button_send_message_clicked()
{
    std::string message = ui->lineEditMessage->text().toStdString();
    ui->lineEditMessage->clear();
    emit sg_send_new_message(m_user_name, message);
}

void ChatWindow::on_push_button_connect_clicked()
{
    m_user_name = ui->lineEditLogin->text().toStdString();

    if (m_user_name.empty())
    {
        ui->lineEditLogin->setText("Input user name");
        return;
    }

    emit sg_send_connect(m_user_name.c_str());
}

void ChatWindow::on_push_button_disconnect_clicked()
{
    emit sg_send_disconnect(m_user_name);
}
