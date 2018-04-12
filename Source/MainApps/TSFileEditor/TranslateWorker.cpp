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
    QByteArray sourceUtf8 = Utf8Encode(source.toUtf8()).toLatin1();

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
            QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();
            qDebug() << "Status Code :" << nStatusCode;

            QByteArray replyData = pReply->readAll();

            qDebug() << replyData;

            QJsonParseError error;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(pReply->readAll(), &error);
            if (error.error == QJsonParseError::NoError) {
                if (!(jsonDocument.isNull() || jsonDocument.isEmpty()) && jsonDocument.isObject()) {
                    QVariantMap data = jsonDocument.toVariant().toMap();
//                    qDebug() << data;
                    int errorcode = data[QLatin1String("errorCode")].toInt();

                    if(0 != errorcode){
                        QString translation = data[QLatin1String("translation")].toString();
                        emit STranslateResult(translation);
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

QString TranslateWorker::Utf8Encode(const QByteArray &array)
{
    QString result;
    result.reserve(array.length() + array.length() / 3);
    for (int i = 0; i < array.length(); ++i) {
        char c = array.at(i);
        // if char is non-ascii, escape it
        if (c < 0x20 || uchar(c) >= 0x7f) {
//            result += "\\x" + QString::number(uchar(c), 16).toUpper();
            result += "%" + QString::number(uchar(c), 16).toUpper();
        }
        else {
            // if previous char was escaped, we need to make sure the next char is not
            // interpreted as part of the hex value, e.g. "äc.com" -> "\xabc.com"; this
            // should be "\xab""c.com"
            QRegExp hexEscape("\\\\x[a-fA-F0-9][a-fA-F0-9]$");
            bool isHexChar = ((c >= '0' && c <= '9') ||
                             (c >= 'a' && c <= 'f') ||
                             (c >= 'A' && c <= 'F'));
            if (result.contains(hexEscape) && isHexChar)
                result += "\"\"";
            result += c;
        }
    }

//    result.replace("\"", ""); //delete "
//    qDebug() << "result: " << result;

    return result;
}
