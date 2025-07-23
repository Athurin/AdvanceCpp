#include <iostream>
#include <QtSql>
#include <QtCore/QString>
#include <QtCore/QDebug>

bool
connectCreate
(const QString SQLDiver, const QString conName, const QString hostName, const QString DBName, const QString User, const QString Password);