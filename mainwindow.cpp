#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "implementation/lfsrdata.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->edtSimpleLFSR1->setMaxLength(LFSRData::LFSR1_BITFIELD_SIZE);
    ui->edtGeffeLFSR1->setMaxLength(LFSRData::LFSR1_BITFIELD_SIZE);
    ui->edtGeffeLFSR2->setMaxLength(LFSRData::LFSR2_BITFIELD_SIZE);
    ui->edtGeffeLFSR3->setMaxLength(LFSRData::LFSR3_BITFIELD_SIZE);
    setInputValidators();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setInputValidators()
{
    QValidator *validatorBinary = new QRegExpValidator(QRegExp("[01]*"), this);
    ui->edtSimpleLFSR1->setValidator(validatorBinary);
    ui->edtGeffeLFSR1->setValidator(validatorBinary);
    ui->edtGeffeLFSR2->setValidator(validatorBinary);
    ui->edtGeffeLFSR1->setValidator(validatorBinary);
}

void MainWindow::enableSimpleStreamEncryptionMode(bool enabled)
{
    ui->grpSimple->setEnabled(enabled);

    if (enabled) {
        enableGeffeCipherMode(false);
        enableRC4Mode(false);
    }
}

void MainWindow::enableGeffeCipherMode(bool enabled)
{
    ui->grpGeffe->setEnabled(enabled);

    if (enabled) {
        enableSimpleStreamEncryptionMode(false);
        enableRC4Mode(false);
    }
}

void MainWindow::enableRC4Mode(bool enabled)
{
    ui->grpRC4->setEnabled(enabled);

    if (enabled) {
        enableGeffeCipherMode(false);
        enableSimpleStreamEncryptionMode(false);
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
            mbError.setText("Initial key required for procedure execution!");
            break;
        case E_INVALID_KEY:
            mbError.setText("Invalid key value!");
            break;
        default:
            mbError.setText("Unknown error!");
            break;
    }

    mbError.exec();
}

void MainWindow::runSimpleStreamEncryptionMode(QString inputFileName, QString outputFileName)
{
    ui->txtLog->appendPlainText("Mode: Simple Stream");

    if (ui->edtSimpleLFSR1->text().isEmpty()) {
        ui->txtLog->appendPlainText("Error: initial key not found!");
        ui->txtLog->appendPlainText("\nProcess failed.");
        displayError(E_KEY_NOT_FOUND);
    } else {
        LFSRData::ContentLFSR1 initKey;
        initKey.value = ui->edtSimpleLFSR1->text().toULong(0, 2);

        ui->txtLog->appendPlainText("LFSR1 init key: " + ui->edtSimpleLFSR1->text());

        MainWindow::setEnabled(false);

        SimpleCryptWorker *worker = new SimpleCryptWorker(inputFileName, outputFileName, initKey);
        QThread *workerThread = getWorkerThread(worker);

        workerThread->start();
        emit doWork();
    }
}

void MainWindow::runGeffeMode(QString inputFileName, QString outputFileName)
{
    ui->txtLog->appendPlainText("Mode: Geffe Cipher");

    if (ui->edtGeffeLFSR1->text().isEmpty() || ui->edtGeffeLFSR2->text().isEmpty() ||
            ui->edtGeffeLFSR3->text().isEmpty()) {
        ui->txtLog->appendPlainText("Error: initial key not found!");
        ui->txtLog->appendPlainText("\nProcess failed.");
        displayError(E_KEY_NOT_FOUND);
    } else {
        LFSRData::ContentLFSR1 initKeyLFSR1;
        LFSRData::ContentLFSR2 initKeyLFSR2;
        LFSRData::ContentLFSR3 initKeyLFSR3;
        initKeyLFSR1.value = ui->edtGeffeLFSR1->text().toULong(0, 2);
        initKeyLFSR2.value = ui->edtGeffeLFSR2->text().toULong(0, 2);
        initKeyLFSR3.value = ui->edtGeffeLFSR3->text().toULong(0, 2);

        ui->txtLog->appendPlainText("LFSR1 init key: " + ui->edtGeffeLFSR1->text());
        ui->txtLog->appendPlainText("LFSR2 init key: " + ui->edtGeffeLFSR2->text());
        ui->txtLog->appendPlainText("LFSR3 init key: " + ui->edtGeffeLFSR3->text());

        MainWindow::setEnabled(false);

        Worker *worker = new GeffeWorker(inputFileName, outputFileName, initKeyLFSR1, initKeyLFSR2, initKeyLFSR3);
        QThread *workerThread = getWorkerThread(worker);

        workerThread->start();
        emit doWork();
    }
}

void MainWindow::runRC4Mode(QString inputFileName, QString outputFileName)
{
    const int KEY_PART_MAX = 255;

    ui->txtLog->appendPlainText("Mode: RC4");

    if (ui->edtRC4Key->text().isEmpty()) {
        ui->txtLog->appendPlainText("Error: secret key not found");
        ui->txtLog->appendPlainText("\nProcess failed.");
        displayError(E_KEY_NOT_FOUND);
    } else {
        std::vector<quint8> secretKey;
        QStringList secretKeyParts = ui->edtRC4Key->text().split(QRegExp(" "));
        for (QString &str : secretKeyParts) {
            bool isValidRC4Key;
            uint currentValue = str.toUInt(&isValidRC4Key);
            if (isValidRC4Key && currentValue <= KEY_PART_MAX) {
                secretKey.push_back(str.toUInt());
            } else {
                ui->txtLog->appendPlainText("Error: invalid secret key");
                ui->txtLog->appendPlainText("\nProcess failed.");
                displayError(E_INVALID_KEY);
                return;
            }
        }

        ui->txtLog->appendPlainText("RC4 secret key: " + ui->edtRC4Key->text());

        MainWindow::setEnabled(false);

        Worker *worker = new RC4Worker(inputFileName, outputFileName, secretKey);
        QThread *workerThread = getWorkerThread(worker);

        workerThread->start();
        emit doWork();
    }
}

QThread *MainWindow::getWorkerThread(Worker *worker)
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
    Worker *worker = static_cast<Worker *>(sender());
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
    ui->txtLog->appendPlainText("Input file: " + inputFileName);
    ui->txtLog->appendPlainText("Input file size: " + QString::number(inputFile.size()) + " byte");
    ui->txtLog->appendPlainText("Output file: " + outputFileName);

    if (ui->rbtnSimple->isChecked()) {
        runSimpleStreamEncryptionMode(inputFileName, outputFileName);
        return;
    }
    if (ui->rbtnGeffe->isChecked()) {
        runGeffeMode(inputFileName, outputFileName);
        return;
    }
    if (ui->rbtnRC4->isChecked()) {
        runRC4Mode(inputFileName, outputFileName);
        return;
    }
}
