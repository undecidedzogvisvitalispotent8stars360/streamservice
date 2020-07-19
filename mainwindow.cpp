#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSettings>
#include<QInputDialog>
#include<QMessageBox>
#include<QBoxLayout>
#include<QNetworkReply>
#include"Adresses.h"

void MainWindow::reregister(void){
    RegWindow.show();
}
void MainWindow::loadStreamList(QNetworkReply * reply){
    //m_streams_urls.clear();
    //m_streams_urls.removeDuplicates();

    auto addresses = reply->readAll().split('\n');

    m_streams_urls << "rtmp://localhost/live/test";

    for( auto url : addresses ){
        m_streams_urls << url ;
        puts( url.toStdString().c_str() );
    }

    model->setStringList(m_streams_urls);

}
void MainWindow::updateStreamsList(void){
    //puts("...");

    auto data = m_netmanager->get(QNetworkRequest(QUrl(QString()+StreamService::Host+StreamService::StreamList)));
    connect(
      m_netmanager, SIGNAL (finished(QNetworkReply*)),
      this, SLOT (loadStreamList(QNetworkReply*))
      );
    QMessageBox errBox;
    errBox.setText(tr("Проверьте прокси. Не могу подключиться к серверу."));
    if( data->error() )    errBox.exec();
    m_streamtimer->start(50000);

}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), model(new QStringListModel(this))
{
    ui->setupUi(this);
    player=new PlayerWindow(this);
    player->hide();
    //QPushButton re_register(this);
    //re_register.setText("ReRegister");
    //player.show();
    //
    QSettings settings("StreamService", "StreamService");
    //connect( RegWindow, SIGNAL( registered() ), this, SLOT( auth() ) )
    m_proxy_host = settings.value("proxy/host").toString();
    m_proxy_port = settings.value("proxy/port").toInt();
    m_username = settings.value("user/nickname").toString();
    if(m_proxy_host == "")RegWindow.show();
    else{
	bool ok;
	show();
        m_password=QInputDialog::getText(this, tr("Авторизация"),
                                     tr("Пароль:"), QLineEdit::Normal,
                                     "", &ok);;
        m_netmanager=new QNetworkAccessManager(this);
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName( m_proxy_host );
        proxy.setPort( m_proxy_port );
        QNetworkProxy::setApplicationProxy(proxy);//TODO: delete repeat code
        m_netmanager->setProxy(proxy);
        m_streamtimer = new QTimer(this);

       // connect(m_streamtimer, SIGNAL(timeout()), this, SLOT(updateStreamsList()) );//DDD
        updateStreamsList();
        //m_streamtimer->start(50000);
        //TODO: though button, without timers;
    }
    //connect(&re_register, SIGNAL(clicked()), this, SLOT(reregister()) );
   // this->layout()->addWidget(player);
   // this->layout()->addWidget(&re_register);
    connect(this->ui->reregister, SIGNAL(triggered()), this, SLOT(reregister()) );
    this->ui->player_widget=player;
    player->setGeometry(220,0,461,301);
    //this->ui->centralwidget->layout()->addWidget(player);
    //this->ui->player_widget->layout()->addWidget(player);//->layout()->addWidget(player);
    player->show();
    this->ui->listView->setModel(model);

    connect(this->ui->listView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(streamSelected(QModelIndex,QModelIndex)));
    this->ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void MainWindow::streamSelected(const QModelIndex& selected, const QModelIndex& previous ){
    puts( selected.data().toString().toStdString().c_str() );
    player->openMedia( selected.data().toString() );
    previous == previous ? 1 : puts("MAGIC");
}
MainWindow::~MainWindow()
{
    delete ui;
}

