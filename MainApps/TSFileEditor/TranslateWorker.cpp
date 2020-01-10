#include "TranslateWorker.h"
#include <NetWorker.h>
#include <QCryptographicHash>
#include <QUuid>

TranslateWorker::TranslateWorker(const QString& id, const QString& key, QList<TranslateModel> &list, QObject *parent) : QObject(parent),
    m_appId(id),
    m_appKey(key),
    m_list(list)
{
    m_pNetWorker = NetWorker::instance();

    connect(this, &TranslateWorker::translateResult, this, &TranslateWorker::onTranslateResult);
}

TranslateWorker::~TranslateWorker()
{

}

bool TranslateWorker::YoudaoTranslate(const QString &from, const QString &to)
{
    if(m_list.count() <=0) {
        emit error("translate list is empty");
        return false;
    }

    m_fromLang = from;
    m_toLang = to;

    for(int i=0; i<m_list.count(); i++) {
        YoudaoTranslate(i, m_list.at(i).GetSource());
    }

    return true;
}

void TranslateWorker::YoudaoTranslate(int index, const QString &source)
{
    QString baseUrl = QString("https://openapi.youdao.com/api");

    QString uuid = QUuid::createUuid().toString();
    uuid.remove("{").remove("}").remove("-");
    QString timestamp = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());

    QUrlQuery query;
    query.addQueryItem("q", source.toUtf8().toPercentEncoding());
    query.addQueryItem("from", m_fromLang);
    query.addQueryItem("to", m_toLang);
    query.addQueryItem("appKey", m_appId);
    query.addQueryItem("salt", uuid);
    query.addQueryItem("sign", GetYoudaoSign(source, uuid, timestamp));
    query.addQueryItem("signType", "v3");
    query.addQueryItem("curtime", timestamp);

    qDebug() << source.toUtf8().toPercentEncoding();

    QNetworkReply *pReply = (m_pNetWorker->get(baseUrl, query));

    connect(pReply, &QNetworkReply::finished, this, [=](){
        if (pReply->error() != QNetworkReply::NoError) {
            emit error("Http error: " + pReply->errorString());
        } else {
//            QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//            int nStatusCode = variant.toInt();
//            qDebug() << "Status Code :" << nStatusCode;

            QByteArray replyData = pReply->readAll();

            qDebug() << replyData;

            QJsonParseError jsonError;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(replyData, &jsonError);
            if (jsonError.error == QJsonParseError::NoError) {
                if (!(jsonDocument.isNull() || jsonDocument.isEmpty()) && jsonDocument.isObject()) {
                    QVariantMap data = jsonDocument.toVariant().toMap();
//                    qDebug() << data;
                    int errorcode = data[QLatin1String("errorCode")].toInt();

                    if(0 == errorcode){
                        QVariantList detailList = data[QLatin1String("translation")].toList();
                        QString str = detailList.first().toString();

                        emit translateResult(index, str);
                    }

                }
            } else {
                emit error(jsonError.errorString());
            }
        }

        pReply->deleteLater();
    });
}

QByteArray TranslateWorker::GetYoudaoSign(const QString &source, const QString &uuid, const QString &timestamp)
{
    QByteArray sign;

    QString str = m_appId + source + uuid + timestamp + m_appKey;

//    qDebug() << "str: " << str;

    sign = QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha256).toHex().toUpper();

//    qDebug() << sign;

    return sign;
}

void TranslateWorker::onTranslateResult(int index, const QString &str)
{
    TranslateModel model = m_list.at(index);
    model.SetTranslate(str);
    m_list.replace(index, model);

    qDebug() << "key: " << m_list.at(index).GetKey() << "\tsource: " << m_list.at(index).GetSource() << "\ttranslation: " << m_list.at(index).GetTranslate();
}
