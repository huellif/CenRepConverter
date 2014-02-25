#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>

#if QT_VERSION >= 0x050000
#include <QWidget>
#include <QtWidgets>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
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
            exit(EXIT_FAILURE);
        }
    }
}

void MainWindow::saveSettings(const QString &path)
{
    QSettings settings("settings/test.ini",QSettings::IniFormat);
    settings.beginGroup("settings");
    settings.setValue("epoc32",path);
    settings.endGroup();
}

void MainWindow::dropEvent(QDropEvent *ev)
{
    QList<QUrl> urls = ev->mimeData()->urls();
    foreach(QUrl url, urls)
    {
        QFileInfo info(url.toString());
        qDebug() << url.toString();
        if(info.baseName().length()==8 && (info.suffix()=="txt" || info.suffix()=="cre"))
        {
            QFile file(epoc32+"winscw/c/"+info.fileName());
            if(file.exists())
            {
                file.remove();
            }
            if(QFile::copy(url.toString().mid(8),epoc32+"winscw/c/"+info.fileName()))
            {
                if(info.suffix()==QLatin1String("txt"))
                {
                    system(QString(epoc32+"release/winscw/udeb/centrepconv.exe -nowait -Dtextshell -- -o c:\\"+info.baseName()+".cre c:\\"+info.baseName()+".txt""").toLocal8Bit().data());
                    QFile::copy(epoc32+"winscw/c/"+info.baseName()+".cre",url.toString().mid(8).replace(QString(".txt"),QString(".cre")));
                    QFile (epoc32+"winscw/c/"+info.baseName()+".cre").remove();
                }

                if(info.suffix()==QLatin1String("cre"))
                {
                    system(QString(epoc32+"release/winscw/udeb/centrepconv.exe -nowait -Dtextshell -- -o c:\\"+info.baseName()+".txt c:\\"+info.baseName()+".cre""").toLocal8Bit().data());
                    QFile::copy(epoc32+"winscw/c/"+info.baseName()+".txt",url.toString().mid(8).replace(QString(".cre"),QString(".txt")));
                    QFile(epoc32+"winscw/c/"+info.baseName()+".txt").remove();
                }
                file.remove();
            }
        }
    }
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
