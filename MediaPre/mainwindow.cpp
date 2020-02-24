#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QtWinExtras>

// 若MSVC 编译版本错误，修改 msvc-version.conf 文件
// 解决MSVC编译时，界面汉字乱码的问题
#if _MSC_VER >= 1600     //MSVC2015>1899,    MSVC_VER= 14.0
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("music.png"));
    setWindowTitle(QStringLiteral("Music Player"));
    ui->PlayBtn->setIcon(QIcon("play.png"));
    ui->PreBtn->setIcon(QIcon("pre.png"));
    ui->NextBtn->setIcon(QIcon("next.png"));
    ui->OpenBtn->setIcon(QIcon("list.png"));
    ui->VolumBtn->setIcon(QIcon("volume.png"));
    //设置封面
    QPixmap Pix = QPixmap("Jane.jpg");
    QSize pixsize = ui->label->size();
    Pix.scaled(pixsize,Qt::KeepAspectRatioByExpanding);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(Pix);

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);

    //默认开始播放默认歌曲
    playlist->addMedia(QUrl::fromLocalFile("music/love.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("music/All of Me.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("music/G.mp3"));

    player->setMedia(playlist);
    //默认播放设置
    ui->listWidget->addItem("love-Alin.mp3");
    ui->listWidget->addItem("All of Me-Jane.mp3");
    ui->listWidget->addItem("G大调的悲伤-Jane.mp3");
    setWindowTitle(QStringLiteral("Music Player-Love-Alin.mp3"));
    player->setVolume(30);
    ui->VolumSplider->setValue(30);
    ui->VolumBox->setValue(30);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);//设置循环模式
    player->play();

    connect(player,SIGNAL(positionChanged(qint64)),
            this, SLOT(onPositionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),
            this, SLOT(onDurationChanged(qint64)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),
            this, SLOT(onPlaylistChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//下一首
void MainWindow::on_NextBtn_clicked()
{
    playlist->next();
    qDebug()<<"currentindex:"<<playlist->currentIndex()<<endl;
}

//上一首
void MainWindow::on_PreBtn_clicked()
{
    playlist->previous();
    qDebug()<<"currentindex:"<<playlist->currentIndex()<<endl;
}

//播放
void MainWindow::on_PlayBtn_clicked()
{
    if(player->state()==QMediaPlayer::StoppedState||player->state()==QMediaPlayer::PausedState){
        player->play();
        ui->PlayBtn->setIcon(QIcon("pause.png"));
    }else if(player->state()==QMediaPlayer::PlayingState){
        ui->PlayBtn->setIcon(QIcon("play.png"));
        player->stop();
    }

}

//打开文件
void MainWindow::on_OpenBtn_clicked()
{
    QString curPath = QDir::homePath();//获取用户目录
    QString dlgTitle = QStringLiteral("请选择音频文件");
    QString filter = QStringLiteral("音频文件(*.mp3 *wav *wma);;mp3文件(*mp3);;"
                                    "wav文件(*wav);;wma文件(*wma);;所有文件(*.*)");
    QStringList fileList = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);
    if(fileList.count()<1){
        qDebug()<<"open false"<<endl;
        return;
    }
    for(int i=0;i<fileList.count();i++){
        QString aFile = fileList.at(i);
        playlist->addMedia(QUrl::fromLocalFile(aFile));
        QFileInfo fileInfo(aFile);//？？？
        ui->listWidget->addItem(fileInfo.fileName());//添加到文件列表
    }
}

//音量键，按下后静音
void MainWindow::on_VolumBtn_clicked()
{
    bool mute=player->isMuted();
    player->setMuted(!mute);
    if (mute)
        ui->VolumBtn->setIcon(QIcon("volume.png"));
    else
        ui->VolumBtn->setIcon(QIcon("mute.png"));

}

//音量进度条改变
void MainWindow::on_VolumSplider_valueChanged(int value)
{
    player->setVolume(value);
    ui->VolumBox->setValue(value);
}

//歌曲进度条调整
void MainWindow::on_ScheSlider_valueChanged(int value)
{
    player->setPosition(value);
}

//播放列表变化,更新当前播放文件名显示
void MainWindow::onPlaylistChanged(int position)
{
    if(playlist->currentIndex()%3==0){
        QPixmap Pix = QPixmap("Jane2.jpg");
        QSize pixsize = ui->label->size();
        Pix.scaled(pixsize,Qt::KeepAspectRatioByExpanding);
        ui->label->setScaledContents(true);
        ui->label->setPixmap(Pix);
    }else if(playlist->currentIndex()%3==1){
        QPixmap Pix = QPixmap("Jane.jpg");
        QSize pixsize = ui->label->size();
        Pix.scaled(pixsize,Qt::KeepAspectRatioByExpanding);
        ui->label->setScaledContents(true);
        ui->label->setPixmap(Pix);
    }else if(playlist->currentIndex()%3==2){
        QPixmap Pix = QPixmap("Alin.jpg");
        QSize pixsize = ui->label->size();
        Pix.scaled(pixsize,Qt::KeepAspectRatioByExpanding);
        ui->label->setScaledContents(true);
        ui->label->setPixmap(Pix);
    }
    ui->listWidget->setCurrentRow(position);
    QListWidgetItem  *item=ui->listWidget->currentItem();
    if (item){
        setWindowTitle(QStringLiteral("Music Player-")+item->text());
    }
}

//文件时长变化，更新进度显示
void MainWindow::onDurationChanged(qint64 duration)
{
     ui->ScheSlider->setMaximum(duration);
     int   secs=duration/1000;//秒
     int   mins=secs/60; //分钟
     secs=secs % 60;//余数秒
     durationTime=QString::asprintf("%d:%d",mins,secs);
     ui->PositionLabel->setText(positionTime+"/"+durationTime);
}

//双击切换歌曲
void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int rowNo=index.row();
    qDebug()<<"currentindex:"<<playlist->currentIndex()<<endl;
    playlist->setCurrentIndex(rowNo);
    player->play();
}

//当前文件播放位置变化，更新进度显示
void MainWindow::onPositionChanged(qint64 position)
{
    if (ui->ScheSlider->isSliderDown())
        return;

    ui->ScheSlider->setSliderPosition(position);

    int   secs=position/1000;//秒
    int   mins=secs/60; //分钟
    secs=secs % 60;//余数秒
    positionTime=QString::asprintf("%d:%d",mins,secs);
    ui->PositionLabel->setText(positionTime+"/"+durationTime);
}

//音量SpiBox改变
void MainWindow::on_VolumBox_valueChanged(int arg1)
{
    ui->VolumSplider->setValue(arg1);
    player->setVolume(arg1);
}

