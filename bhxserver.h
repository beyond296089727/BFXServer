#ifndef BHXSERVER_H
#define BHXSERVER_H
#include <rapidjson\document.h>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonDocument>
#include <QtWidgets/QMainWindow>
#include "ui_bhxserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QHostInfo>
#include <QTextBrowser>
#include <QJsonValue>



//#include <QNetworkInterface>



class BHXServer : public QMainWindow
{
	Q_OBJECT

public:
	BHXServer(QWidget *parent = 0);
	~BHXServer();
	QTcpServer *server;
	QTcpSocket *clientConnection;
	QHostAddress getLocalHostIP();
	
private:
	Ui::BHXServerClass ui;

public 
slots:
void startServer();
public
slots:
void acceptConnection();

public
slots:
void cleanLog();


public
slots:
void sendMsg();
};
class SendMsgObj
{
public:
	bool isSubCount;
	QString wxId;
	QString nickName;
	int msgType;
	QString sendData;
	QString macCode;
};
class Client : public QObject
{
	Q_OBJECT
public:
	Client(QTcpSocket *theClient, QTextBrowser *tb);
		QTcpSocket *theClient;
		QTextBrowser *tb_log;
		void noticeXiaoHaoToReceiveHB(SendMsgObj data,QTcpSocket *socket);
		void xiaoHaoFindHB(SendMsgObj data, QTcpSocket *socket);
public
slots:

void readClient();


};
class ClientObj 
{

	
public:
	QTcpSocket *theSocket;
	SendMsgObj theMsgObject;
private:

};



#endif // BHXSERVER_H
