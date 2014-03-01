#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

#if QT_VERSION >= 0x050000
#include <QWidget>
#include <QtWidgets>
#endif

#define _UNICODE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //epoc32="C:/Nokia/devices/Nokia_Symbian_Belle_SDK_v1.0/epoc32/";
    loadSettings();
    ui->lineEdit->setText(epoc32);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("settings/test.ini",QSettings::IniFormat);
    settings.beginGroup("settings");
    epoc32=settings.value("epoc32","C:/Nokia/devices/Nokia_Symbian_Belle_SDK_v1.0/epoc32/").toString();
    settings.endGroup();

    QDir folder(epoc32);
    if(!folder.exists())
    {
        on_Load_clicked();
        QDir folder2(epoc32);
        if(!folder2.exists())
        {
            exit(1);
        }
    }
}

void MainWindow::saveSettings(const QString &path)
{
    QDir test(path);
    if(test.exists())
    {
        QSettings settings("settings/test.ini",QSettings::IniFormat);
        settings.beginGroup("settings");
        settings.setValue("epoc32",path);
        settings.endGroup();
    }
    else
    {
        QMessageBox::warning(0, "Error", path + " is no valid folder");
        ui->lineEdit->setText(epoc32);
    }
}

void MainWindow::convert(const QList<QUrl> &urls)
{
    foreach(QUrl url, urls)
    {
        QFileInfo info(url.toString());
        QString srcfile = url.toString().mid(8);
        qDebug() << url.toString() ;
        if(info.baseName().length()==8 && (info.suffix()=="txt" || info.suffix()=="cre"))
        {

            ui->output->showMessage("Converting " + srcfile + " ...");

            QFile file(epoc32+"winscw/c/"+info.fileName());

            if(file.exists())
            {
                file.remove();
            }

            if(QFile::copy(srcfile,epoc32+"winscw/c/"+info.fileName()))
            {
                if(info.suffix()==QLatin1String("txt"))
                {
                    //system(QString(epoc32+"release/winscw/udeb/centrepconv.exe -nowait -Dtextshell -- -o c:\\"+info.baseName()+".cre c:\\"+info.baseName()+".txt""").toLocal8Bit().data());

                    STARTUPINFO si;
                    PROCESS_INFORMATION pi;

                    ZeroMemory( &si, sizeof(si) );
                    si.cb = sizeof(si);
                    ZeroMemory( &pi, sizeof(pi) );

                    if(CreateProcessW(NULL,
                                      (LPWSTR)QString(epoc32+"release/winscw/udeb/centrepconv.exe -nowait -Dtextshell -- -o c:\\"+info.baseName()+".cre c:\\"+info.baseName()+".txt""").toStdWString().c_str(),
                                      NULL,NULL,FALSE,
                                      NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
                                      NULL,NULL,
                                      &si,
                                      &pi
                                      ))
                    {
                        WaitForSingleObject( pi.hProcess, INFINITE );
                        CloseHandle( pi.hProcess );
                        CloseHandle( pi.hThread );

                    }
                    if(QFile::exists(epoc32+"winscw/c/"+info.baseName()+".cre"))
                    {
                        QFile::remove(srcfile.replace(QString(".txt"),QString(".cre")));
                        QFile::copy(epoc32+"winscw/c/"+info.baseName()+".cre",srcfile.replace(QString(".txt"),QString(".cre")));
                        QFile (epoc32+"winscw/c/"+info.baseName()+".cre").remove();
                        ui->output->showMessage("Done, converted " + srcfile, 1800);
                    }
                    else
                    {
                        ui->output->showMessage("Couldn't convert " + srcfile, 2000);
                    }

                }

                if(info.suffix()==QLatin1String("cre"))
                {
                    //system(QString(epoc32+"release/winscw/udeb/centrepconv.exe -nowait -Dtextshell -- -o c:\\"+info.baseName()+".txt c:\\"+info.baseName()+".cre""").toLocal8Bit().data());


                    STARTUPINFO si;
                    PROCESS_INFORMATION pi;

                    ZeroMemory( &si, sizeof(si) );
                    si.cb = sizeof(si);
                    ZeroMemory( &pi, sizeof(pi) );
                    if(CreateProcess(NULL,
                                      (LPWSTR)QString(epoc32+"release/winscw/udeb/centrepconv.exe -nowait -Dtextshell -- -o c:\\"+info.baseName()+".txt c:\\"+info.baseName()+".cre""").toStdWString().c_str(),
                                      NULL,NULL,FALSE,
                                      NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
                                      NULL,NULL,
                                      &si,
                                      &pi
                                      ))
                    {
                        WaitForSingleObject( pi.hProcess, INFINITE );
                        CloseHandle( pi.hProcess );
                        CloseHandle( pi.hThread );

                    }
                    if(QFile::exists(epoc32+"winscw/c/"+info.baseName()+".txt"))
                    {
                        QFile::remove(srcfile.replace(QString(".cre"),QString(".txt")));
                        QFile::copy(epoc32+"winscw/c/"+info.baseName()+".txt",srcfile.replace(QString(".cre"),QString(".txt")));
                        QFile(epoc32+"winscw/c/"+info.baseName()+".txt").remove();
                        ui->output->showMessage("Done", 1800);
                    }
                    else
                    {
                        ui->output->showMessage("Couldn't convert " + srcfile, 2000);
                    }
                }
                file.remove();

            }
        }
        else
        {
            ui->output->showMessage(srcfile + " is no valid cenrep file!", 1800);
            qDebug() << "error";
        }
    }
}

void MainWindow::convertFolder(QDir &dir)
{
    QStringList filters;
    filters << "*.cre" << "*.txt";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    QList<QUrl> urls;
    foreach (QFileInfo a, list)
    {
        urls << a.absoluteFilePath().prepend("file:///");
    }
    if(urls.isEmpty())
    {
        ui->output->showMessage(dir.absolutePath() + " contains no valid cenrep file!", 1800);
    }
    else
    {
       QtConcurrent::run(this, &MainWindow::convert, urls);
    }
}

void MainWindow::dropEvent(QDropEvent *ev)
{
    QList<QUrl> urls = ev->mimeData()->urls();
    QList<QUrl> urls2;

    foreach(QUrl url, urls)
    {
        if(QDir(url.toString().mid(8)).exists())
        {
            QDir dir(url.toString().mid(8));
            convertFolder(dir);
        }
        else
        {
            urls2 << url;
        }
    }
    QtConcurrent::run(this, &MainWindow::convert, urls2);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *ev)
{
    ev->accept();
}

void MainWindow::on_Load_clicked()
{
    QString path = QFileDialog::getExistingDirectory(0,"Choose your epoc32 folder",epoc32,QFileDialog::ShowDirsOnly)+"/";
    if(path!=QLatin1String("/"))
    {
        epoc32=path;
        ui->lineEdit->setText(path);
        on_Save_clicked();
    }
}

void MainWindow::on_Save_clicked()
{
    saveSettings(ui->lineEdit->text());
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionA_file_triggered()
{
    QList<QUrl> urls = QFileDialog::getOpenFileUrls(0, "Choose a .txt or .cre file", QUrl(), "CenRep files (*.cre *.txt)");
    if(urls.isEmpty()) return;
    QList<QUrl> urls2;
    foreach(QUrl url, urls)
    {
        QFileInfo info(url.toString());
        if(info.baseName().length()!=8) urls2 << url ;
    }
    if(urls2.isEmpty()) return;
    QtConcurrent::run(this, &MainWindow::convert, urls2);
    //convert(urls2);
}

void MainWindow::on_actionA_folder_triggered()
{
    QString url = QFileDialog::getExistingDirectory(0, "Choose a folder which contains cenrep files");
    if(url.isEmpty()) return;
    QDir folder(url);
    convertFolder(folder);
}

void MainWindow::on_actionDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HJ62F4CEATR2N"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(0, "About CenRepConverter", "CenRepConverter 1.2 by Fabian Hüllmantel\n\nThanks to:\n- Mesut\n- Motaz\n- Juampy\n\nBased on Qt 5");
}
