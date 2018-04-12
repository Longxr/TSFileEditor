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
    QByteArray GetYoudaoSign(const QString &source, int salt);

signals:
    void STranslateResult(const QString &str);

public slots:

private:
//    QString GetYoudaoSign(const QString &source, int salt);
    QString Utf8Encode(const QByteArray &array);

    NetWorker*  m_pNetWorker;
};

#endif // TRANSLATEWORKER_H
