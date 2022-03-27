#include "source/ChatWindow.hpp"
#include "source/ChatSession.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "Client_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    ChatWindow chat_window;
    ChatSession chat_session;

    QObject::connect(&chat_window, SIGNAL(sg_send_connect(const std::string&)), &chat_session, SLOT(sl_write_handshake(const std::string&)));
    QObject::connect(&chat_window, SIGNAL(sg_send_disconnect(const std::string&)), &chat_session, SLOT(sl_disconected(const std::string&)));
    QObject::connect(&chat_window, SIGNAL(sg_send_new_message(const std::string&, const std::string&)), &chat_session, SLOT(sl_write_message(const std::string&, const std::string&)));
    QObject::connect(&chat_session, SIGNAL(sg_read_message(const std::string&, const std::string&)), &chat_window, SLOT(sl_show_text(const std::string&, const std::string&)));
    QObject::connect(&chat_session, SIGNAL(sg_read_info(const std::string&, const std::string&)), &chat_window, SLOT(sl_show_text(const std::string&, const std::string&)));

    chat_window.show();
    return a.exec();
}
