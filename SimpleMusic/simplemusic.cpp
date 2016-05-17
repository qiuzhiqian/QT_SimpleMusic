#include "simplemusic.h"
#include "ui_simplemusic.h"
#include <QFileDialog>
//#include <QDebug>

SimpleMusic::SimpleMusic(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleMusic)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);     //限制最大化
    //setAttribute(Qt::WA_TranslucentBackground);

    musicLen=0;
    getLenFlag=false;
    currentLen=0;

    QPalette palette;
    QPixmap pixmap(":/image/image/background.png");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);

    musicTimer=new QTimer(this);        //1s计时的定时器

    QTableWidgetItem *item00 = new QTableWidgetItem();
    item00->setBackgroundColor(QColor(0,0,0));
    item00->setForeground(QColor(255,255,0));
    item00->setTextColor(QColor(200,111,100));
    item00->setText(tr("歌曲"));
    ui->tableWidget->setHorizontalHeaderItem(0,item00);

    QTableWidgetItem *item01 = new QTableWidgetItem();
    item01->setBackgroundColor(QColor(200,200,200));
    item01->setForeground(QColor(255,255,0));
    item01->setTextColor(QColor(200,111,100));
    item01->setText(tr("类型"));
    ui->tableWidget->setHorizontalHeaderItem(1,item01);

    playList=new QMediaPlaylist();                            //播放列表
    playList->setPlaybackMode(QMediaPlaylist::Loop);        //循环播放
    player=new QMediaPlayer(this);                                //播放器
    player->setPlaylist(playList);                          //将播放列表添加到播放器中
    player->setVolume(80);                                  //设置播放音量
    ui->slider_volume->setValue(80);




    connect(ui->toolButton_Open, SIGNAL(clicked()), this, SLOT(addFile()));             //添加歌曲
    connect(ui->toolButton_Last, SIGNAL(clicked()), this, SLOT(playLast()));            //上一曲
    connect(ui->toolButton_Play, SIGNAL(clicked()), this, SLOT(playOrPause()));         //播放
    connect(ui->toolButton_Stop, SIGNAL(clicked()), this, SLOT(playStop()));              //停止
    connect(ui->toolButton_Next, SIGNAL(clicked()), this, SLOT(playNext()));            //下一曲

    connect(ui->slider_volume,SIGNAL(valueChanged(int)),this,SLOT(sliderPressed(int)));
    connect(musicTimer,SIGNAL(timeout()),this,SLOT(updateProgress()));
    connect(player,SIGNAL(mediaChanged(QMediaContent)),this,SLOT(timeUp()));
}

SimpleMusic::~SimpleMusic()
{
    delete ui;
}

void SimpleMusic::addFile()         //添加文件
{
    QStringList fileNameList=QFileDialog::getOpenFileNames(this,tr("选择文件"), QDir::currentPath(),"Mp3(*.mp3);;WAV(*.wav)");

    for(int i=0; i<fileNameList.size(); ++i)
    {
        QString path=QDir::toNativeSeparators(fileNameList.at(i));
        if(!path.isEmpty())
        {
            playList->addMedia(QUrl::fromLocalFile(path));
            QString fileName=path.split("\\").last();
            int rownum=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);

            QTableWidgetItem *item1 = new QTableWidgetItem();
            item1->setBackgroundColor(QColor(0,0,0));
            item1->setForeground(QColor(255,255,0));
            item1->setTextColor(QColor(255,255,255));
            item1->setText(fileName.split(".").front());
            ui->tableWidget->setItem(rownum, 0, item1);
            //ui->tableWidget->setItem(rownum,1,new QTableWidgetItem());

            QTableWidgetItem *item2 = new QTableWidgetItem();
            item2->setBackgroundColor(QColor(0,0,0));
            item2->setForeground(QColor(255,255,0));
            item2->setTextColor(QColor(200,111,100));
            item2->setTextAlignment(Qt::AlignCenter);
            item2->setText(fileName.split(".").last());
            ui->tableWidget->setItem(rownum, 1, item2);
            //ui->tableWidget->setItem(rownum, 2, new QTableWidgetItem(path));
        }
    }
}

void SimpleMusic::playLast()        //上一曲
{
    int currentIndex=playList->currentIndex();
    if(--currentIndex<0) currentIndex=0;
    playList->setCurrentIndex(currentIndex);
    player->play();
    musicTimer->start(1000);
}

void SimpleMusic::playOrPause()     //播放
{
    //playList->setCurrentIndex(0);
    if(ui->toolButton_Play->text()==tr("播放"))
    {
        player->play();
        musicTimer->start(1000);
        ui->toolButton_Play->setText(tr("暂停"));
        //qDebug()<<"播放";
    }
    else
    {
        player->pause();
        musicTimer->stop();
        ui->toolButton_Play->setText(tr("播放"));
        //qDebug()<<"暂停";
    }
}

void SimpleMusic::playStop()        //终止播放
{
    player->stop();
    musicTimer->stop();
    ui->toolButton_Play->setText(tr("播放"));
    ui->progressBar->setValue(0);
    getLenFlag=false;
    //qDebug()<<"暂停";
}

void SimpleMusic::playNext()        //播放下一曲
{
    int currentIndex=playList->currentIndex();
    if(++currentIndex==playList->mediaCount()) currentIndex=0;
    playList->setCurrentIndex(currentIndex);
    player->play();
    musicTimer->start(1000);
}

void SimpleMusic::sliderPressed(int vol)       //设置音量(0~200)
{
    //qDebug("vol=%d",vol);
    player->setVolume(vol);
}

void SimpleMusic::updateProgress()
{
    currentLen=player->position();
    if(getLenFlag==false)
    {
        getLenFlag=true;
        musicLen=(player->duration());
        ui->progressBar->setMaximum(musicLen);
    }
    ui->progressBar->setValue(currentLen);
    //qDebug()<<"CLen="<<currentLen<<",ALen="<<musicLen;
}

void SimpleMusic::timeUp()
{
    if(getLenFlag==true)
    {
        getLenFlag=false;
    }
}
