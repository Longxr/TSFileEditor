#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "XmlRW.h"
#include "ExcelRW.h"
#include "TranslateWorker.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QListView>
#include <QSslSocket>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_toLanguage = "en";
    m_pXmlWorker = new XmlRW(this);
    m_pExcelWorker = new ExcelRW(1, 2, 3, this);
    m_pTranslateWorker = new TranslateWorker(ui->youdaoAppIdlineEdit->text(), ui->youdaoKeylineEdit->text(), m_transList, this);

    ui->youdaoTipLabel->setVisible(false);
    ui->comboBox->setView(new QListView());
    ui->comboBox->addItem("英文", "en");
    ui->comboBox->addItem("中文", "zh-CHS");
    ui->comboBox->addItem("日文", "ja");
    ui->comboBox->addItem("韩文", "ko");
    ui->comboBox->addItem("法文", "fr");
    ui->comboBox->addItem("俄文", "ru");
    ui->comboBox->addItem("葡萄牙文", "pt");
    ui->comboBox->addItem("西班牙文", "es");
    ui->comboBox->addItem("其他", "other");

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxChanged);
    connect(m_pExcelWorker, &ExcelRW::error, this, &MainWindow::onReceiveMsg);
    connect(m_pTranslateWorker, &TranslateWorker::error, this, &MainWindow::onReceiveMsg);

    qDebug() <<  "qt openssl support: " << QSslSocket::supportsSsl() <<  QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tsLookBtn_clicked()
{
    const QString documentLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("select .ts file"), documentLocation, "Files (*.ts)");

    if(fileName.isEmpty()){
        return;
    }

    ui->tsPathEdit->setText(fileName);
    on_tsImportBtn_clicked();
}

void MainWindow::on_excelLookBtn_clicked()
{
    const QString documentLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("select excel file"), documentLocation, "Files (*.xlsx)");

    if(fileName.isEmpty()){
        return;
    }
    else{
        QFileInfo info(fileName);
        if ("xlsx" != info.suffix()){
            onReceiveMsg("File type is not supported");
            return;
        }
    }

    ui->excelPathEdit->setText(fileName);
}

void MainWindow::on_generateBtn_clicked()
{
    bool re;

    m_pExcelWorker->SetTransColumn(ui->transSpinBox->value());

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
        onReceiveMsg("export excel file success");
        ui->youdaoTipLabel->setVisible(false);
    } else {
        onReceiveMsg("export excel file failed");
    }
}

void MainWindow::on_tsUpdateBtn_clicked()
{
    bool re;

    m_pExcelWorker->SetTransColumn(ui->transSpinBox->value());

    //import excel file
    if(ui->excelPathEdit->text().isEmpty()) {
        on_excelLookBtn_clicked();
    }

    re = m_pExcelWorker->ImportFromXlsx(m_transList, ui->excelPathEdit->text());
    if(re) {
        onReceiveMsg("import excel file success");
        ui->youdaoTipLabel->setVisible(false);
    } else {
        onReceiveMsg("import excel file failed");
    }

    //update ts file
    if(ui->tsPathEdit->text().isEmpty()) {
        on_tsLookBtn_clicked();
    }

    re = m_pXmlWorker->ExportToTS(m_transList, ui->tsPathEdit->text());

    if(re) {
        onReceiveMsg("update .ts file success");
    } else {
        onReceiveMsg("update .ts file failed");
    }

}

void MainWindow::on_translateBtn_clicked()
{
    bool re;

    m_pExcelWorker->SetTransColumn(ui->transSpinBox->value());

    //import excel file
    if(ui->excelPathEdit->text().isEmpty()) {
        on_excelLookBtn_clicked();
    }

    re = m_pExcelWorker->ImportFromXlsx(m_transList, ui->excelPathEdit->text());
    if(re) {
        onReceiveMsg("import excel file success");
    }
    else {
        onReceiveMsg("import excel file failed");
    }

//    m_pTranslateWorker->YoudaoTranslate("你好", "auto", m_toLanguage);

    //translate excel file
    re = m_pTranslateWorker->YoudaoTranslate("auto", m_toLanguage);
    if(re) {
        onReceiveMsg("translate excel file success");
        ui->youdaoTipLabel->setVisible(true);
    }
    else {
        onReceiveMsg("translate excel file failed");
    }
}

void MainWindow::onComboBoxChanged(int)
{
    QString langCode = ui->comboBox->currentData().toString();

    if ("other" == langCode) {
        m_toLanguage = ui->otherLineEdit->text();
    } else {
        m_toLanguage = langCode;
    }

}



void MainWindow::on_tsImportBtn_clicked()
{
    bool re;

    //import .ts file
    if(ui->tsPathEdit->text().isEmpty()) {
        on_tsLookBtn_clicked();
    }

    QFileInfo info(ui->tsPathEdit->text());
    if (!info.isFile() || "ts" != info.suffix()){
        onReceiveMsg("File type is not supported");
        return;
    }

    m_transList.clear();
    re = m_pXmlWorker->ImportFromTS(m_transList, ui->tsPathEdit->text());

    if(re) {
        onReceiveMsg("import .ts file success");
    } else {
        onReceiveMsg("import .ts file failed");
    }
}

void MainWindow::onReceiveMsg(const QString &msg)
{
    ui->statusBar->showMessage(msg);
}

void MainWindow::on_tsDirLookBtn_clicked()
{
    const QString documentLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString dirName = QFileDialog::getExistingDirectory(this, tr("select .ts dir"), documentLocation);

    if(dirName.isEmpty()){
        return;
    }

    ui->tsDirEdit->setText(dirName);
}

void MainWindow::on_excelDirBtn_clicked()
{
    const QString documentLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString dirName = QFileDialog::getExistingDirectory(this, tr("select excel dir"), documentLocation);

    if(dirName.isEmpty()){
        return;
    }

    ui->excelDirEdit->setText(dirName);
}

void MainWindow::on_generateBtn_2_clicked()
{
    bool re;

    QFileInfo tsDirinfo(ui->tsDirEdit->text());
    if (!tsDirinfo.isDir()){
        onReceiveMsg("ts dir is empty");
        return;
    }

    QFileInfo excelDirinfo(ui->excelDirEdit->text());
    if (!excelDirinfo.isDir()){
        onReceiveMsg("excel dir is empty");
        return;
    }

    QStringList filters;
    filters << QString("*.ts");
    QDir tsdir(ui->tsDirEdit->text());
    tsdir.setFilter(QDir::Files | QDir::NoSymLinks);
    tsdir.setNameFilters(filters);

    if (tsdir.count() <= 0) {
        onReceiveMsg("ts dir ts file is 0");
        return;
    }

    for (QFileInfo info : tsdir.entryInfoList())
    {
        //import ts file
        m_transList.clear();
        re = m_pXmlWorker->ImportFromTS(m_transList, info.absoluteFilePath());

        if(re) {
            onReceiveMsg("import " + info.fileName() + " success");
        } else {
            onReceiveMsg("import " + info.fileName() + " failed");
        }

        //generate excel file
        m_pExcelWorker->SetTransColumn(ui->transSpinBox->value());
        QString excelFileName = ui->excelDirEdit->text() + "/" + info.baseName() + ".xlsx";
        re = m_pExcelWorker->ExportToXlsx(m_transList, excelFileName);
        if(re) {
            onReceiveMsg("export " + excelFileName + " success");
            ui->youdaoTipLabel->setVisible(false);
        } else {
            onReceiveMsg("export " + excelFileName + " failed");
        }
    }
}
