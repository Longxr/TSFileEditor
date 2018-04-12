#ifndef TRANSLATEWORKER_H
#define TRANSLATEWORKER_H

#include <QObject>

class NetWorker;

class TranslateWorker : public QObject
{
    Q_OBJECT
public:
    explicit TranslateWorker(QObject *parent = 0);
    ~TranslateWorker();

    void YoudaoTranslate(const QString &source, const QString &from = QString("zh-CHS"), const QString &to = QString("en"));

signals:
    void STranslateResult(const QString &str);

public slots:

private:
    QByteArray GetYoudaoSign(const QString &source, int salt);

    NetWorker*  m_pNetWorker;
};

#endif // TRANSLATEWORKER_H
