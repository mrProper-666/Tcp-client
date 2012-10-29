/**
*	���������� ����������
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

// ��������� ����� � ������������ ���� Ui
namespace Ui {
class MainWindow;
}

// ������� ����� �������� ����
class MainWindow : public QMainWindow
{
// ������(����� ��� ������-������)
    Q_OBJECT

// ��������� ���������� (�������� ��� ������)
public:
    explicit MainWindow(QWidget *parent = 0);		// ����������� (explicit - ��������� �������������� �������� ��������������� �������r����
    ~MainWindow();									// ����������

// ��������� ���������� (�������� ������ � ������)
private:
    Ui::MainWindow *ui;								// ��������� �� �����

    QTcpSocket *socket;								// ��������� �� ��������� ������ QTcpSocket
    void connectToServer();							// ������� ���������� ������ � ��������
    QHostAddress addr;								// ����� �����������
    qint16 port;									// ����
    void sendString(QTcpSocket *,QString);			// ���������� ������, ��������� - ��������� �� ����� � ������ ������

// ��������� ����� (�������� ������ �� ������)
private slots:
    void on_pushButton_clicked();					// ���� �� ����� ����� �� ������
    void errorConnection(QAbstractSocket::SocketError e);// ���� ��������� ������ ����������
    void readData();								// ���� ������ ������
    void on_pushButton_2_clicked();					// ���� �� ����� ����� �� ������
    void socketIsConnected();						// ���� ��������� ��������� ����������
};

#endif // MAINWINDOW_H
