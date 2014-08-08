#include "gifs2png_gui.h"
#include "ui_gifs2png_gui.h"
#include <QMessageBox>
#include <QFileDialog>
#include "../dev_console/devconsole.h"

gifs2png_gui::gifs2png_gui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gifs2png_gui)
{
    ui->setupUi(this);
    proc= new QProcess();
}

gifs2png_gui::~gifs2png_gui()
{
    delete proc;
    delete ui;
}

void gifs2png_gui::on_BrowseInput_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Source Directory"),
                                                 ui->inputDir->text().isEmpty()?QApplication::applicationDirPath():ui->inputDir->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty()) return;

    ui->inputDir->setText(dir);

}

void gifs2png_gui::on_BrowseOutput_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Target Directory"),
                                                 ui->outputDir->text().isEmpty()?QApplication::applicationDirPath():ui->outputDir->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dir.isEmpty()) return;

    ui->outputDir->setText(dir);
}

void gifs2png_gui::on_startTool_clicked()
{
    if(ui->inputDir->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Source directory is not set"), tr("Please, set the source directory"), QMessageBox::Ok);
        return;
    }

    QString command;

    #ifdef _WIN32
    command = QApplication::applicationDirPath()+"/GIFs2PNG.exe";
    #else
    command = QApplication::applicationDirPath()+"/GIFs2PNG";
    #endif

    if(!QFile(command).exists())
    {
        QMessageBox::warning(this, tr("Tool is not found"), tr("Can't run application: \n%1\nPlease, check the application directory.").arg(command), QMessageBox::Ok);
        return;
    }

    QStringList args;
    args << ui->inputDir->text();
    if(!ui->outputDir->text().isEmpty()) args << QString("-O%1").arg(ui->outputDir->text());

    if(ui->WalkSubDirs->isChecked()) args << "-W";
    if(ui->RemoveSource->isChecked()) args << "-R";
    args << "--nopause";

    DevConsole::show();
    DevConsole::log("Ready>>>", "GIFs2PNG");
    DevConsole::log("----------------------------------", "GIFs2PNG", true);
    proc->waitForFinished(1);
    proc->start(command, args);
    connect(proc, SIGNAL(readyReadStandardOutput()),this, SLOT(consoleMessage()) );
    //connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(consoleMessage()) );
}

void gifs2png_gui::consoleMessage()
{
    QByteArray strdata = proc->readAllStandardOutput();
    DevConsole::log(strdata, "GIFs2PNG");
}

void gifs2png_gui::on_close_clicked()
{
    this->close();
}