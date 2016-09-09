#pragma once
// *********************************************************************************************************************
// Copyright(c) 1997-2012 By Jonix Fu (Shanghai China), all rights reserved.
// Written by Jonix (fs.lanchat@gmail.com, Jonix@sina.com).
// ---------------------------------------------------------------------------------------------------------------------
// JSK Application frame v4.0 for Qt 4.8 / G++ 4.4
// Author : Jonix Fu
// Date of: 2012.11.05
// =====================================================================================================================
#ifndef QT_JSON_H
#define QT_JSON_H

#include <QMap>
#include <QList>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDateTime>

#include <QDateTime>
#include <QDateTime>
#include <QDateTime>

#include<QSettings>
#include<QDebug>
#include<QMessageBox>
#include<ActiveQt/QAxObject>
#include<QApplication>

#include <windows.h>
#include<stdio.h>
#include<QUuid>
#include <tchar.h>
#include <QFile>
#include<QNetworkInterface>
#include<QCryptographicHash>
#include<QClipboard>




QString getWMIHWInfo(int type);

QString getCPUID1();
QString getCPUID2();

QString getHDLogicalID();

QString getMac();

QString getCPUManID();

QString getMachineCode(int type);
#endif // QT_JSON_H
