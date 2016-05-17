#ifndef SIMPLEMUSIC_H
#define SIMPLEMUSIC_H

#include <QMainWindow>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QTimer>

namespace Ui {
class SimpleMusic;
}

class SimpleMusic : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleMusic(QWidget *parent = 0);
    ~SimpleMusic();

private slots:
    void addFile();
    void playLast();
    void playOrPause();
    void playStop();
    void playNext();

    void sliderPressed(int vol);
    void updateProgress();
    void timeUp();

private:
    Ui::SimpleMusic *ui;

    QMediaPlayer *player;           //播放器
    QMediaPlaylist *playList;       //播放列表

    qint64 musicLen;
    bool getLenFlag;
    qint64 currentLen;

    QTimer *musicTimer;                  //创建一个定时器
};

#endif // SIMPLEMUSIC_H
