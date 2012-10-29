/**
*	подключаем библиотеки
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"					// автоматически сгенерирована, содержит элементы формы из дизайнера

/**
*	конструктор класса MainWindow
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	// устанавливаем заголовок окна
    setWindowTitle(qApp->applicationName() + ' ' + qApp->applicationVersion());

	// создаем экземпляр класса QTcpSocket
    socket = new QTcpSocket(this);
	
	// макрос для выполнения слота чтения данных по сигналу готовности сокета
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
	
	// макрос для выполнения обработки ошибки соединения, после сигнала ошибки сокета
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(errorConnection(QAbstractSocket::SocketError)));
			
	// макрос для выполнения слота соединения после сигнла а сокета об успешном соединении с сервером
    connect(socket,SIGNAL(connected()),this,SLOT(socketIsConnected()));

	// устанавливаем маску для корректного ввода IP адреса
    ui->lineEdit->setInputMask("000.000.000.000:0000");
	
	// устанавливаем валидатор ввода в QLineEdit для ввода только цифр и пробелов
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^[0-9\\s]*$"), this);
    ui->lineEdit_2->setValidator(validator);
}

/**
*	деструктор класса MainWindow
*/
MainWindow::~MainWindow()
{
	// удаляем указатель на форму
    delete ui;
}

/**
*	функция подключения к серверу
*/
void MainWindow::connectToServer(){
		// получаем порт из введенного пользователем IP адреса
        port = ui->lineEdit->text().remove(0,ui->lineEdit->text().indexOf(":")+1).toInt();
		
		// получаем адрес сервера из введенного пользователем IP адреса
        addr.setAddress(ui->lineEdit->text().remove(ui->lineEdit->text().indexOf(":"),
                                                    ui->lineEdit->text().length()));
		
		// если адрес пустой
        if (addr.isNull()) {
		
			// выводим сообщение о том, что адрес стоит вводить внимательнее
            QMessageBox::critical(this,qApp->applicationName(),
                                  tr("Ошибка подключения. Введите корректный адрес\n"
                                     "например 127.0.0.1:666"),
                                  QMessageBox::Ok);
		// иначе
        } else {
			// соединяем сокет по полученным адресу и порту для чтения/записи
            socket->connectToHost(addr,port,QIODevice::ReadWrite);
			
			// добавляем сообщение о соединении для пользователя
            ui->listWidget->addItem(QString("%1 Подключение к %2:%3")
                                    .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
                                    .arg(addr.toString())
                                    .arg(port));
									
			// выключаем кнопку для подключения
            ui->pushButton->setEnabled(false);
        }
}

/**
*	слот, выполняющийся по клику мышкой на кнопке "Соединиться"
*/
void MainWindow::on_pushButton_clicked()
{
	// вызываем метод
    connectToServer();
}

/**
*	слот для обработки ошибок соединения с сервером
*/
void MainWindow::errorConnection(QAbstractSocket::SocketError e){
	// если сокет соединяется
    if (socket->state() == QAbstractSocket::ConnectingState){
		
		// порвать соединение
        socket->disconnectFromHost();
		
	// иначе
    } else {
	
		// выходим из функции
        return;
    }
	
	// включаем кнопку соединения
    ui->pushButton->setEnabled(true);

	// выводим сообщение об ошибке соединения
    QMessageBox::critical(this,qApp->applicationName(),
                          tr("Ошибка подключения\n Возможно сервер не отвечает, или не запущен\n" +
                             e),
                          QMessageBox::Ok);
}

/**
*	функция получения данных с сервера
*/
void MainWindow::readData(){
	// инициализируем поток данных из сокета
    QDataStream in(socket);
	
	// указываем версию для полной совместимости с сервером
    in.setVersion(QDataStream::Qt_4_0);

	// обычная строка
    QString dataString;
	
	// получаем строку из потока
    in >> dataString;

	// создаем элемент QListWidget'а, отображающий полученные данные с сервера для пользователя
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setText(QString("%1 >> %2")
               .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
               .arg(dataString));
    item->setBackgroundColor(QColor(Qt::green));
	
	// добавляем элементы
    ui->listWidget->addItem(item);
}

/**
*	функция отправки строки на сервер
*/
void MainWindow::sendString(QTcpSocket *sock,QString string){
	// создаем элемент QListWidget'а, отображающий отправляемые данные на сервер для пользователя
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setText(QString("%1 << %2")
               .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
               .arg(string));
    item->setBackgroundColor(QColor(Qt::blue));
	
	// добавляем элемент
    ui->listWidget->addItem(item);

	// инициализируем массив данных
    QByteArray block;
	
	// инициализируем поток данных для отправки данных на сервер
    QDataStream out(&block, QIODevice::WriteOnly);

	// указываем версию для совместимости
    out.setVersion(QDataStream::Qt_4_0);
	
	// отправляем данные в поток
    out << string;
	
	// переходим к началу потока
    out.device()->seek(0);
	
	// отправляем данные
    sock->write(block);
}

/**
*	слой, выполняющийся по клике мышкой на кнопке "Отправить"
*/
void MainWindow::on_pushButton_2_clicked()
{
	// вызываем функцию отправки данных на версер
    sendString(socket,ui->lineEdit_2->text());
}

/**
*	слот, выполняющийся после удачного соединения с сервером
*/
void MainWindow::socketIsConnected(){
	// создаем элемент QListWidget'а с сообщением об удачном подключении
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setText(QString("%1 %2")
               .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate))
                  .arg("Успешное соедниение с сервером"));
    item->setBackgroundColor(QColor(Qt::blue));
	
	// добавляем элемент
    ui->listWidget->addItem(item);
	
	// включаем кнопку соединения
    ui->pushButton->setEnabled(true);
	
	// включаем кнопку отправки
    ui->pushButton_2->setEnabled(true);
}
