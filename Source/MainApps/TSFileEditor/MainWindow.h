#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include "DataModel/TranslateModel.h"

namespace Ui {
class MainWindow;
}

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

private:
    Ui::MainWindow *ui;

    QList<TranslateModel> m_transList;
};

#endif // MAINWINDOW_H
