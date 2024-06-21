#include "mainwindow.h"
#include "ui_mainwindow.h"
#define DINO ":/images/dino.png"
#define CACTUS1 ":/images/cactus1.png"
#define CACTUS2 ":/images/cactus2.png"
#define BIRD ":/images/bird.png"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    dino = new QGraphicsPixmapItem(QPixmap(DINO));
    scene->addItem(dino);

    dino->setPos(-350,100);

    QStringList obstacleImages = {CACTUS1, CACTUS2, BIRD};
    for (int i = 0; i < 10; ++i) {
        QString obstacleImage = obstacleImages[QRandomGenerator::global()->bounded(obstacleImages.size())];
        QGraphicsPixmapItem *obstacle = new QGraphicsPixmapItem(QPixmap(obstacleImage));
        if (obstacleImage == BIRD)
        {
            obstacle->setPos(2200 + QRandomGenerator::global()->bounded(-1, 2) * 200 + i * 2200, QRandomGenerator::global()->bounded(-1, 1) * 100 + 100);
        } else
        {
            obstacle->setPos(2200 + QRandomGenerator::global()->bounded(-1, 2) * 200 + i * 2200, 100);
        }
        scene->addItem(obstacle);
        obstacles.append(obstacle);
    }


    score = 0;
    highScore = loadHighScore();
    speed = 5.0;
    gameStarted = false;
    scoreText = new QGraphicsTextItem();
    scoreText->setPlainText(QString("Score: %1").arg(int(score)));
    scoreText->setPos(300, -100);
    scene->addItem(scoreText);


    highScoreText = new QGraphicsTextItem();
    highScoreText->setPlainText(QString("High Score: %1").arg(highScore));
    highScoreText->setPos(300, -80);
    scene->addItem(highScoreText);

    gameStartedText = new QGraphicsTextItem();
    gameStartedText->setPlainText(QString("Press space to play"));
    gameStartedText->setPos(0, 0);
    scene->addItem(gameStartedText);

    gameOverText = new QGraphicsTextItem();
    gameOverText->setPlainText(QString("GAME OVER!"));
    gameOverText->setPos(0, 0);
    gameOverText->hide();
    scene->addItem(gameOverText);

    game_timer = new QTimer(this);
    connect(game_timer, &QTimer::timeout, this, &MainWindow::updateGame);
    game_timer->start(16);
    jump_timer = new QTimer(this);
    connect(jump_timer, &QTimer::timeout, this, &MainWindow::endJump);
    jump_timer->setSingleShot(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::loadHighScore() {
    QFile file("highscore.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    QTextStream in(&file);
    return in.readLine().toInt();
}

void MainWindow::saveHighScore(int score) {
    QFile file("highscore.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << score;
}

void MainWindow::updateGame() {
    if (!gameStarted) return;
    for (QGraphicsPixmapItem *obstacle : obstacles) {
        obstacle->setPos(obstacle->x() - speed, obstacle->y());
        if (obstacle->x() < -ui->graphicsView->width()/2) {
            obstacle->setPos(2200, obstacle->y());
        }
        if (dino->collidesWithItem(obstacle)) {
            game_timer->stop();
            gameOverText->show();
            gameStarted = false;
            if (int(score) > highScore) {
                highScore = score;
                highScoreText->setPlainText(QString("High Score: %1").arg(highScore));
                saveHighScore(highScore);
                return;
            }

        }
    }

    score += 0.25;
    if (speed < 25)
        speed += 0.005;
    scoreText->setPlainText(QString("Score: %1").arg(int(score)));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up)
    {
        if (!gameStarted) {
            gameStarted = true;
            gameStartedText->hide();
        } else if (!is_jumping) {
            dino->setPos(dino->x(), dino->y() - 150);
            jump_timer->start(650);
            is_jumping = true;
        }
    }
}

void MainWindow::endJump()
{
    dino->setPos(dino->x(), dino->y() + 150);
    is_jumping = false;
}
