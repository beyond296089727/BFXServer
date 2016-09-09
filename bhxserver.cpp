#include "bhxserver.h"
#include"jsk_json.h"


#pragma execution_character_set("utf-8")


QList<ClientObj> *clientList;
QTcpSocket* theclient;
QHash<QString, QList<ClientObj>*>* HBMap;
QHash<QString, ClientObj>* OnLineClients;

bool isAuth = false;

BHXServer::BHXServer(QWidget *parent)
	: QMainWindow(parent)
{
	server = new QTcpServer();
	ui.setupUi(this);
	connect(ui.pb_startserver,SIGNAL(clicked()),this,SLOT(startServer()));
	//connect(ui.pb_update_mac, SIGNAL(clicked()), this, SLOT(updata_mac()));
	connect(ui.pb_clean_log, SIGNAL(clicked()), this, SLOT(cleanLog()));
	connect(ui.pb_authorize, SIGNAL(clicked()), this, SLOT(Authorize()));
	clientList = new QList<ClientObj>();
	HBMap = new QHash<QString, QList<ClientObj>*>();
	OnLineClients = new QHash<QString, ClientObj>();
	QHostAddress ip = getLocalHostIP();
	ui.lb_ip->setText(ip.toString());
	ui.tb_wx_mac->append("本机机器码:\n"+getMyMac());

	//A92CE8BA9E519647
	//564835810457BCC3

	if (isAuthrioze())
	{
		ui.lb_isAuth->setText("已授权");
		QSettings setting(".\\settings.ini", QSettings::IniFormat);
		QString auth_coade=setting.value("ac").toString();
		ui.le_my_tuth_code->setText(auth_coade);
		isAuth = true;
	}

}

void Client::checkServerOnLine(SendMsgObj data, QTcpSocket *socket)
{
	QString mac = data.macCode;
	QString nickName = data.nickName;
	
	if (mac==NULL||OnLineClients->contains(mac))
	{
		return;
	}
	ClientObj o;
	o.theSocket = socket;
	o.theMsgObject = data;
	OnLineClients->insert(mac,o);
	tb_log->append(nickName+"上线...");
}
BHXServer::~BHXServer()
{

}

void BHXServer::Authorize()
{
	QString maccode = getMyMac();
	QString auth_code_input = ui.le_my_tuth_code->text().trimmed();
	QSettings setting(".\\settings.ini", QSettings::IniFormat);

	//QString auth_code = setting.value("auth_code").toString();
	QString t1 = maccode.mid(3, 3);
	QString t2 = maccode.mid(4, 4);
	maccode.append(t1).append(t2);

	QString md5;
	QByteArray ba, bb;
	QCryptographicHash md(QCryptographicHash::Md5);
	ba.append(maccode);
	md.addData(ba);
	bb = md.result();
	QString auth_code = md5.append(bb.toHex()).mid(8,16);
	if (auth_code_input == auth_code)
	{
		//setting.setValue(auth_code, auth_code);
		ui.tb_wx_mac->append("授权本机成功");
		ui.lb_isAuth->setText("已授权");
		isAuth = true;
		QSettings setting(".\\settings.ini", QSettings::IniFormat);
		//setting.beginGroup("anth");
		setting.setValue("ac", auth_code_input);
		setting.endGroup();
	}
	else
	{
		ui.tb_wx_mac->append("授权本机失败");
		ui.tb_wx_mac->append(auth_code);
	}
	
}
QString getMyMac()
{
	QUuid id = QUuid::createUuid();
	QString strId = id.toString();
	QString md5;
	QByteArray ba, bb;
	QCryptographicHash md(QCryptographicHash::Md5);
	ba.append(strId);
	md.addData(ba);
	bb = md.result();
	md5.append(bb.toHex());
	QString mac = getMachineCode(1);
	//return md5.mid(8,16).toUpper();
	return mac.mid(8, 16);	
}
void BHXServer::acceptConnection()
{
	clientConnection = server->nextPendingConnection();
	QObject::connect(clientConnection, SIGNAL(readyRead()), new Client(clientConnection,ui.tb_log,ui.tb_wx_mac), SLOT(readClient()));
}

void BHXServer::cleanLog()
{
	ui.tb_log->setText("");
}
void BHXServer::startServer()
{
	/*if (!isAuthrioze())
	{
		ui.tb_log->append("未授权，请授权授权在使用。");
		return;
	}*/
	
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

void BHXServer::updata_mac()
{
	//ui.le_mac->setText(getMyMac());
}

bool isAuthrioze()
{
	QString maccode = getMyMac();
	QSettings setting(".\\settings.ini", QSettings::IniFormat);
	if (!setting.contains("ac"))
	{

		return false;
	}
	//setting.beginGroup("auth");
	QVariant auth_code_v = setting.value("ac");
	QString auth_code_input = auth_code_v.toString();
	//setting.endGroup();
	QString t1 = maccode.mid(3, 3);
	QString t2 = maccode.mid(4, 4);
	maccode.append(t1).append(t2);

	QString md5;
	QByteArray ba, bb;
	QCryptographicHash md(QCryptographicHash::Md5);
	ba.append(maccode);
	md.addData(ba);
	bb = md.result();
	QString auth_code=md5.append(bb.toHex()).mid(8,16);
	if (auth_code_input==auth_code)
	{
		return true;
	}
	else
	{
		return false;
	}

}

Client ::Client(QTcpSocket *theClient,QTextBrowser *tb, QTextBrowser *tb_wx_mac)
{
	this->theClient = theClient;
	this->tb_log = tb;
	this->tb_wx_mac = tb_wx_mac;
}


void Client::readClient()
{

	QString data = theClient->readAll();
	

	Document document;
	if (document.Parse(data.toStdString().c_str()).HasParseError())
	{
	
		tb_log->append("数据JSON解析错误");
		return;
	}

	QString wxId = "";
	//if (document.HasMember("wxId"))
	//{
		wxId = document["wxId"].GetString();
	//}
	QString macCode ="";
	//if (document.HasMember("macCode"))
	//{
		macCode = document["macCode"].GetString();
	//}
	
	bool isSubCount = false;
//	if (document.HasMember("isSubCount"))
	//{
		isSubCount = document["isSubCount"].GetBool();
	//}
	int msgType = -1;
	//if (document.HasMember("msgType"))
	//{
		msgType = document["msgType"].GetInt();
	//}
	QString nickName = "";
	//if (document.HasMember("nickName"))
	//{
		nickName = document["nickName"].GetString();
	//}
	QString sendId ="";
	//if (document.HasMember("sendId"))
	//{
		sendId = document["sendId"].GetString();
	//}
	/*if (msgType != 4)
	{
		tb_log->append("收到" + nickName + "的消息:\n" + data + "\n");
	}*/
	
	//if (!isAuth)
	//{
	//	tb_log->append("未授权");
	//	return;
	//}


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
	case 3:
		findMac(msgJsonObj, theClient);
		break;

	case 4:
		checkServerOnLine(msgJsonObj, theClient);
		break;
	
	default:
		break;
	}


}

void Client::noticeXiaoHaoToReceiveHB(SendMsgObj data, QTcpSocket *socket)
{

	if (!isAuth)
	{
	//ui.tb_log->append("未授权，请授权授权在使用。");
	return;
	}


	if (clientList->isEmpty())
	{
		return;
	}

	//启动新线程通知小号抢包
	/*NoTiceXiaoHaoTask *newTask=new NoTiceXiaoHaoTask(data,tb_log);
	newTask->start();*/
	QString sendid = data.sendId;
	QString wxid = data.wxId;
	QString nickname = data.nickName;
	//QString reqdata = data.sendData;


	//先随机随机发送给一个连接抢
	//QTime time;
	//time = QTime::currentTime();
	//qsrand(time.msec() + time.second() * 1000);
	//int c_size = clientList->size() - 1;
	//int xxx = 0;
	//if (c_size > 0)
	//{
	//
	//	xxx = qrand() % (c_size);
	//	//	% 100 ：在0 - 100中选出随机数
	//	ClientObj theClient1 = clientList->takeAt(xxx);
	//	QTcpSocket *socket1 = theClient1.theSocket;
	//	SendMsgObj o1 = theClient1.theMsgObject;
	//	QString nn1 = o1.nickName;
	//	QJsonObject jo1;
	//	jo1.insert("from", nickname);
	//	jo1.insert("type", 1);
	//	jo1.insert("sendid", sendid);
	//	QJsonDocument jd1(jo1);
	//	QByteArray ba1 = jd1.toJson();
	//	int size1 = socket1->write(ba1);
	//	tb_log->append(nickname + "随机向" + nn1 + "发送:\n" + sendid + "抢包命令\n");
	//}
	//


	for (int a = 0; a<clientList->size(); a++)
	{

		/*if (xxx!=0&&xxx==a)
		{
		continue;
		}*/

		ClientObj theClient = clientList->takeAt(a);
		QTcpSocket *socket = theClient.theSocket;
		SendMsgObj o = theClient.theMsgObject;
		QString nn = o.nickName;
		QString client_sendId = o.sendId;
		if (client_sendId != sendid)
		{
			continue;
		}

		QJsonObject jo;
		jo.insert("from", nickname);
		jo.insert("type", 1);
		jo.insert("sendid", sendid);
		QJsonDocument jd(jo);
		QByteArray ba = jd.toJson();

		int size = socket->write(ba);

		tb_log->append(nickname + "向" + nn + "发送:\n" + sendid + "抢包命令\n");

	}





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


	
	
	

}
void  Client::xiaoHaoFindHB(SendMsgObj data, QTcpSocket *socket)
{

	if (!isAuth)
	{
		//ui.tb_log->append("未授权，请授权授权在使用。");
		return;
	}


	ClientObj theClient(data,socket);
	QString sendId = data.sendId;
	QString nn = data.nickName;
	theClient.theMsgObject = data;
	theClient.theSocket = socket;
	tb_log->append(nn+":\n"+"发现红包"+sendId+"等待指令抢");
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
		
		tb_log->append(nickname+"向" + nn + "发送:\n" + sendid + "抢包命令\n");
		

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

void Client::findMac(SendMsgObj data, QTcpSocket *socket)
{
	QString wxId = data.wxId;
	QString nickName = data.nickName;
	QString mac = data.macCode;
	tb_wx_mac->append(nickName+"上传机器码:\n"+mac);
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



