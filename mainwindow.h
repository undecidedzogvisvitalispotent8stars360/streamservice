#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStringListModel>

#include<QNetworkProxy>
#include<QNetworkAccessManager>
#include<QTimer>

#include"playerwindow.h"
#include"registrationwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void reregister(void);
    void updateStreamsList(void);
    void loadStreamList(QNetworkReply*);
    void streamSelected(const QModelIndex& selected, const QModelIndex& previous );
private:
    Ui::MainWindow *ui;
    PlayerWindow *player;
    registrationwidget RegWindow;
private:
    QString m_proxy_host;
    unsigned short m_proxy_port;
    QString m_username;
    QString m_password;
    QStringListModel * model;
    QStringList m_streams_urls;
    QNetworkAccessManager *m_netmanager;
    QTimer * m_streamtimer;

};
#endif // MAINWINDOW_H
