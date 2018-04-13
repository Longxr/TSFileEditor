#include "ExcelRW.h"
#include <QRegExpValidator>
#include <QFileInfo>
#include <QDebug>

ExcelRW::ExcelRW(QObject *parent) : QObject(parent)
{
    nTotalCount = 0;
}

bool ExcelRW::ImportFromXlsx(QList<TranslateModel> &list, QString strPath)
{
    bool bSuccess = true;
//    int nErrLine = 1;
    list.clear();

    QString strKey, strSource, strTranslate;
    QXlsx::CellRange cellRange;

    QXlsx::Document* m_pDoc = new QXlsx::Document(strPath);
    if(m_pDoc == NULL)
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
        if(1 == cellRange.lastRow() && (m_pDoc->cellAt(cellRange.lastRow(), cellRange.lastColumn()) == 0))
        {
            bSuccess = true;
            break;
        }

        for(int i = 2; i<=cellRange.lastRow(); i++)
        {
            nTotalCount++;

            if(m_pDoc->cellAt(i,CELL_KEY) == 0)
            {
                strKey = "";
            }else
            {
                strKey = m_pDoc->cellAt(i, CELL_KEY)->value().toString().trimmed();
            }

            if(m_pDoc->cellAt(i, CELL_SOURCE) == 0)
            {
                strSource = "";
            }else
            {
                strSource = m_pDoc->cellAt(i, CELL_SOURCE)->value().toString().trimmed();
            }

            if(m_pDoc->cellAt(i, CELL_TRANSLATE) == 0)
            {
                strTranslate = "";
            }else
            {
                strTranslate = m_pDoc->cellAt(i, CELL_TRANSLATE)->value().toString().trimmed();
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
        qDebug() << tr("导出路径不能为空");
        return false;
    }

    if (list.count() <= 0)
    {
        qDebug() << tr("未读取翻译文件");
        return false;
    }

    QString strHeaderkey = tr("Key");
    QString strHeaderSource = tr("Source");
    QString strHeaderTranslate = tr("Translate");

    QXlsx::Document xlsx;
    xlsx.addSheet("Sheet1");
    xlsx.write(1, CELL_KEY, QVariant(strHeaderkey));
    xlsx.write(1, CELL_SOURCE, QVariant(strHeaderSource));
    xlsx.write(1, CELL_TRANSLATE, QVariant(strHeaderTranslate));

    for(int i=0; i < list.count(); i++)
    {
        for(int j=1; j<=3; j++){
            switch (j) {
            case CELL_KEY:
                xlsx.write(i+2, j, QVariant(list[i].GetKey()));
                break;
            case CELL_SOURCE:
                xlsx.write(i+2, j, QVariant(list[i].GetSource()));
                break;
            case CELL_TRANSLATE:
                xlsx.write(i+2, j, QVariant(list[i].GetTranslate()));
                break;
            default:
                break;
            }
        }
    }

    xlsx.saveAs(strPath);

    return true;
}

bool ExcelRW::checkAccountName(QString string)
{
    if(string.isEmpty())
    {
        return false;
    }

    QRegExp regp("^[a-zA-Z_0-9]+$");
    QRegExpValidator validator(regp,0);
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
    QRegExpValidator validator(regp,0);
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
    QRegExpValidator validator(regp,0);
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
    QRegExpValidator validator(regp,0);
    int pos=0;
    if(QValidator::Acceptable != validator.validate(site, pos))
    {
        return false;
    }

    return true;
}
