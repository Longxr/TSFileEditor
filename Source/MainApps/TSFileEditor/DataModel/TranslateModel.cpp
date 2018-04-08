#include "TranslateModel.h"

TranslateModel::TranslateModel()
{

}

TranslateModel::TranslateModel(const TranslateModel &other)
{
    this->m_key = other.GetKey();
    this->m_source = other.GetSource();
    this->m_translate = other.GetTranslate();
}

TranslateModel &TranslateModel::operator=(const TranslateModel &other)
{
    if(&other != this){
        this->m_key = other.GetKey();
        this->m_source = other.GetSource();
        this->m_translate = other.GetTranslate();
    }

    return *this;
}

TranslateModel::~TranslateModel()
{

}

//****************************Getter && Setter*************************************
QString TranslateModel::GetKey() const
{
    return m_key;
}

QString TranslateModel::GetSource() const
{
    return m_source;
}

QString TranslateModel::GetTranslate() const
{
    return m_translate;
}

void TranslateModel::SetKey(const QString &str)
{
    m_key = str;
}

void TranslateModel::SetSource(const QString &str)
{
    m_source = str;
}

void TranslateModel::SetTranslate(const QString &str)
{
    m_translate = str;
}
