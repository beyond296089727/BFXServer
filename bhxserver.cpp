#include "bhxserver.h"

#pragma execution_character_set("utf-8")


QList<ClientObj> *clientList;
QTcpSocket* theclient;
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
	clientList = new QList<QTcpSocket*>();
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
	tb_log->append("收到消息:"+data);
	QByteArray dataarray = data.toLatin1();
	char *char_data = dataarray.data();
	QJsonParseError error;
	QJsonDocument json = QJsonDocument::fromJson(dataarray, &error);
	if (error.error != QJsonParseError::NoError)

	{
		//json解析错误
		tb_log->append("数据JSON解析错误");
		return;
	}
	QJsonObject jo = json.object();
	QJsonValue  isSubCount = jo.take("isSubCount");
	QJsonValue  wxId = jo.take("wxId");
	QJsonValue  nickName = jo.take("nickName");
	QJsonValue  sendData = jo.take("sendData");
	QJsonValue  macCode = jo.take("macCode");
	QJsonValue  msgType = jo.take("msgType");
	SendMsgObj msgJsonObj;
	msgJsonObj.isSubCount = (bool)isSubCount.toBool();
	msgJsonObj.macCode = macCode.toString();
	msgJsonObj.wxId = wxId.toString();
	msgJsonObj.msgType = msgType.toInt();
	msgJsonObj.sendData = sendData.toString();
	switch (msgType.toInt())
	{
	case 1:
		xiaoHaoFindHB(msgJsonObj, theClient);
	case 2:
		noticeXiaoHaoToReceiveHB(msgJsonObj,theClient);
	
	default:
		break;
	}


}

void Client::noticeXiaoHaoToReceiveHB(SendMsgObj data, QTcpSocket *socket)
{
	QString nickName = data.nickName;
	QString reqData = data.sendData;
	tb_log->append(nickName+"(大号):"+reqData);

	for (int a=0;a<clientList->size();a++)
	{
		ClientObj theClient=clientList->takeAt(a);
		SendMsgObj data=theClient.theMsgObject;
		QString nickName = data.nickName;
		QTcpSocket *socket = theClient.theSocket;
		QString sendData = data.sendData;
		
		QByteArray ay=sendData.toLatin1();
		char *d = ay.data();
		if (socket!=NULL&&socket->isOpen())
		{
			tb_log->append("向小号"+nickName+"发送指令:"+sendData);
			socket->write(d);
			socket->flush();
		}
	}
	clientList->clear();

	

}
void  Client::xiaoHaoFindHB(SendMsgObj data, QTcpSocket *socket)
{
	ClientObj theClient;
	theClient.theMsgObject = data;
	theClient.theSocket = socket;
	clientList->append(theClient);

}
