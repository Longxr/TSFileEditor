#include "TranslateWorker.h"

#include <NetWorker.h>
#include <QCryptographicHash>

#define APP_KEY     "149a78d6b76617a0"
#define APP_SECRET  "z47ziTdEURm59sxii1L3RKhp3Myy1qfW"

TranslateWorker::TranslateWorker(QObject *parent) : QObject(parent)
{
    m_pNetWorker = NetWorker::instance();
}

TranslateWorker::~TranslateWorker()
{

}

void TranslateWorker::YoudaoTranslate(const QString &source, const QString &from, const QString &to)
{
    QString baseUrl = QString("http://openapi.youdao.com/api");

    int salt = 2;

    QByteArray dataArray;
    dataArray.append("q=");
    dataArray.append(source.toUtf8().toPercentEncoding());
    dataArray.append("&from=");
    dataArray.append(from);
    dataArray.append("&to=");
    dataArray.append(to);
    dataArray.append("&appKey=");
    dataArray.append(APP_KEY);
    dataArray.append("&salt=");
    dataArray.append(QString::number(salt));
    dataArray.append("&sign=");
    dataArray.append(GetYoudaoSign(source, salt));

    QNetworkReply *pReply = (m_pNetWorker->post(baseUrl, dataArray));

    connect(pReply, &QNetworkReply::finished, this, [=](){
        if (pReply->error() != QNetworkReply::NoError) {
            qDebug() << "Error String : " << pReply->errorString();
        }
        else
        {
//            QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//            int nStatusCode = variant.toInt();
//            qDebug() << "Status Code :" << nStatusCode;

            QByteArray replyData = pReply->readAll();

//            qDebug() << replyData;

            QJsonParseError error;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(replyData, &error);
            if (error.error == QJsonParseError::NoError) {
                if (!(jsonDocument.isNull() || jsonDocument.isEmpty()) && jsonDocument.isObject()) {
                    QVariantMap data = jsonDocument.toVariant().toMap();
//                    qDebug() << data;
                    int errorcode = data[QLatin1String("errorCode")].toInt();

                    if(0 == errorcode){
                        QVariantList detailList = data[QLatin1String("translation")].toList();
                        QString str = detailList.first().toString();
                        qDebug() << "source: " << source << "\ttranslation: " << str;

                        emit STranslateResult(str);
                    }

                }
            }
            else
            {
                qDebug() << "Error String : " << error.errorString();
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
