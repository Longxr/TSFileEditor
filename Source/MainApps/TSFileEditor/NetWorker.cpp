#include "networker.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDebug>
#include <QNetworkConfigurationManager>

NetWorker *NetWorker::instance()
{
    static NetWorker netWorker;
    return &netWorker;
}

NetWorker::NetWorker(QObject *parent) :
    QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);

    connect(m_manager, &QNetworkAccessManager::finished, this, &NetWorker::finished);
}

NetWorker::~NetWorker()
{

}

QNetworkAccessManager *NetWorker::GetAccessManager()
{
    return m_manager;
}

void NetWorker::setDefaultConfig()
{   //WLAN changed
    qDebug() << "setDefaultConfig";
    QNetworkConfigurationManager manager;
    m_manager->setConfiguration(manager.defaultConfiguration());
}

QNetworkReply * NetWorker::get(const QString &baseUrl)
{
    QUrl url(baseUrl);
    return (m_manager->get(QNetworkRequest(url)));
}

QNetworkReply * NetWorker::get(const QString &baseUrl, const QUrlQuery &query)
{
    QUrl url(baseUrl);
    url.setQuery(query);
//    qDebug() << "get argv: " << url;
    return (m_manager->get(QNetworkRequest(url)));
}

QNetworkReply * NetWorker::post(const QString &url, const QByteArray &data)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

//    qDebug() << "post argv: " << data;
    return (m_manager->post(request, data));
}

QNetworkReply * NetWorker::postJson(const QString &url, const QByteArray &data)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//    qDebug() << "post argv: " << data;

    return (m_manager->post(request, data));
}

