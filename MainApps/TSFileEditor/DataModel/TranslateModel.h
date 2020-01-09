#ifndef TRANSLATEMODEL_H
#define TRANSLATEMODEL_H

#include <QString>

class TranslateModel
{
public:
    explicit TranslateModel();
    TranslateModel(const TranslateModel&);
    TranslateModel &operator= (const TranslateModel&);
    ~TranslateModel();

    QString GetKey() const;
    QString GetSource() const;
    QString GetTranslate() const;

    void SetKey(const QString &str);
    void SetSource(const QString &str);
    void SetTranslate(const QString &str);

private:
    QString m_key;
    QString m_source;
    QString m_translate;
};

#endif // TRANSLATEMODEL_H
