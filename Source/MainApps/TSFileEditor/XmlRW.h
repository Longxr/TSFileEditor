#ifndef XMLRW_H
#define XMLRW_H

#include <QObject>
#include <QtXml>

#include "DataModel/TranslateModel.h"

class XmlRW : public QObject
{
    Q_OBJECT
public:
    explicit XmlRW(QObject *parent = 0);

    bool ImportFromTS(QList<TranslateModel>& list, QString strPath);
    bool ExportToTS(QList<TranslateModel>& list, QString strPath);
    QString ErrorString() const;

private:
    void UpdateTranslateMap(QList<TranslateModel>& list);
    void ReadXBEL();
    void ReadContext();
    void ReadMessage();

    QXmlStreamReader xml;
    QString m_tsFilePath;
    QMap<QString, QString> m_translateMap;
};

#endif // XMLRW_H
