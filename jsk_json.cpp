
#include "jsk_json.h"

namespace JSK
{

	JSONObject::JSONObject(const QString& key)
	{
		parent = NULL;
		m_index = 1;

		init();

		setKey(key);
	}

	void JSONObject::init()
	{
		m_key = "";
		m_type = vtfObject;
	}

	JSONObject::~JSONObject()
	{
		clear();
	}

	QString JSONObject::quote(const QString& Text)
	{
		QString ret;
		std::wstring ws = Text.toStdWString();
		
		const wchar_t* ptr = ws.c_str();
		while (*ptr)
		{
			switch (*ptr)
			{
			case L'\\': ret += "\\\\"; break;
			case L'\"': ret += "\\\""; break;
			case L'\b': ret += "\\b";  break;
			case L'\t': ret += "\\t";  break;
			case L'\n': ret += "\\n";  break;
			case L'\f': ret += "\\f";  break;
			case L'\r': ret += "\\r";  break;
			default:
				if (*ptr > 255)
				{
					uint16_t v = *ptr;
					QString S = QString::number(v, 16);
					ret += "\\u" + S;
				}
				else
				{
					ret += *ptr;
				}
				break;
			}
			ptr++;
		}
		return ret;
	}

	QString JSONObject::dequote(const QString& text)
	{
		QString s = text;
		int idx;
		while ((idx = s.indexOf("\\u")) >= 0)
		{
			int nHex = s.mid(idx + 2, 4).toInt(0, 16);
			s.replace(idx, 6, QChar(nHex));
		}
		return s;
	}

	QString JSONObject::key() const
	{
		return m_key;
	}

	void JSONObject::setKey(const QString& value)
	{
		m_key = value.trimmed();
	}

	QVariant JSONObject::value() const
	{
		if (m_list.count() >0)
		{
			return toString();
		}
		else
		{
			return m_value;
		}
	}

	bool JSONObject::value(const QString& key, QVariant& v) const
	{
		bool ret = false;
		QMap<QString, JSONObject*>::const_iterator child = m_list.find(key);

		if (child != m_list.constEnd() && child.key() == key)
		{
			v = child.value()->value();
			ret = true;
		}
		return ret;
	}

	QList<JSONObject*> JSONObject::children() const
	{
		QList<JSONObject*> ret;
		for (QMap<QString, JSONObject*>::const_iterator it = m_list.begin();
			it != m_list.end();
			it++)
		{
			ret.append(it.value());
		}
		return ret;
	}

	QString JSONObject::jsonKey() const
	{
		return "\"" + quote(m_key) + "\"";
	}

	QString JSONObject::jsonValue() const
	{
		QString ret;
		switch (m_type)
		{
		default:
		case vtfString:
			ret = "\"" + quote(m_value.toString()) + "\"";
			break;
		case vtfInt:
			ret = QString::number(m_value.toInt());
			break;
		case vtfDouble:
			ret = QString::number(m_value.toDouble());
			break;
		case vtfBool:
		{
			QString v = m_value.toString().trimmed().toLower();
			ret = v == "ok" || v == "yes" || v == "true" || v.toInt() >0 ? "true" : "false";
		}
		break;
		case vtfDateTime:
			ret = m_value.toDateTime().toString("yyyy/MM/dd HH:mm:ss.zzz");
			break;
		}
		return ret;
	}

	JSONObject* JSONObject::addObject(const QString& key)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfObject);
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	JSONObject* JSONObject::addArray(const QString& key)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfArray);
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	JSONObject* JSONObject::addNull(const QString& key)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfNull);
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	JSONObject* JSONObject::add(const QString& key, const QString& value)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfString);
		ret->m_value = value;
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	JSONObject* JSONObject::add(const QString& key, int value)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfInt);
		ret->m_value = value;
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	JSONObject* JSONObject::add(const QString& key, double value)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfDouble);
		ret->m_value = value;
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	JSONObject* JSONObject::add(const QString& key, bool value)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfBool);
		ret->m_value = value;
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	JSONObject* JSONObject::add(const QString& key, QDateTime value)
	{
		JSONObject* ret = new JSONObject(key);
		ret->parent = this;
		ret->setType(vtfDateTime);
		ret->m_value = value;
		m_list[ret->key().isEmpty() ? QString::number(m_index++) : ret->key()] = ret;
		return ret;
	}

	void JSONObject::deleteChildren(const QString& key)
	{
		JSONObject* ret = m_list.value(key);
		m_list.remove(key);
		if (ret != NULL)
		{
			delete ret;
		}
	}

	void JSONObject::clearChildren()
	{
		QMap<QString, JSONObject*>::iterator it;
		while (m_list.count()>0)
		{
			it = m_list.begin();
			delete it.value();
			m_list.remove(it.key());
		}
	}

	void JSONObject::clear()
	{
		clearChildren();
		init();
	}

	QString JSONObject::toString() const
	{
		QString values;
		if (m_list.count() >0)
		{
			for (QMap<QString, JSONObject*>::const_iterator it = m_list.begin();
				it != m_list.end();
				it++)
			{
				JSONObject* obj = it.value();

				values += "," + obj->toString();
			}
			if (!values.isEmpty())
			{
				values.remove(0, 1);
			}
			if (m_type == vtfArray)
			{
				values = "[" + values + "]";
			}
			else
			{
				values = "{" + values + "}";
			}
		}
		else
		{
			if (m_type == vtfArray)
			{
				values = "[]";
			}
			else
			{
				values = jsonValue();
			}
		}
		return parent != NULL && parent->type() != vtfArray && !key().isEmpty() ? jsonKey() + ":" + values : values;
	}

	bool JSONObject::fromString(QString jsonText)
	{
		clear();

		std::wstring ws = jsonText.toStdWString();
		const wchar_t* ptr = ws.c_str();
		ptr = skip(ptr);
		switch (*ptr)
		{
		case L'{':
			ptr = fromObject("", ptr);
			break;

		case L'[':
			ptr = fromArray(ptr);
			break;
		}
		ptr = skip(ptr);
		if (!ptr)
		{
			clear();
		}
		return ptr != NULL;
	}

	const wchar_t* JSONObject::fromObject(const QString& Key, const wchar_t* ptr)
	{
		ptr = skip(ptr);
		if (*ptr == L'{')
		{
			setType(vtfObject);
			do {
				ptr = skip(++ptr);
				QString Key;
				ptr = skip(parseKey(Key, ptr));
				if (*ptr == L':')
				{
					ptr = skip(++ptr);

					ptr = parseValue(Key, ptr);
				}
				ptr = skip(ptr);
			} while (ptr && *ptr && *ptr == ',');
			if (ptr && *ptr == L'}')
			{
				ptr = skip(++ptr);
			}
			else
			{
				ptr = 0;
			}
		}
		return ptr;
	}

	const wchar_t* JSONObject::fromArray(const wchar_t* ptr)
	{

		ptr = skip(ptr);
		if (*ptr == L'[')
		{
			setType(vtfArray);
			do {
				ptr = skip(++ptr);
				ptr = parseValue("", ptr);
				ptr = skip(ptr);
			} while (ptr && *ptr && *ptr == ',');
			if (ptr && *ptr == L']')
			{
				ptr = skip(++ptr);
			}
			else
			{
				ptr = 0;
			}
		}
		return ptr;
	}

	const wchar_t* JSONObject::parseKey(QString& Value, const wchar_t* ptr)
	{
		Value = "";
		ptr = skip(ptr);
		if (*ptr == L'\"')
		{
			ptr++;
			while (*ptr != L'\"' && *ptr)
			{
				if (*ptr == L'\\')
				{
					ptr++;
					switch (*ptr)
					{
					case L'\"': Value += L'\"'; ptr++; break;
					case L'\\': Value += L'\\'; ptr++; break;
					case L'b': Value += L'\b'; ptr++; break;
					case L'f': Value += L'\f'; ptr++; break;
					case L'n': Value += L'\n'; ptr++; break;
					case L'r': Value += L'\r'; ptr++; break;
					case L't': Value += L'\t'; ptr++; break;
					case L'u':
					{
						ptr++;
						QString s = QString::fromStdWString(std::wstring(ptr, 4));
						Value += QChar(s.toUInt(NULL, 16));
						ptr += 4;
					}
					break;
					}
				}
				else
				{
					Value += *ptr;
					ptr++;
				}
			}
			++ptr;
		}
		return ptr;
	}

	const wchar_t* JSONObject::parseValue(QString Key, const wchar_t* ptr)
	{
		ptr = skip(ptr);
		if (*ptr)
		{
			if (*ptr == L'\"')
			{
				QString Value;
				ptr = skip(parseKey(Value, ptr));
				add(newKey(Key), Value);
			}
			else
				if (*ptr == L'{')
				{
					JSONObject* child = addObject(Key);
					ptr = skip(child->fromObject(Key, ptr));
				}
				else
					if (*ptr == L'[')
					{
						JSONObject* child = addArray(Key);
						ptr = skip(child->fromArray(ptr));
					}
					else
						if (ptr && *ptr
							&& (*ptr == L'+' || *ptr == L'-' || *ptr == L'.' || (*ptr >= L'0'&&*ptr <= L'9') || *ptr == L'E' || *ptr == L'e'))
						{
							QString Value;
							ptr = skip(parseNumber(Value, ptr));
							if (Value.indexOf(".") >-1)
							{
								add(newKey(Key), Value.toDouble());
							}
							else
							{
								add(newKey(Key), Value.toInt());
							}
						}
						else
							if (std::wstring(ptr, 4).compare(L"true") == 0)
							{
								add(newKey(Key), true);
								ptr += 4;
							}
							else
								if (std::wstring(ptr, 5).compare(L"false") == 0)
								{
									add(newKey(Key), false);
									ptr += 5;
								}
								else
									if (std::wstring(ptr, 4).compare(L"null") == 0)
									{
										add(newKey(Key));
										ptr += 4;
									}
		}
		return ptr;
	}

	const wchar_t* JSONObject::parseNumber(QString& Value, const wchar_t* ptr)
	{
		Value = "";
		ptr = skip(ptr);
		while (ptr && *ptr
			&& (*ptr == L'+' || *ptr == L'-' || *ptr == L'.' || (*ptr >= L'0'&&*ptr <= L'9') || *ptr == L'E' || *ptr == L'e'))
		{
			Value += *ptr;
			ptr = skip(++ptr);
		}
		return ptr;
	}
	// =====================================================================================================================
}; // End of namespace JSK;


QString getWMIHWInfo(int type) {
	/*
	//1.当前原生网卡地址：
	SELECT MACAddress  FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))
	// 2.硬盘序列号
	SELECT PNPDeviceID FROM Win32_DiskDrive WHERE(SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')
	//3.获取主板序列号
	SELECT SerialNumber FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)
	// 4.处理器ID
	SELECT ProcessorId  FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)
	// 5.BIOS序列号
	SELECT SerialNumber  FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)
	// 6.主板型号
	SELECT Product FROM Win32_BaseBoard WHERE (Product IS NOT NULL)*/

	QString hwInfo = "";
	QStringList sqlCmd;
	sqlCmd.clear();
	sqlCmd << "SELECT MACAddress  FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))";
	//注意qt调用 wmi时，对查询语句要求很严格，所以 like之类的句子务必精确才能有结果出来
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
	//需要 include windows.h
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
	DWORD dwSerialNum;                                          //硬盘序列号
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
	//用来存储信息
	DWORD deax;
	DWORD debx;
	DWORD decx;
	DWORD dedx;

	char ID[25];//存储制造商信息
	memset(ID, 0, sizeof(ID));//先清空数组 ID
	__asm
	{
		mov eax, 0
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}

	memcpy(ID + 0, &debx, 4);//制造商信息前四个字符复制到数组
	memcpy(ID + 4, &dedx, 4);//中间四个
	memcpy(ID + 8, &decx, 4);//最后四个
							 //如果返回 char * ,会出现乱码;故返回 string 值
	qDebug() << "manufacture id is " << QString::fromLocal8Bit(ID);
	return QString::fromLocal8Bit(ID);

}

QString getMac() {
	QString macAddress = "";
	QList<QNetworkAddressEntry> lclInfAE;
	QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
	foreach(QNetworkInterface iface, list)
	{
		//保证获取的是本地IP地址，不是虚拟机，隧道 之类的网络地址
		//以下这句可优化
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

	// SetDlgItemText(IDC_STATIC1,OEMString);  //CPU 供应商 名称
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
	// SetDlgItemText(IDC_STATIC2,Family);    //CPU系列
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
	//大写16 进制，宽度占8 个位置，右对齐
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
	// SetDlgItemText(IDC_STATIC3,szCPUID);   //CPUID号
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
		//取得CPU的ID号
		mov   eax, 1   //功能号
		_emit   0x0f
		_emit   0xa2
		mov   dwId1, eax
		mov   dwId2, ebx
		mov   dwId3, ecx
		mov   dwId4, edx

		//取得CPU的制造公司名称
		mov   edi, pCompany   //功能号
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
		// QMessageBox::information (NULL,QObject::tr("机器码"),QObject::tr("机器码已经复制到粘贴板上,\r\n请发给软件供应商以获取激活码"));
	}



	return machineCode;




}
