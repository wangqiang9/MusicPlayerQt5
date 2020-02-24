#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QStringList>
#include <QStackedWidget>
#include <QPixmap>
#include <QtWinExtras>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QSettings>
#include <QIcon>
#include <QDir>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //自定义槽函数
    void onPlaylistChanged(int position);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);

    void on_NextBtn_clicked();

    void on_PreBtn_clicked();

    void on_PlayBtn_clicked();

    void on_OpenBtn_clicked();

    void on_VolumBtn_clicked();

    void on_VolumSplider_valueChanged(int value);

    void on_ScheSlider_valueChanged(int value);

    void on_listWidget_doubleClicked(const QModelIndex &index);


    void on_VolumBox_valueChanged(int arg1);

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QString  durationTime;//总长度
    QString  positionTime;//当前播放到位置

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
