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
    QNetworkRequest request = QNetworkRequest(url);
//    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
//    request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.8");
//    request.setRawHeader("Cache-Control", "no-cache");
//    request.setRawHeader("Connection", "keep-alive");
//    request.setRawHeader("DNT","1");
//    request.setRawHeader("Pragma","no-cache");
//    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.76 Safari/537.36");
    return (m_manager->get(request));
}

QNetworkReply * NetWorker::get(const QString &baseUrl, const QUrlQuery &query)
{
    QUrl url(baseUrl);
    url.setQuery(query);
    qDebug() << "get argv: " << url;
    return (m_manager->get(QNetworkRequest(url)));
}

QNetworkReply * NetWorker::post(const QString &url, const QByteArray &data)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    qDebug() << "post argv: " << data;
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

