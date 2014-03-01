#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>
#include <QUrl>
#include <QDebug>
#include <QString>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString epoc32;
    void loadSettings();
    void saveSettings(const QString &path);
    void convert(QList<QUrl> &urls);
    void convertFolder(QDir &dir);

protected:
    void dropEvent(QDropEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
private slots:

    void on_Load_clicked();
    void on_Save_clicked();
    void on_actionExit_triggered();
    void on_actionA_file_triggered();
    void on_actionA_folder_triggered();
    void on_actionDonate_triggered();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
