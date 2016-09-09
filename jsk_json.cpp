
#include "jsk_json.h"


QString getWMIHWInfo(int type) {
	/*
	//1.��ǰԭ��������ַ��
	SELECT MACAddress  FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))
	// 2.Ӳ�����к�
	SELECT PNPDeviceID FROM Win32_DiskDrive WHERE(SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')
	//3.��ȡ�������к�
	SELECT SerialNumber FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)
	// 4.������ID
	SELECT ProcessorId  FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)
	// 5.BIOS���к�
	SELECT SerialNumber  FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)
	// 6.�����ͺ�
	SELECT Product FROM Win32_BaseBoard WHERE (Product IS NOT NULL)*/

	QString hwInfo = "";
	QStringList sqlCmd;
	sqlCmd.clear();
	sqlCmd << "SELECT MACAddress  FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))";
	//ע��qt���� wmiʱ���Բ�ѯ���Ҫ����ϸ����� like֮��ľ�����ؾ�ȷ�����н������
	sqlCmd << "SELECT PNPDeviceID  FROM  Win32_DiskDrive WHERE( PNPDeviceID IS NOT NULL) AND (MediaType LIKE 'Fixed%')";
	sqlCmd << "SELECT SerialNumber FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)";
	sqlCmd << "SELECT ProcessorId  FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)";
	sqlCmd << "SELECT SerialNumber  FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)";
	sqlCmd << "SELECT Product FROM Win32_BaseBoard WHERE (Product IS NOT NULL)";

	QStringList columnName;
	columnName.clear();
	columnName << "MACAddress";
	columnName << "PNPDeviceID";
	columnName << "SerialNumber";
	columnName << "ProcessorId";
	columnName << "SerialNumber";
	columnName << "Product";


	QAxObject *objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
	// ui->textBrowser->setHtml(objIWbemLocator->generateDocumentation());
	QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)", QString("."), QString("root\\cimv2"));
	QString query = "";
	if (type<sqlCmd.size())
		query = sqlCmd.at(type);

	QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))", query);
	QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
	//��Ҫ include windows.h
	IEnumVARIANT* enumInterface = 0;
	enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);
	enumInterface->Reset();

	// qDebug()<<"number is "<<QString::number(objInterList->dynamicCall("Count").toInt());
	for (int i = 0; i < objInterList->dynamicCall("Count").toInt(); i++) {
		//qDebug()<<__FILE__<<__LINE__;
		VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
		if (enumInterface->Next(1, theItem, NULL) != S_FALSE) {
			//        qDebug()<<__FILE__<<__LINE__;
			QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
			//          qDebug()<<__FILE__<<__LINE__;
			if (item) {
				if (type<columnName.size()) {
					QByteArray datagram(columnName.at(type).toLatin1()); //Or
					const char* tempConstChar = datagram.data();
					qDebug() << "the query is " << query << " and cn is " << QString::fromLatin1(tempConstChar);
					hwInfo = item->dynamicCall(tempConstChar).toString();
					//ui->textBrowser_5->append(tr("\r\n query is %1:, result is %2").arg(query).arg(hwInfo));
				}

				qDebug() << " string is " << hwInfo;
			}
			else {
				qDebug() << " item is null";
			}
		}
		else {
			qDebug() << " item is false";
		}
	}
	//this->getCPUManID();
	return hwInfo;
}

QString getHDLogicalID() {
	/*char cVolume[256];                                         //
	char cFileSysName[256];
	DWORD dwSerialNum;                                          //Ӳ�����к�
	DWORD dwFileNameLength;
	DWORD dwFileSysFlag;

	::GetVolumeInformation(L"C://", cVolume, 256, &dwSerialNum, &dwFileNameLength, &dwFileSysFlag, cFileSysName, 256);
	*/
	DWORD VolumeSerialNumber;
	GetVolumeInformation(L"C:\\", NULL, 0, &VolumeSerialNumber, NULL, NULL, NULL, 0);
	//ui->textBrowser_5->append(tr("\r\n disknum is %1").arg(QString::number(VolumeSerialNumber, 16).toUpper()));
	return QString::number(VolumeSerialNumber, 16).toUpper();
}

QString getCPUManID() {
	//�����洢��Ϣ
	DWORD deax;
	DWORD debx;
	DWORD decx;
	DWORD dedx;

	char ID[25];//�洢��������Ϣ
	memset(ID, 0, sizeof(ID));//��������� ID
	__asm
	{
		mov eax, 0
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}

	memcpy(ID + 0, &debx, 4);//��������Ϣǰ�ĸ��ַ����Ƶ�����
	memcpy(ID + 4, &dedx, 4);//�м��ĸ�
	memcpy(ID + 8, &decx, 4);//����ĸ�
							 //������� char * ,���������;�ʷ��� string ֵ
	qDebug() << "manufacture id is " << QString::fromLocal8Bit(ID);
	return QString::fromLocal8Bit(ID);

}

QString getMac() {
	QString macAddress = "";
	QList<QNetworkAddressEntry> lclInfAE;
	QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
	foreach(QNetworkInterface iface, list)
	{
		//��֤��ȡ���Ǳ���IP��ַ���������������� ֮��������ַ
		//���������Ż�
		if (!(iface.humanReadableName().contains("VMware", Qt::CaseInsensitive)) && !(iface.humanReadableName().contains("Tunnel", Qt::CaseInsensitive)) && !(iface.humanReadableName().contains("Tunneling", Qt::CaseInsensitive)) && !(iface.humanReadableName().contains("Loopback", Qt::CaseInsensitive)) && !(iface.humanReadableName().contains("Pseudo", Qt::CaseInsensitive))) {
			qDebug() << "caught iface name is " << iface.humanReadableName();
			if (iface.hardwareAddress() != "") {
				macAddress = iface.hardwareAddress().toUpper();
				qDebug() << "hdmac is " << macAddress;
			}

		}
	}
	//ui->textBrowser_5->append(tr("\r\n MAC is %1").arg(macAddress));
	return macAddress;

}


QString getCPUID1() {
	char    OEMString[13];
	QString result = "";
	int    iEAXValue, iEBXValue, iECXValue, iEDXValue;
	_asm
	{
		mov     eax, 0
		cpuid
		mov     DWORD     PTR     OEMString, ebx
		mov     DWORD     PTR     OEMString + 4, edx
		mov     DWORD     PTR     OEMString + 8, ecx
		mov     BYTE     PTR     OEMString + 12, 0
	}

	// SetDlgItemText(IDC_STATIC1,OEMString);  //CPU ��Ӧ�� ����
	qDebug() << "manufacture id is " << QString(OEMString);
	_asm
	{
		mov     eax, 1
		cpuid
		mov     iEAXValue, eax
		mov     iEBXValue, ebx
		mov     iECXValue, ecx
		mov     iEDXValue, edx
	}

	int iCPUFamily = (0xf00 & iEAXValue) >> 8;
	char Family[10] = { 0 };
	_itoa_s(iCPUFamily, Family, 10);
	// SetDlgItemText(IDC_STATIC2,Family);    //CPUϵ��
	qDebug() << "cpu family is " << QString(Family);

	_asm
	{
		mov     eax, 2
		CPUID
	}

	char szCPUID[129] = { NULL };
	char szTmp[33] = { NULL };
	unsigned long s1 = 0, s2 = 0;

	_asm
	{
		mov     eax, 01h
		xor     edx, edx
		cpuid
		mov     s1, edx
		mov     s2, eax
	}
	//��д16 ���ƣ����ռ8 ��λ�ã��Ҷ���
	sprintf_s(szTmp, "%08X%08X", s1, s2);
	strcpy_s(szCPUID, szTmp);
	_asm
	{
		mov     eax, 03h
		xor     ecx, ecx
		xor     edx, edx
		cpuid
		mov     s1, edx
		mov     s2, ecx
	}
	sprintf_s(szTmp, "%08X%08X", s1, s2);
	strcat_s(szCPUID, szTmp);
	// SetDlgItemText(IDC_STATIC3,szCPUID);   //CPUID��
	qDebug() << "cpuid2 is " << QString(szCPUID);

	//ui->textBrowser_5->append(tr("\r\n cpuid1 is %1").arg(QString(szCPUID)));
	result = QString(szCPUID).toUpper();
	return result;


}

QString getCPUID2() {
	DWORD   dwId1, dwId2, dwId3, dwId4;
	char   szCompany[13];
	PCHAR   pCompany = szCompany;
	szCompany[12] = 0;

	_asm
	{
		pushfd
		pushad
		//ȡ��CPU��ID��
		mov   eax, 1   //���ܺ�
		_emit   0x0f
		_emit   0xa2
		mov   dwId1, eax
		mov   dwId2, ebx
		mov   dwId3, ecx
		mov   dwId4, edx

		//ȡ��CPU�����칫˾����
		mov   edi, pCompany   //���ܺ�
		mov   eax, 0
		_emit   0x0f
		_emit   0xa2
		mov   eax, ebx
		stosd
		mov   eax, edx
		stosd
		mov   eax, ecx
		stosd
		popad
		popfd
	}

	DWORD dwResult = 0;
	DWORD dwTemp1 = dwId1 << 12;
	DWORD dwTemp2 = dwId2 << 8;
	DWORD dwTemp3 = dwId3 << 4;

	QString t= "splitted string is %1_%2_%3_%4";
	

	QString res = t.arg(QString::number(dwTemp1, 16)).arg(QString::number(dwTemp2, 16)).arg(QString::number(dwTemp3, 16)).arg(QString::number(dwId4, 16));
	dwResult = dwTemp1 + dwTemp2 + dwTemp3 + dwId4;
	QString result = QString::number(dwResult, 16).toUpper();
	qDebug() << "the result is " << result;
	QString cpy = QString::fromLocal8Bit(szCompany);
	//ui->textBrowser_5->append(res + "\r\n cpuid2 is :" + result.toUpper() + "\r\n company is :" + cpy);
	return result;
}


QString getMachineCode(int type) {

	QString machineInfo = "";
	machineInfo.append(getCPUID1());
	machineInfo.append("@");
	machineInfo.append(getCPUID2());
	machineInfo.append("@");
	for (int i = 0; i<6; i++) {
		machineInfo.append(getWMIHWInfo(i));
		machineInfo.append("@");
	}

	machineInfo.append(getHDLogicalID());
	machineInfo.append("@");
	machineInfo.append(getMac());

	QCryptographicHash sha1(QCryptographicHash::Sha1);
	
	QByteArray datagram(machineInfo.toLatin1());

	const char* tempConstChar = datagram.data();
	sha1.addData(tempConstChar);
	QString  machineCode = sha1.result().toHex();

	if (type == 1) {
		QClipboard *board = QApplication::clipboard();
	
		//board->setText(machineCode);
		// QMessageBox::information (NULL,QObject::tr("������"),QObject::tr("�������Ѿ����Ƶ�ճ������,\r\n�뷢�������Ӧ���Ի�ȡ������"));
	}



	return machineCode;




}
