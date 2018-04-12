#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include "DataModel/TranslateModel.h"

namespace Ui {
class MainWindow;
}

class XmlRW;
class ExcelRW;
class TranslateWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tsLookBtn_clicked();

    void on_excelLookBtn_clicked();

    void on_generateBtn_clicked();

    void on_tsUpdateBtn_clicked();

    void on_translateBtn_clicked();

    void SlotComboBoxChanged(int);

private:
    Ui::MainWindow*         ui;

    QList<TranslateModel>   m_transList;

    QString                 m_toLanguage;

    XmlRW*                  m_pXmlWorker;
    ExcelRW*                m_pExcelWorker;
    TranslateWorker*        m_pTranslateWorker;
};

#endif // MAINWINDOW_H
