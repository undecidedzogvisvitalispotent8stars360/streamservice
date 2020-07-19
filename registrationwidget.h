#ifndef REGISTRATIONWIDGET_H
#define REGISTRATIONWIDGET_H
#include<QWidget>
#include<QBoxLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QIntValidator>
#include<QNetworkProxy>
#include<QNetworkAccessManager>
#include<QPixmap>
#include<QLabel>

class registrationwidget : public QWidget
{
Q_OBJECT


public:
    registrationwidget(void);
protected slots:
    bool doRegister(void);
    void captchaLoad(QNetworkReply* pReply);
    void endRegistration(QNetworkReply* pReply);


signals:
 void downloaded();
 void registered();
private slots:
 void reuploadCaptcha(void);
private:
 bool m_reupload_captcha;
 protected:
    QVBoxLayout* m_layout;
    QHBoxLayout* m_proxy_layout;
    QLineEdit * m_nick;
    QLineEdit * m_pass;
    QLineEdit * m_proxy_host;
    QLineEdit * m_proxy_port;
    QPushButton * m_regButton;
    QNetworkAccessManager *m_manager;
    QPixmap m_image;
    QLabel * m_image_label;
    QLineEdit * m_captcha_text;
    QByteArray m_session_hash;
};

#endif // REGISTRATIONWIDGET_H
