#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "XmlRW.h"
#include "ExcelRW.h"
#include "NetWorker.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tsLookBtn_clicked()
{
    const QString documentLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择ts文件"), documentLocation, "Files (*.ts)");

    if(fileName.isEmpty()){
        return;
    }
    else{
        QFileInfo info(fileName);
        if("ts" != info.suffix()){
            qDebug() << "File type is not supported";
            return;
        }
    }

    ui->tsPathEdit->setText(fileName);
}

void MainWindow::on_excelLookBtn_clicked()
{
    const QString documentLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择excel文件"), documentLocation, "Files (*.xlsx)");

    if(fileName.isEmpty()){
        return;
    }
    else{
        QFileInfo info(fileName);
        if("xlsx" != info.suffix()){
            qDebug() << "File type is not supported";
            return;
        }
    }

    ui->excelPathEdit->setText(fileName);
}

void MainWindow::on_generateBtn_clicked()
{
    bool re;

    //import .ts file
    if(ui->tsPathEdit->text().isEmpty()) {
        on_tsLookBtn_clicked();
    }

    XmlRW *newXmlRw = new XmlRW(this);
    re = newXmlRw->ImportFromTS(m_transList, ui->tsPathEdit->text());

    if(re) {
        qDebug() << tr("import .ts file success");
    }
    else {
        qDebug() << tr("import .ts file failed");
    }

    //generate excel file
    if(ui->excelPathEdit->text().isEmpty()) {
        const QString documentLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString saveName = documentLocation + "/untitled.xlsx";
        QString fileName = QFileDialog::getSaveFileName(this, "excel file path", saveName, "Files (*.xlsx)");

        if (fileName.isEmpty())
        {
            return;
        }
        else{
            ui->excelPathEdit->setText(fileName);
        }
    }

    ExcelRW *newExcel = new ExcelRW(this);
    re = newExcel->ExportToXlsx(m_transList, ui->excelPathEdit->text());
    if(re) {
        qDebug() << tr("export excel file success");
    }
    else {
        qDebug() << tr("export excel file failed");
    }
}

void MainWindow::on_tsUpdateBtn_clicked()
{
    bool re;

    //import excel file
    if(ui->excelPathEdit->text().isEmpty()) {
        on_excelLookBtn_clicked();
    }

    ExcelRW *newExcel = new ExcelRW(this);
    re = newExcel->ImportFromXlsx(m_transList, ui->excelPathEdit->text());
    if(re) {
        qDebug() << tr("import excel file success");
    }
    else {
        qDebug() << tr("import excel file failed");
    }

    //update ts file
    if(ui->tsPathEdit->text().isEmpty()) {
        on_tsLookBtn_clicked();
    }

    XmlRW *newXmlRw = new XmlRW(this);
    re = newXmlRw->ExportToTS(m_transList, ui->tsPathEdit->text());

    if(re) {
        qDebug() << tr("update .ts file success");
    }
    else {
        qDebug() << tr("update .ts file failed");
    }
}

void MainWindow::on_translateBtn_clicked()
{
    qDebug() << "translate";

    NetWorker *InfoWorker = NetWorker::instance();
    QNetworkReply *pReply = (InfoWorker->get(QString("http://www.baidu.com")));

    connect(pReply, &QNetworkReply::finished, this, [=](){
        if (pReply->error() != QNetworkReply::NoError) {
            qDebug() << "Error String : " << pReply->errorString();

        }
        else
        {
            QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();
            qDebug() << "Status Code :" << nStatusCode;

            QByteArray datagram = pReply->readAll();
            qDebug() << datagram;
        }

            pReply->deleteLater();
        });
}
