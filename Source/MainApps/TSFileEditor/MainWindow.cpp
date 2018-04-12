#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "XmlRW.h"
#include "ExcelRW.h"
#include "TranslateWorker.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QDebug>
#include <QListView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_toLanguage = "en";
    m_pXmlWorker = new XmlRW(this);
    m_pExcelWorker = new ExcelRW(this);
    m_pTranslateWorker = new TranslateWorker(this);

    ui->comboBox->setView(new QListView());
    ui->comboBox->addItem("英文", "en");
    ui->comboBox->addItem("中文", "zh-CHS");
    ui->comboBox->addItem("日文", "ja");
    ui->comboBox->addItem("韩文", "ko");
    ui->comboBox->addItem("法文", "fr");
    ui->comboBox->addItem("俄文", "ru");
    ui->comboBox->addItem("葡萄牙文", "pt");
    ui->comboBox->addItem("西班牙文", "es");

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotComboBoxChanged(int)));
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


    re = m_pXmlWorker->ImportFromTS(m_transList, ui->tsPathEdit->text());

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

    re = m_pExcelWorker->ExportToXlsx(m_transList, ui->excelPathEdit->text());
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

    re = m_pExcelWorker->ImportFromXlsx(m_transList, ui->excelPathEdit->text());
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

    re = m_pXmlWorker->ExportToTS(m_transList, ui->tsPathEdit->text());

    if(re) {
        qDebug() << tr("update .ts file success");
    }
    else {
        qDebug() << tr("update .ts file failed");
    }
}

void MainWindow::on_translateBtn_clicked()
{
    m_pTranslateWorker->YoudaoTranslate("你好", "auto", m_toLanguage);
}

void MainWindow::SlotComboBoxChanged(int)
{
    m_toLanguage = ui->comboBox->currentData().toString();
}
