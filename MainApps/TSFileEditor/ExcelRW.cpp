#include "ExcelRW.h"
#include <QRegExpValidator>
#include <QFileInfo>

ExcelRW::ExcelRW(int keyColumn, int sourceColumn, int transColumn, QObject *parent) : QObject(parent)
{
    m_TotalCount = 0;
    m_KeyColumn = keyColumn;
    m_SourceColumn = sourceColumn;
    m_TransColumn = transColumn;
}

bool ExcelRW::ImportFromXlsx(QList<TranslateModel> &list, QString strPath)
{
    bool bSuccess = true;
//    int nErrLine = 1;
    list.clear();

    QString strKey, strSource, strTranslate;
    QXlsx::CellRange cellRange;

    QXlsx::Document* m_pDoc = new QXlsx::Document(strPath);
    if(nullptr == m_pDoc)
    {
        return bSuccess;
    }
    if(m_pDoc->sheetNames().isEmpty())
    {
        return bSuccess;
    }
    m_pDoc->selectSheet(m_pDoc->sheetNames().first());
    cellRange = m_pDoc->currentWorksheet()->dimension();

    do
    {
        if(1 == cellRange.lastRow() && (nullptr == m_pDoc->cellAt(cellRange.lastRow(), cellRange.lastColumn())))
        {
            bSuccess = true;
            break;
        }

        for(int i = 2; i<=cellRange.lastRow(); i++)
        {
            m_TotalCount++;

            if(m_pDoc->cellAt(i, m_KeyColumn) == nullptr)
            {
                strKey = "";
            }else
            {
                strKey = m_pDoc->cellAt(i, m_KeyColumn)->value().toString();
            }

            if(m_pDoc->cellAt(i, m_SourceColumn) == nullptr)
            {
                strSource = "";
            }else
            {
                strSource = m_pDoc->cellAt(i, m_SourceColumn)->value().toString();
            }

            if(m_pDoc->cellAt(i, m_TransColumn) == nullptr)
            {
                strTranslate = "";
            }else
            {
                strTranslate = m_pDoc->cellAt(i, m_TransColumn)->value().toString();
            }

//            nErrLine = i;

//            if(strSite.isEmpty())
//            {
//                addError(EXCEL_EMPTY_ERROR, nErrLine);
//                bSuccess = false;
//                continue;
//            }


            TranslateModel model;
            model.SetKey(strKey);
            model.SetSource(strSource);
            model.SetTranslate(strTranslate);
            list.append(model);
        }
    }while(0);

    delete m_pDoc;
    return bSuccess;
}

bool ExcelRW::ExportToXlsx(QList<TranslateModel>& list, QString strPath)
{
    if(strPath.isEmpty()) {
        emit error("Export path cannot be empty");
        return false;
    }

    if (list.count() <= 0)
    {
        emit error("*.ts file is empty");
        return false;
    }

    QString strHeaderkey = tr("Key");
    QString strHeaderSource = tr("Source");
    QString strHeaderTranslate = tr("Translate");

    QXlsx::Document xlsx;
    xlsx.addSheet("Sheet1");
    xlsx.write(1, m_KeyColumn, QVariant(strHeaderkey));
    xlsx.write(1, m_SourceColumn, QVariant(strHeaderSource));
    xlsx.write(1, m_TransColumn, QVariant(strHeaderTranslate));

    for(int i=0; i < list.count(); i++)
    {
        for(int j=1; j<=3; j++){
            xlsx.write(i+2, m_KeyColumn, QVariant(list[i].GetKey()));
            xlsx.write(i+2, m_SourceColumn, QVariant(list[i].GetSource()));
            xlsx.write(i+2, m_TransColumn, QVariant(list[i].GetTranslate()));
        }
    }

    xlsx.saveAs(strPath);

    return true;
}

void ExcelRW::SetTransColumn(int column)
{
    m_TransColumn = column;
}

bool ExcelRW::checkAccountName(QString string)
{
    if(string.isEmpty())
    {
        return false;
    }

    QRegExp regp("^[a-zA-Z_0-9]+$");
    QRegExpValidator validator(regp,nullptr);
    int pos = 0;
    if(QValidator::Acceptable != validator.validate(string, pos)){
        return false;
    }

    return true;
}

bool ExcelRW::checkPassword(QString string)
{
    if(string.isEmpty())
    {
        return false;
    }

    QRegExp regp("^[\\x21-\\x7E]+$");
    QRegExpValidator validator(regp, this);
    int pos = 0;
    if(QValidator::Acceptable != validator.validate(string, pos)){
        return false;
    }
    return true;
}

bool ExcelRW::checkIsNumber(QString string)
{
    if(string.isEmpty())
    {
        return false;
    }
    QRegExp regp("^[0-9]+$");
    QRegExpValidator validator(regp,nullptr);
    int pos=0;
    if(QValidator::Acceptable != validator.validate(string,pos))
    {
        return false;
    }
    return true;
}

bool ExcelRW::checkWebSite(QString site)
{
    if(site.isEmpty())
    {
        return false;
    }
    QRegExp regp("^([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+[a-zA-Z]{2,6}$");
    QRegExpValidator validator(regp,nullptr);
    int pos=0;
    if(QValidator::Acceptable != validator.validate(site, pos))
    {
        return false;
    }

    return true;
}
