
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
        SnakeGameWidget(QWidget *parent = nullptr) : QWidget(parent)
        {
            timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, &SnakeGameWidget::moveSnake);
            QVBoxLayout *layout = new QVBoxLayout();

            setMinimumSize(blockSize * 10, blockSize * 10);
            init();
            setLayout(layout);
        }

        ~SnakeGameWidget()
        {
        }

        void start()
        {
            timer->start(speed);
            gameStatus = true;
            init();
            setFixedSize(blockSize * widgetAllBlockWidth, blockSize * widgetAllBlockHeight);
            update();
        }

        void end()
        {
            if (gameStatus)
            {
                emit gameOver(1);
            }
            gameStatus = false;
            score = 0;
            setMinimumSize(blockSize * 10, blockSize * 10);
            setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
            timer->stop();
            update();
        }

        void init()
        {
            snake.clear();
            snakeHead = new QPoint(widgetAllBlockWidth / 2, widgetAllBlockHeight / 2);
            direction = "n";
            createFood();
            update();
        }

        void moveSnake()
        {
            // qDebug() << "Play";

            if (direction == "u" && preDirection == "d")
            {
                direction = preDirection;
            }
            else if (direction == "d" && preDirection == "u")
            {
                direction = preDirection;
            }
            else if (direction == "l" && preDirection == "r")
            {
                direction = preDirection;
            }
            else if (direction == "r" && preDirection == "l")
            {
                direction = preDirection;
            }

            preDirection = direction;

            if (direction == "n")
            {
                return;
            }
            else if (direction == "d")
            {
                snake.append(new QPoint(snakeHead->x(), snakeHead->y()));
                snakeHead->setY(snakeHead->y() + 1);
            }
            else if (direction == "l")
            {
                snake.append(new QPoint(snakeHead->x(), snakeHead->y()));
                snakeHead->setX(snakeHead->x() - 1);
            }
            else if (direction == "r")
            {
                snake.append(new QPoint(snakeHead->x(), snakeHead->y()));
                snakeHead->setX(snakeHead->x() + 1);
            }
            else if (direction == "u")
            {
                snake.append(new QPoint(snakeHead->x(), snakeHead->y()));
                snakeHead->setY(snakeHead->y() - 1);
            }

            if (isContain(snakeHead, snake))
            {
                end();
                return;
            }

            if (snakeHead->x() < 0 || snakeHead->x() >= widgetAllBlockWidth || snakeHead->y() < 0
                || snakeHead->y() >= widgetAllBlockHeight)
            {
                end();
                return;
            }
            if (isEqual(snakeHead, food))
            {
                score++;
                emit getScore(score);
                createFood();
            }
            else
            {
                snake.removeFirst();
            }
            update();
        }

        bool isEqual(QPoint *p1, QPoint *p2)
        {
            if (p1->x() == p2->x() && p1->y() == p2->y())
            {
                return true;
            }
            return false;
        }

        bool isContain(QPoint *p, QList<QPoint *> list)
        {
            for (int i = 0; i < list.size(); i++)
            {
                QPoint *point = list.at(i);
                if (isEqual(p, point))
                {
                    return true;
                }
            }
            return false;
        }

        void createFood()
        {
            if (score == widgetAllBlockWidth * widgetAllBlockHeight - 1)
            {
                end();
                return;
            }
            food = new QPoint(QRandomGenerator::global()->bounded(widgetAllBlockWidth),
                              QRandomGenerator::global()->bounded(widgetAllBlockHeight));
            while (isContain(food, snake) || isEqual(food, snakeHead))
            {
                food = new QPoint(QRandomGenerator::global()->bounded(widgetAllBlockWidth),
                                  QRandomGenerator::global()->bounded(widgetAllBlockHeight));
            }
        }

        void setWidgetSize(int x, int y)
        {
            widgetAllBlockWidth = x;
            widgetAllBlockHeight = y;
            // qDebug() << "blockCountX:" << widgetAllBlockWidth;
            // qDebug() << "blockCountY:" << widgetAllBlockHeight;
            resize(blockSize * widgetAllBlockWidth, blockSize * widgetAllBlockHeight);
        }

        void setBlockSize(int size)
        {
            blockSize = size;
            setMinimumSize(blockSize * widgetAllBlockWidth, blockSize * widgetAllBlockHeight);
        }

        int getBlockSize()
        {
            return blockSize;
        }

        void setSpeed(int speed)
        {
            this->speed = speed;
            timer->setInterval(speed);
        }

        int getSpeed()
        {
            return speed;
        }

        int getWidgetAllBlockWidth()
        {
            return widgetAllBlockWidth;
        }

        int getWidgetAllBlockHeight()
        {
            return widgetAllBlockHeight;
        }

        void paintEvent(QPaintEvent *event)
        {
            QPainter painter(this);
            // 设置背景色
            painter.setBrush(Qt::black);
            painter.drawRect(0, 0, width(), height());
            if (gameStatus)
            {
                // 画蛇头
                painter.setBrush(Qt::yellow);
                painter.drawRect(snakeHead->x() * blockSize,
                                 snakeHead->y() * blockSize,
                                 blockSize,
                                 blockSize);
                // qDebug() << snakeHead->x() << snakeHead->y();

                // 画食物
                painter.setBrush(Qt::red);
                painter.drawRect(food->x() * blockSize,
                                 food->y() * blockSize,
                                 blockSize,
                                 blockSize);

                // 画蛇身
                painter.setBrush(Qt::green);
                for (int i = 0; i < snake.size(); i++)
                {
                    QPoint *point = snake.at(i);
                    painter.drawRect(point->x() * blockSize,
                                     point->y() * blockSize,
                                     blockSize,
                                     blockSize);
                }
            }
            else
            {
            }
        }

        void setSnakeDirection(QString dir)
        {
            if (direction == "u" && dir == "d")
            {
                return;
            }
            else if (direction == "d" && dir == "u")
            {
                return;
            }
            else if (direction == "l" && dir == "r")
            {
                return;
            }
            else if (direction == "r" && dir == "l")
            {
                return;
            }
            direction = dir;
        }

        void resizeEvent(QResizeEvent *event)
        {
            if (!gameStatus)
            {
                int X = width() / blockSize;
                int Y = height() / blockSize;
                setWidgetSize(X, Y);
            }
        }

        bool gameStatus = false;

    signals:
        void gameOver(int mode);
        void getScore(int score);

    private:
        QTimer *timer;
        int blockSize = 20;
        int widgetAllBlockWidth = 10;
        int widgetAllBlockHeight = 10;
        QList<QPoint *> snake;
        QPoint *snakeHead;
        QString direction = "n";
        QString preDirection = "n";
        QPoint *food;
        int score = 0;
        int speed = 100;
};
