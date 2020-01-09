#ifndef TRANSLATEWORKER_H
#define TRANSLATEWORKER_H

#include <QObject>
#include "DataModel/TranslateModel.h"

class NetWorker;

class TranslateWorker : public QObject
{
    Q_OBJECT
public:

    explicit TranslateWorker(QList<TranslateModel> &list, QObject *parent = 0);
    ~TranslateWorker();

    bool YoudaoTranslate(const QString &from = QString("auto"), const QString &to = QString("en"));

signals:
    void STranslateResult(int index, const QString &str);

private slots:
    void SlotTranslateResult(int index, const QString &str);

private:
    QByteArray GetYoudaoSign(const QString &source, int salt);
    void YoudaoTranslate(int index, const QString &source);

    NetWorker*              m_pNetWorker;
    QList<TranslateModel>   &m_list;

    QString                 m_fromLang;
    QString                 m_toLang;
};

#endif // TRANSLATEWORKER_H
