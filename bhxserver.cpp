#include "bhxserver.h"
#include"jsk_json.h"
using namespace JSK;

#pragma execution_character_set("utf-8")


QList<ClientObj> *clientList;
QTcpSocket* theclient;
QHash<QString, QList<ClientObj>*>* HBMap;

BHXServer::BHXServer(QWidget *parent)
	: QMainWindow(parent)
{
	server = new QTcpServer();
	ui.setupUi(this);
	connect(ui.pb_startserver,SIGNAL(clicked()),this,SLOT(startServer()));
	///connect(ui.pb_send_msg, SIGNAL(clicked()), this, SLOT(sendMsg()));
	connect(ui.pb_clean_log, SIGNAL(clicked()), this, SLOT(cleanLog()));
	//server->listen(QHostAddress::Any, 6665);
	//connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
	clientList = new QList<ClientObj>();
	HBMap = new QHash<QString, QList<ClientObj>*>();
	QHostAddress ip = getLocalHostIP();
	ui.lb_ip->setText(ip.toString());

	

}

BHXServer::~BHXServer()
{

}

void BHXServer::acceptConnection()
{
	clientConnection = server->nextPendingConnection();
	QObject::connect(clientConnection, SIGNAL(readyRead()), new Client(clientConnection,ui.tb_log), SLOT(readClient()));
}

void BHXServer::cleanLog()
{
	ui.tb_log->setText("");
}
void BHXServer::startServer()
{
	
	if (server==NULL||!server->isListening())
	{
		server->listen(QHostAddress::Any, 6665);
		connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
		ui.tb_log->append("启动服务器成功");
		return;
	}
	ui.tb_log->append("服务器已经启动");
}

QHostAddress BHXServer::getLocalHostIP()
{
	//QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
	//QHostAddress result;
	//foreach(QHostAddress address, AddressList) {
	//	if (address.protocol() == QAbstractSocket::IPv4Protocol &&
	//		address != QHostAddress::Null &&
	//		address != QHostAddress::LocalHost) {
	//		if (address.toString().contains("127.0.")) {
	//			continue;
	//		}
	//		result = address;
	//		break;
	//	}
	//}
	//
	//return result;

	QHostAddress address;
	QString localHostName = QHostInfo::localHostName();
	QHostInfo info = QHostInfo::fromName(localHostName);
	 //info.addresses();

	 foreach(QHostAddress address, info.addresses())
	 {
		 if (address.protocol() == QAbstractSocket::IPv4Protocol)
		 {
			 if (address.toString().contains("192."))
			 {
				 return address;
			 }
		 }
			// qDebug() << address.toString();
	 }

	 return address;
}

void BHXServer::sendMsg()
{
	//QString msg = ui.te_msg->toPlainText();
	//	for (int i = 0; i < clientList->size(); i++)
	//	{
	//		QTcpSocket * t = clientList->takeAt(i);
	//			t->write(msg.toStdString().c_str());	
	//	}
}

Client ::Client(QTcpSocket *theClient,QTextBrowser *tb)
{
	this->theClient = theClient;
	this->tb_log = tb;
}


void Client::readClient()
{

	QString data = theClient->readAll();
	/*tb_log->append("收到消息:" + str);
	QByteArray ba=str.toLatin1();
	char *str_ch = ba.data();
	if (str == "小号")
	{
		theclient = theClient;
		clientList->append(theClient);
		return;
	
	}


	for (int a=0;a<clientList->size();a++)
	{
		QTcpSocket *sochet = clientList->takeAt(a);
		if (sochet != NULL&&sochet->isOpen())
		{

			int wsize = sochet->write(str_ch);
			theclient->flush();
			QString msg = "向小号发送:";
			tb_log->append(msg + str_ch);


		}

	}
	
	clientList->clear();*/

	//JSON解析
	
	//QByteArray dataarray = data.toLatin1();
	//char *char_data = dataarray.data();
	//QJsonParseError error;
	//QJsonDocument json = QJsonDocument::fromJson(dataarray, &error);
	//if (error.error != QJsonParseError::NoError)
	//{
	//	//json解析错误
	//	tb_log->append("数据JSON解析错误");
	//	return;
	//}
	//QJsonObject jo = json.object();
	//QJsonValue  isSubCount = jo.take("isSubCount");
	//QJsonValue  wxId = jo.take("wxId");
	//QJsonValue  nickName = jo.take("nickName");
	////QJsonValue  sendData = jo.take("sendData");
	//QJsonValue  macCode = jo.take("macCode");
	//QJsonValue  msgType = jo.take("msgType");
	//QJsonValue  sendId = jo.take("sendId");
	//
	//bool isSubCountV = (bool)isSubCount.toBool();
	//QString macCodeV = macCode.toString();
	//QString wxIdV = wxId.toString();
	//int msgTypeV = msgType.toInt();
	//QString nickNameV = nickName.toString();
	//QString sendIdV = sendId.toString();

	/*JSONObject jo;
	if (!jo.fromString(data))
	{
		
	}
	*/

	Document document;
	if (document.Parse(data.toStdString().c_str()).HasParseError())
	{
	
		tb_log->append("数据JSON解析错误");
		return;
	}

	QString wxId = "";
	if (document.HasMember("wxId"))
	{
		wxId = document["wxId"].GetString();
	}
	QString macCode ="";
	if (document.HasMember("macCode"))
	{
		macCode = document["macCode"].GetString();
	}
	
	bool isSubCount = false;
	if (document.HasMember("isSubCount"))
	{
		isSubCount = document["isSubCount"].GetBool();
	}
	int msgType = -1;
	if (document.HasMember("msgType"))
	{
		msgType = document["msgType"].GetInt();
	}
	QString nickName = "";
	if (document.HasMember("nickName"))
	{
		nickName = document["nickName"].GetString();
	}
	QString sendId ="";
	if (document.HasMember("sendId"))
	{
		sendId = document["sendId"].GetString();
	}

	tb_log->append("收到" + nickName+"的消息:\n"+ data+"\n");
	//tb_log->append("sendDataV="+sendDataV);
	SendMsgObj msgJsonObj(isSubCount,msgType,wxId,nickName, macCode, sendId);
	switch (msgType)
	{
	case 1:
		xiaoHaoFindHB(msgJsonObj, theClient);
		break;
	case 2:
		noticeXiaoHaoToReceiveHB(msgJsonObj,theClient);
		break;
	
	default:
		break;
	}


}

void Client::noticeXiaoHaoToReceiveHB(SendMsgObj data, QTcpSocket *socket)
{
	//启动新线程通知小号抢包
	/*NoTiceXiaoHaoTask *newTask=new NoTiceXiaoHaoTask(data,tb_log);
	newTask->start();*/
	QString sendid = data.sendId;
	QString wxid = data.wxId;
	QString nickname = data.nickName;
	//QString reqdata = data.sendData;
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName(".\\database.db");
	QString create_table = "create table hbrecoreds(sendid nvchar(30) ,wxid nvchar(30),nickname nvchar(30))";

	


	if (!database.isOpen())
	{
		bool i = database.open();
		if (!i)
		{
			tb_log->append("数据据打开失败");
		}
	}

	QString args = "('" + sendid + "','" + wxid + "','"+nickname+"')";
	QString sql = "insert into hbrecoreds(sendid ,wxid ,nickname ) values" + args;
	//insert into hbrecoreds(sendid ,wxid ,nickname ) values('5565','wx_id1552','fager');
	QSqlQuery query(database);
	QSqlError error = query.lastError();
	//qDebug() << "From mysql database: " + error.databaseText();
	bool b=query.exec(sql);
	if (!b)
	{
		tb_log->append(error.databaseText());
	}

	if (clientList->isEmpty())
	{
		return;
	}


	for (int a=0;a<clientList->size();a++)
	{
	
		ClientObj theClient=clientList->takeAt(a);
		QTcpSocket *socket=theClient.theSocket;
		SendMsgObj o=theClient.theMsgObject;
		QString nn = o.nickName;

		QJsonObject jo;
		jo.insert("from", nickname);
		jo.insert("type", 1);
		jo.insert("sendid", sendid);
		QJsonDocument jd(jo);
		QByteArray ba = jd.toJson();

		
		int size = socket->write(ba);
		
		tb_log->append(nickname+"向" + nn + "发送:\n" + sendid + "抢包命令\n");

	}
	

}
void  Client::xiaoHaoFindHB(SendMsgObj data, QTcpSocket *socket)
{
	ClientObj theClient(data,socket);
	QString sendId = data.sendId;
	QString nn = data.nickName;
	theClient.theMsgObject = data;
	theClient.theSocket = socket;

	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName(".\\database.db");
	
	if (!database.isOpen())
	{
		bool i = database.open();
		if (!i)
		{
			tb_log->append("数据据打开失败");
		}
	}
	

	QSqlQuery query;
	query.exec("select * from hbrecoreds where sendid='" + sendId + "'");
	if (query.first())
	{
		QString wxid=query.value("wxid").toString();
		QString sendid =query.value("sendid").toString();
		QString nickname =query.value("nickname").toString();

		QJsonObject jo;
		jo.insert("from", nickname);
		jo.insert("type", 1);
		jo.insert("sendid", sendid);
		QJsonDocument jd(jo);
		QByteArray ba = jd.toJson();
		
		int size = socket->write(ba);
		
		tb_log->append("向" + nn + "发送:\n" + sendid + "抢包命令\n");
		

	}
	else
	{
		clientList->append(theClient);
	}
	
	//if (HBMap->contains(sendId))
	//{
	//
	//	QList<ClientObj> *hblist = HBMap->value(sendId);
	//	hblist->append(theClient);
	//	

	//}
	//else
	//{
	//	QList<ClientObj> *hblist = new QList<ClientObj>;
	//	HBMap->insert(sendId, hblist);
	//	hblist->append(theClient);
	//}
	
	//clientList->append(theClient);

}

SendMsgObj::SendMsgObj(bool isSubCount, int msgType, QString wxId, QString nickName,  QString macCode,QString sendId)

{
	this->isSubCount = isSubCount;
	this->msgType = msgType;
	this->wxId = wxId;
	this->nickName = nickName;
	//this->sendData = sendData;
	this->macCode = macCode;
	this->sendId = sendId;

}

SendMsgObj::SendMsgObj()
{

	
}

 ClientObj::ClientObj(SendMsgObj theMsgObject, QTcpSocket *theSocket)
{
	this->theMsgObject = theMsgObject;
	this->theSocket = theSocket;
}

 ClientObj::ClientObj()
 {

 }

 NoTiceXiaoHaoTask::NoTiceXiaoHaoTask( SendMsgObj data, QTextBrowser *tb)
 {
	 //this->hblist = hblist;
	 this->mysendid = data.sendId;
	 this->data = data;
	 this->tb = tb;
	// this->HBMap = HBMap;
 }

 void NoTiceXiaoHaoTask::run()
 {

	 if (!HBMap->contains(mysendid))
	 {
		 return;
	 }

	 QList<ClientObj> *list= HBMap->value(mysendid);
	 QString HostnickName = data.nickName;
	 QString Hostwxid = data.wxId;
	 //QString reqData = data.sendData;
	// QByteArray ba = reqData.toLatin1();
	// char *sendD = ba.data();
	 //最多通知次数为10
	 for (int b=0;b<10;b++)
	 {
	 
		 tb->append("通知次数:"+QString::number(b));
		 for (int a = 0; a<list->size(); a++)
		 {
			 ClientObj theClient = list->takeAt(a);
			 QTcpSocket * thesocket = theClient.theSocket;
			 SendMsgObj data = theClient.theMsgObject;
			 QString nickName = data.nickName;
			 QString wxid = data.wxId;

			 if (Hostwxid == wxid)
			 {
				 //如果是自己的号就不发
				 tb->append("自己的号不抢");
				 list->removeAt(a);
				 continue;
			 }

			 if (thesocket != NULL&&thesocket->isOpen())
			 {
				 QJsonObject jo;
				 jo.insert("from", HostnickName);
				 jo.insert("type", 1);
				// jo.insert("msg", sendD);
				 QJsonDocument jd(jo);
				 QByteArray ba = jd.toJson();
				 //ba.data();
				 int size = thesocket->write(ba);
				 //socket->flush();
				 //socket->close();

				 list->removeAt(a);
				// tb->append("向" + nickName + "发送:\n" + sendD + "抢包命令\n");
			 }
		 }

		/* if (list->size()<=0)
		 {
			 return;
		 }*/
		 //每一次休眠1000毫秒
		 Sleep(2000);
		 // clientList->clear();

	 
	 }

	 

 }
 NoTiceXiaoHaoTask::~NoTiceXiaoHaoTask()
 {
 }
