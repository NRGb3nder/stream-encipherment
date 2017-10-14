#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "implementation/lfsrdata.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->edtSimpleLFSR->setMaxLength(LFSRData::LFSR1_BITFIELD_SIZE);
    setInputValidators();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setInputValidators()
{
    QValidator *inputValidator = new QRegExpValidator(QRegExp("[01]*"), this);
    ui->edtSimpleLFSR->setValidator(inputValidator);
}

void MainWindow::enableSimpleStreamEncryptionMode(bool enabled)
{
    ui->grpSimple->setEnabled(enabled);

    if (enabled) {
        enableGeffeCipherMode(false);
        enableRC4Mode(false);
        ui->grpMode->setEnabled(false);
    }
}

void MainWindow::enableGeffeCipherMode(bool enabled)
{
    ui->grpGeffe->setEnabled(enabled);

    if (enabled) {
        enableSimpleStreamEncryptionMode(false);
        enableRC4Mode(false);
        ui->grpMode->setEnabled(true);
    }
}

void MainWindow::enableRC4Mode(bool enabled)
{
    ui->grpRC4->setEnabled(enabled);

    if (enabled) {
        enableGeffeCipherMode(false);
        enableSimpleStreamEncryptionMode(false);
        ui->grpMode->setEnabled(true);
    }
}

void MainWindow::displayError(ErrorType error)
{
    QMessageBox mbError;
    mbError.setWindowTitle("Error");
    mbError.setIcon(QMessageBox::Critical);

    switch (error) {
        case E_INVALID_INPUT_FILE:
            mbError.setText("File input error!");
            break;
        case E_INVALID_OUTPUT_FILE:
            mbError.setText("File output error!");
            break;
        case E_KEY_NOT_FOUND:
            mbError.setText("Binary key required for procedure execution!");
            break;
        default:
            mbError.setText("Unknown error!");
            break;
    }

    mbError.exec();
}

void MainWindow::runSimpleStreamEncryptionMode(QString inputFileName, QString outputFileName)
{
    ui->txtLog->appendPlainText("Mode: Simple Stream Encryption/Decryption");

    if (ui->edtSimpleLFSR->text().isEmpty()) {
        ui->txtLog->appendPlainText("Error: LFSR1 initial key not found!");
        ui->txtLog->appendPlainText("\nProcess failed.");
        displayError(E_KEY_NOT_FOUND);
    } else {
        LFSRData::ContentLFSR1 initKey;
        initKey.value = ui->edtSimpleLFSR->text().toUInt(0, 2);

        ui->txtLog->appendPlainText("LFSR1 init key: " + ui->edtSimpleLFSR->text());

        MainWindow::setEnabled(false);

        CryptWorker *worker = new CryptWorker(inputFileName, outputFileName, initKey);
        QThread *workerThread = getCryptWorkerThread(worker);

        workerThread->start();
        emit doWork();
    }
}

QThread *MainWindow::getCryptWorkerThread(CryptWorker *worker)
{
    QThread *workerThread = new QThread();

    connect(this, SIGNAL(destroyed()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(destroyed()), workerThread, SLOT(quit()));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    worker->moveToThread(workerThread);
    connect(this, SIGNAL(doWork()), worker, SLOT(startWork()));

    connect(worker, SIGNAL(done(QString, QString, QString)), this, SLOT(workDone(QString, QString, QString)));
    connect(worker, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));

    return workerThread;
}

void MainWindow::workDone(QString sourceContent, QString resultContent, QString keyContent)
{
    CryptWorker *worker = static_cast<CryptWorker *>(sender());
    delete worker;

    ui->txtResult->appendPlainText(resultContent);
    ui->txtSource->appendPlainText(sourceContent);
    ui->txtKey->appendPlainText(keyContent);
    ui->txtLog->appendPlainText("\nProcess accomplished.");

    MainWindow::setEnabled(true);

    QMessageBox::information(this, "Info", "Process accomplished!");
    ui->progressBar->setValue(0);
}

void MainWindow::on_rbtnSimple_clicked()
{
    enableSimpleStreamEncryptionMode(true);
}

void MainWindow::on_rbtnGeffe_clicked()
{
    enableGeffeCipherMode(true);
}

void MainWindow::on_rbtnRC4_clicked()
{
    enableRC4Mode(true);
}

void MainWindow::on_btnChooseInput_clicked()
{
    QString inputFileName = QFileDialog::getOpenFileName(this, "Open file", 0, "All files (*)");
    if (!inputFileName.isNull()) {
        ui->edtInputFile->setText(inputFileName);
    }
}

void MainWindow::on_btnChooseOutput_clicked()
{
    QString outputFileName = QFileDialog::getSaveFileName(this, "Save file", 0, "All files (*)");
    if (!outputFileName.isNull()) {
        ui->edtOutputFile->setText(outputFileName);
    }
}

void MainWindow::on_btnExecute_clicked()
{
    QString inputFileName = ui->edtInputFile->text();
    QString outputFileName = ui->edtOutputFile->text();
    QFile inputFile(inputFileName);
    QFile outputFile(outputFileName);

    ui->txtResult->clear();
    ui->txtSource->clear();
    ui->txtKey->clear();
    ui->txtLog->clear();

    if (!inputFile.open(QIODevice::ReadOnly)) {
        displayError(E_INVALID_INPUT_FILE);
        return;
    }
    if (!outputFile.open(QIODevice::WriteOnly)) {
        displayError(E_INVALID_OUTPUT_FILE);
        return;
    }
    inputFile.close();
    outputFile.close();

    ui->txtLog->clear();
    ui->txtLog->appendPlainText("Input file size: " + QString::number(inputFile.size()) + " byte");

    if (ui->rbtnSimple->isChecked()) {
        runSimpleStreamEncryptionMode(inputFileName, outputFileName);
    }
}
