/**
*	подключаем библиотеки
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QMessageBox>
#include <QHostAddress>
#include <QtDebug>
#include <QAbstractSocket>
#include <QDateTime>

// объ€вл€ем класс в пространстве имен Ui
namespace Ui {
class MainWindow;
}

// создаем класс главного окна
class MainWindow : public QMainWindow
{
// макрос(нужен дл€ сигнал-слотов)
    Q_OBJECT

// публичные объ€влени€ (доступны вне класса)
public:
    explicit MainWindow(QWidget *parent = 0);		// конструктор (explicit - запрещает автоматическое создание конвертирующего конструrтора
    ~MainWindow();									// деструктор

// приватные объ€влени€ (доступны только в классе)
private:
    Ui::MainWindow *ui;								// указатель на форму

    QTcpSocket *socket;								// указатель на экземпл€р класса QTcpSocket
    void connectToServer();							// функци€ соединени€ сокета с сервером
    QHostAddress addr;								// адрес подключени€
    qint16 port;									// порт
    void sendString(QTcpSocket *,QString);			// отправл€ем строку, аргументы - указатель на сокет и строка данных

// приватные слоты (доступны только из класса)
private slots:
    void on_pushButton_clicked();					// слот по клику мышки на кнопке
    void errorConnection(QAbstractSocket::SocketError e);// слот обработки ошибки соединени€
    void readData();								// слот чтени€ данных
    void on_pushButton_2_clicked();					// слот по клику мышки на кнопке
    void socketIsConnected();						// слот обработки успешного соединени€
};

#endif // MAINWINDOW_H
