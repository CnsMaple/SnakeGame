
#pragma once

#include <QKeyEvent>
#include <QList>
#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include <QWidget>
#include <qboxlayout.h>

class SnakeGameWidget : public QWidget
{
        Q_OBJECT;

    public:
        SnakeGameWidget(QWidget *parent = nullptr);

        ~SnakeGameWidget() = default;

        void start();
        void end();

        void init();

        void moveSnake();

        bool isEqual(QPoint *p1, QPoint *p2);
        bool isContain(QPoint *p, QList<QPoint *> list);

        void createFood();

        void setWidgetSize(int x, int y);
        void setBlockSize(int size);
        int getBlockSize();
        void setSpeed(int speed);
        int getSpeed();
        int getWidgetAllBlockWidth();
        int getWidgetAllBlockHeight();
        QString getSnakeDirection();
        void setSnakeDirection(QString dir);

        void paintEvent(QPaintEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    signals:
        void gameOver(int mode);
        void getScore(int score);

    private:
        QTimer *timer;
        bool gameStatus = false;
        int blockSize = 20;
        int widgetAllBlockWidth = 10;
        int widgetAllBlockHeight = 10;
        QList<QPoint *> snake;
        QPoint *snakeHead;
        QString direction = "n";
        QString preDirection = "n";
        QPoint *food;
        int score = 0;
        int speed = 250;
};
