/**
*	подключаем библиотеки
*/

#include <QtGui/QApplication>				
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])								// точка входа в приложение
{
    QTextCodec *codec = QTextCodec::codecForName("CP-1251");	// создаем кодек для нормальной кодировки
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);
    a.setApplicationName("Client");								// задаем имя приложения
    a.setApplicationVersion("v0.1");							// и версию
    MainWindow w;
    w.show();													// показываем главное окно
    
    return a.exec();											// запускаем приложение
}
