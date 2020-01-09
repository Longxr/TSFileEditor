#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>

class QNetworkReply;

class NetWorker : public QObject
{
    Q_OBJECT
public:
//    explicit NetWorker(QObject *parent = 0);
    static NetWorker * instance();
    ~NetWorker();

    QNetworkAccessManager* GetAccessManager();
    void setDefaultConfig();

    QNetworkReply *get(const QString &baseUrl);
    QNetworkReply *get(const QString &baseUrl, const QUrlQuery &query);

    QNetworkReply *post(const QString &url, const QByteArray &data);
    QNetworkReply *postJson(const QString &url, const QByteArray &data);

signals:
    void finished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;

    explicit NetWorker(QObject *parent = 0);
    NetWorker(const NetWorker &) Q_DECL_EQ_DELETE;
    NetWorker& operator=(NetWorker rhs) Q_DECL_EQ_DELETE;
};

#endif // NETWORKER_H




