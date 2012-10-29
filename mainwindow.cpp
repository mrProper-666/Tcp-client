/**
*	���������� ����������
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"					// ������������� �������������, �������� �������� ����� �� ���������

/**
*	����������� ������ MainWindow
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	// ������������� ��������� ����
    setWindowTitle(qApp->applicationName() + ' ' + qApp->applicationVersion());

	// ������� ��������� ������ QTcpSocket
    socket = new QTcpSocket(this);
	
	// ������ ��� ���������� ����� ������ ������ �� ������� ���������� ������
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
	
	// ������ ��� ���������� ��������� ������ ����������, ����� ������� ������ ������
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(errorConnection(QAbstractSocket::SocketError)));
			
	// ������ ��� ���������� ����� ���������� ����� ������ � ������ �� �������� ���������� � ��������
    connect(socket,SIGNAL(connected()),this,SLOT(socketIsConnected()));

	// ������������� ����� ��� ����������� ����� IP ������
    ui->lineEdit->setInputMask("000.000.000.000:0000");
	
	// ������������� ��������� ����� � QLineEdit ��� ����� ������ ���� � ��������
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^[0-9\\s]*$"), this);
    ui->lineEdit_2->setValidator(validator);
}

/**
*	���������� ������ MainWindow
*/
MainWindow::~MainWindow()
{
	// ������� ��������� �� �����
    delete ui;
}

/**
*	������� ����������� � �������
*/
void MainWindow::connectToServer(){
		// �������� ���� �� ���������� ������������� IP ������
        port = ui->lineEdit->text().remove(0,ui->lineEdit->text().indexOf(":")+1).toInt();
		
		// �������� ����� ������� �� ���������� ������������� IP ������
        addr.setAddress(ui->lineEdit->text().remove(ui->lineEdit->text().indexOf(":"),
                                                    ui->lineEdit->text().length()));
		
		// ���� ����� ������
        if (addr.isNull()) {
		
			// ������� ��������� � ���, ��� ����� ����� ������� ������������
            QMessageBox::critical(this,qApp->applicationName(),
                                  tr("������ �����������. ������� ���������� �����\n"
                                     "�������� 127.0.0.1:666"),
                                  QMessageBox::Ok);
		// �����
        } else {
			// ��������� ����� �� ���������� ������ � ����� ��� ������/������
            socket->connectToHost(addr,port,QIODevice::ReadWrite);
			
			// ��������� ��������� � ���������� ��� ������������
            ui->listWidget->addItem(QString("%1 ����������� � %2:%3")
                                    .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
                                    .arg(addr.toString())
                                    .arg(port));
									
			// ��������� ������ ��� �����������
            ui->pushButton->setEnabled(false);
        }
}

/**
*	����, ������������� �� ����� ������ �� ������ "�����������"
*/
void MainWindow::on_pushButton_clicked()
{
	// �������� �����
    connectToServer();
}

/**
*	���� ��� ��������� ������ ���������� � ��������
*/
void MainWindow::errorConnection(QAbstractSocket::SocketError e){
	// ���� ����� �����������
    if (socket->state() == QAbstractSocket::ConnectingState){
		
		// ������� ����������
        socket->disconnectFromHost();
		
	// �����
    } else {
	
		// ������� �� �������
        return;
    }
	
	// �������� ������ ����������
    ui->pushButton->setEnabled(true);

	// ������� ��������� �� ������ ����������
    QMessageBox::critical(this,qApp->applicationName(),
                          tr("������ �����������\n �������� ������ �� ��������, ��� �� �������\n" +
                             e),
                          QMessageBox::Ok);
}

/**
*	������� ��������� ������ � �������
*/
void MainWindow::readData(){
	// �������������� ����� ������ �� ������
    QDataStream in(socket);
	
	// ��������� ������ ��� ������ ������������� � ��������
    in.setVersion(QDataStream::Qt_4_0);

	// ������� ������
    QString dataString;
	
	// �������� ������ �� ������
    in >> dataString;

	// ������� ������� QListWidget'�, ������������ ���������� ������ � ������� ��� ������������
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setText(QString("%1 >> %2")
               .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
               .arg(dataString));
    item->setBackgroundColor(QColor(Qt::green));
	
	// ��������� ��������
    ui->listWidget->addItem(item);
}

/**
*	������� �������� ������ �� ������
*/
void MainWindow::sendString(QTcpSocket *sock,QString string){
	// ������� ������� QListWidget'�, ������������ ������������ ������ �� ������ ��� ������������
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setText(QString("%1 << %2")
               .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
               .arg(string));
    item->setBackgroundColor(QColor(Qt::blue));
	
	// ��������� �������
    ui->listWidget->addItem(item);

	// �������������� ������ ������
    QByteArray block;
	
	// �������������� ����� ������ ��� �������� ������ �� ������
    QDataStream out(&block, QIODevice::WriteOnly);

	// ��������� ������ ��� �������������
    out.setVersion(QDataStream::Qt_4_0);
	
	// ���������� ������ � �����
    out << string;
	
	// ��������� � ������ ������
    out.device()->seek(0);
	
	// ���������� ������
    sock->write(block);
}

/**
*	����, ������������� �� ����� ������ �� ������ "���������"
*/
void MainWindow::on_pushButton_2_clicked()
{
	// �������� ������� �������� ������ �� ������
    sendString(socket,ui->lineEdit_2->text());
}

/**
*	����, ������������� ����� �������� ���������� � ��������
*/
void MainWindow::socketIsConnected(){
	// ������� ������� QListWidget'� � ���������� �� ������� �����������
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setText(QString("%1 %2")
               .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
                  .arg("�������� ���������� � ��������"));
    item->setBackgroundColor(QColor(Qt::blue));
	
	// ��������� �������
    ui->listWidget->addItem(item);
	
	// �������� ������ ����������
    ui->pushButton->setEnabled(true);
	
	// �������� ������ ��������
    ui->pushButton_2->setEnabled(true);
}
