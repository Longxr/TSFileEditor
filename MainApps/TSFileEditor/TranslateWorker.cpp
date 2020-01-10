#include "TranslateWorker.h"
#include "AppConfig.h"

#include <NetWorker.h>
#include <QCryptographicHash>

TranslateWorker::TranslateWorker(QList<TranslateModel> &list, QObject *parent) : QObject(parent),
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
    QString baseUrl = QString("http://openapi.youdao.com/api");

    int salt = 2;

    QByteArray dataArray;
    dataArray.append("q=");
    dataArray.append(source.toUtf8().toPercentEncoding());
    dataArray.append("&from=");
    dataArray.append(m_fromLang);
    dataArray.append("&to=");
    dataArray.append(m_toLang);
    dataArray.append("&appKey=");
    dataArray.append(APP_KEY);
    dataArray.append("&salt=");
    dataArray.append(QString::number(salt));
    dataArray.append("&sign=");
    dataArray.append(GetYoudaoSign(source, salt));

    QNetworkReply *pReply = (m_pNetWorker->post(baseUrl, dataArray));

    connect(pReply, &QNetworkReply::finished, this, [=](){
        if (pReply->error() != QNetworkReply::NoError) {
            emit error("Http error: " + pReply->errorString());
        } else {
//            QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//            int nStatusCode = variant.toInt();
//            qDebug() << "Status Code :" << nStatusCode;

            QByteArray replyData = pReply->readAll();

//            qDebug() << replyData;

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

QByteArray TranslateWorker::GetYoudaoSign(const QString &source, int salt)
{
    QByteArray sign;

    QString str = APP_KEY + source + QString::number(salt) + APP_SECRET;

//    qDebug() << "str: " << str;

    sign = QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Md5).toHex().toUpper();

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
