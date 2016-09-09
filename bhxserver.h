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
#include <QHash>
#include <QThread>
#include<windows.h>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include<rapidjson/document.h>
#include<QObject>
#include <QUuid>  
#include <QCryptographicHash>  
#include <QSettings>  
#include <QTime>  

using namespace rapidjson;



extern QString getMachineCode(int type);
bool isAuthrioze();
QString getMyMac();

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
void Authorize();

};
class SendMsgObj
{
public:
	SendMsgObj(bool,int, QString, QString,QString,QString);
	SendMsgObj();
	bool isSubCount;
	QString wxId;
	QString nickName;
	int msgType;
	//QString sendData;
	QString macCode;
	QString sendId;
};
class Client : public QObject
{
	Q_OBJECT
public:
	Client(QTcpSocket *theClient, QTextBrowser *tb, QTextBrowser *tb_wx_mac);
		QTcpSocket *theClient;
		QTextBrowser *tb_log;
		QTextBrowser *tb_wx_mac;
		void noticeXiaoHaoToReceiveHB(SendMsgObj data,QTcpSocket *socket);
		void xiaoHaoFindHB(SendMsgObj data, QTcpSocket *socket);
		void findMac(SendMsgObj data, QTcpSocket *socket);
		
public
slots:

void readClient();


};
class ClientObj 
{

	
public:
	ClientObj(SendMsgObj theMsgObject ,QTcpSocket *theSocket);
	ClientObj();
	QTcpSocket *theSocket;
	SendMsgObj theMsgObject;
private:

};


#endif // BHXSERVER_H
