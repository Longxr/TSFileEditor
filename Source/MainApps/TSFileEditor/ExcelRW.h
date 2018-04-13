#ifndef EXCELRW_H
#define EXCELRW_H

#include <QObject>
#include <LibXlsxRW/LibXlsxRW.h>
#include "DataModel/TranslateModel.h"

#define MAX_ERROR_COUNT 10
#define MAX_GROUP 16
#define MAX_GROUP_LENGTH 2

class ExcelRW : public QObject
{
    Q_OBJECT
public:
    enum ERROR_TYPE{
        EXCEL_EMPTY_ERROR = 0,
    };

    enum EXCEL_TRANSLATE_COLUMN{
        CELL_KEY = 1,
        CELL_SOURCE,
        CELL_TRANSLATE,
    };

public:
    explicit ExcelRW(QObject *parent = 0);

    bool  ImportFromXlsx(QList<TranslateModel>& list, QString strPath);
    bool  ExportToXlsx(QList<TranslateModel>& list, QString strPath);

private:


public slots:

private:
    bool checkAccountName(QString string);
    bool checkPassword(QString string);
    bool checkIsNumber(QString string);
    bool checkWebSite(QString site);

    int nTotalCount;
};

#endif // EXCELRW_H
