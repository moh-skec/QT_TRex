#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int loadHighScore();
    void saveHighScore(int score);
    void updateGame();
    void keyPressEvent(QKeyEvent *event);
    void endJump();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *dino;
    QGraphicsPixmapItem *cactus;
    QList<QGraphicsPixmapItem*> obstacles;
    QTimer *game_timer;
    QTimer *jump_timer;
    bool is_jumping;
    QGraphicsTextItem *scoreText;
    bool gameStarted;
    QGraphicsTextItem *gameStartedText;
    QGraphicsTextItem *gameOverText;
    float score;
    QGraphicsTextItem *highScoreText;
    int highScore;
    double speed;
};
#endif // MAINWINDOW_H
