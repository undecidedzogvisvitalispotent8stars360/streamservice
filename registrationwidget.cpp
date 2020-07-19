#include"registrationwidget.h"
#include<QNetworkReply>
#include<QCryptographicHash>
#include<QDateTime>
#include<QSettings>
#include<QCoreApplication>
#include"Adresses.h"

registrationwidget::registrationwidget(void) :   m_layout(  new QVBoxLayout ), m_proxy_layout( new QHBoxLayout ),
    m_nick(new QLineEdit()), m_pass(new QLineEdit()),
    m_proxy_host( new QLineEdit ), m_proxy_port( new QLineEdit ),
    m_regButton( new QPushButton ), m_image_label(new QLabel ), m_captcha_text(new QLineEdit )

{
    connect(m_regButton, SIGNAL(clicked()),this, SLOT(doRegister()) );
    m_nick->setPlaceholderText(tr("Ваш ник"));
    m_pass->setPlaceholderText(tr("Ваш пароль"));
    m_regButton->setText( tr("Регистрация") );
    m_proxy_host->setPlaceholderText( tr("Хост прокси") );
    m_proxy_port->setPlaceholderText( tr("Порт прокси") );
    m_proxy_port->setValidator( new QIntValidator(1,65535,this));
    m_proxy_port->setText("4447");
    m_proxy_host->setText("127.0.0.1");

    m_layout->addWidget(m_captcha_text);
    m_captcha_text->setPlaceholderText("captcha");
    m_captcha_text->hide();
    m_layout->addWidget(m_nick);
    m_layout->addWidget(m_pass);
    m_layout->addWidget(m_regButton);
    m_layout->addWidget(m_image_label);
    m_layout->addLayout(m_proxy_layout);
    m_proxy_layout->addWidget(m_proxy_host);
    m_proxy_layout->addWidget(m_proxy_port);
    this->setLayout(m_layout);
    m_pass->setEchoMode(QLineEdit::Password);
}
void registrationwidget::captchaLoad(QNetworkReply* data){
    QByteArray DownloadedData;
    DownloadedData.append(data->readAll());
    m_image.loadFromData(DownloadedData);
    m_image_label->setPixmap(m_image);
    m_image_label->resize(105,50);
    emit downloaded();
    m_captcha_text->show();
    m_reupload_captcha=false;
    m_regButton->setDisabled(false);
    puts("Captcha downloaded");

}
void registrationwidget:: endRegistration(QNetworkReply* data){
    emit downloaded();

    auto reply_text = data->readAll();
    if( reply_text != "Accepted" ){
        m_captcha_text->setText(reply_text);
	m_regButton->setDisabled(false);
        return;
    }
    QSettings settings("StreamService", "StreamService");
    settings.setValue("proxy/host", m_proxy_host->text());
    settings.setValue("proxy/port", m_proxy_port->text().toInt());
    settings.setValue("user/nickname", m_nick->text());
    settings.setValue("user/password", m_pass->text());
    hide();
    emit registered();
    QCoreApplication::exit(1234);
}

void registrationwidget::reuploadCaptcha(void){
    m_reupload_captcha=true;
    m_regButton->setText(tr("Перегрузить капчу"));
}
bool registrationwidget::doRegister(void){

    m_regButton->setText(tr("Регистрация"));
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName( m_proxy_host->text() );
    proxy.setPort( atoi(m_proxy_port->text().toStdString().c_str()) );
    QNetworkProxy::setApplicationProxy(proxy);//TODO: delete repeat code
    m_manager = new QNetworkAccessManager(this);
   // connect(m_manager, SIGNAL(m_manager->finished),
     //       this, SLOT( request_download() ) );
    m_manager->setProxy(proxy);
    if(m_captcha_text->isHidden() || m_reupload_captcha){
        m_session_hash = QCryptographicHash::hash(QDateTime::currentDateTime().toString().toLatin1(), QCryptographicHash::Sha256).toHex();
        auto data = m_manager->get(QNetworkRequest(QUrl(QString()+StreamService::Host+StreamService::Captcha+"?TIMESTAMP="+m_session_hash)));
        connect(
          m_manager, SIGNAL (finished(QNetworkReply*)),
          this, SLOT (captchaLoad(QNetworkReply*))
          );
        //connect( m_image_label, SIGNAL(clicked()), this, SLOT(reuploadCaptcha()) );
        m_image_label->setText("Download ... captcha...");
        if( data->error() ){
            m_image_label->setText("error... check your proxy");
            return false;
        }
    }else{
       disconnect(m_manager, SIGNAL (finished(QNetworkReply*)),
                   this, SLOT (captchaLoad(QNetworkReply*)));
        auto data = m_manager->get(QNetworkRequest(QUrl(QString()+StreamService::Host+StreamService::EndRegistration+"?hash_session="+m_session_hash+"&hash_value="+m_captcha_text->text()+"&nick="+m_nick->text()+"&password="+m_pass->text() )));
        connect(
          m_manager, SIGNAL (finished(QNetworkReply*)),
          this, SLOT (endRegistration(QNetworkReply*))
          );
        if( data->error() ){
            puts("error");
            m_image_label->setText("error... check your proxy");
            return false;
        }

    }

    m_regButton->setDisabled(true);
    return true;
}
