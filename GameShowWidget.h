
#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <qgridlayout.h>

#include "SnakeGameWidget.h"

class GameShowWidget : public QWidget
{
        Q_OBJECT;

    public:
        GameShowWidget(QWidget *parent = nullptr) : QWidget(parent)
        {
            layout = new QVBoxLayout();

            gameTopLayout = new QHBoxLayout();
            gameScoreNameLabel = new QLabel("得分:");
            gameScoreNumLabel = new QLabel("0");
            gameStartGameButton = new QPushButton("开始游戏");
            gameSettingButton = new QPushButton("游戏设置");
            gameHelpButton = new QPushButton("帮助");

            // 取消键盘焦点
            gameStartGameButton->setFocusPolicy(Qt::NoFocus);
            gameSettingButton->setFocusPolicy(Qt::NoFocus);
            gameHelpButton->setFocusPolicy(Qt::NoFocus);

            gameTopLayout->addWidget(gameScoreNameLabel);
            gameTopLayout->addWidget(gameScoreNumLabel);
            gameTopLayout->addStretch();
            gameTopLayout->addWidget(gameStartGameButton);
            gameTopLayout->addWidget(gameSettingButton);
            gameTopLayout->addWidget(gameHelpButton);

            gameArea = new SnakeGameWidget(this);
            layout->addLayout(gameTopLayout);
            layout->addWidget(gameArea);

            speed = gameArea->getSpeed();
            speedAdd = speed - 200;

            connect(gameStartGameButton,
                    &QPushButton::clicked,
                    this,
                    [=]()
                    {
                        QString str = gameStartGameButton->text();
                        if (str == "开始游戏")
                        {
                            changeGameMode(0);
                        }
                        else
                        {
                            changeGameMode(1);
                        }
                    });
            connect(gameHelpButton,
                    &QPushButton::clicked,
                    this,
                    [=]()
                    {
                        QMessageBox::information(
                            this,
                            "帮助",
                            "1.单按方向键控制方向\n"
                            "2.长按方向键变速\n"
                            "3.窗口的大小是蛇的活动空间大小，适当调整窗口大小\n"
                            "4.撞墙和撞到自己都会结束游戏\n"
                            "5.游戏设置可以设置速度");
                    });

            connect(gameSettingButton,
                    &QPushButton::clicked,
                    [=]()
                    {
                        changeGameMode(1);
                        QDialog dialog(this);

                        QGridLayout dialogGridLayout;

                        QLabel dialogBottomLabel("正常速度(1-1000):");
                        QLineEdit dialogBottomLineEdit;
                        dialogBottomLineEdit.setText(QString::number(1000 - gameArea->getSpeed()));
                        dialogGridLayout.addWidget(&dialogBottomLabel, 0, 0);
                        dialogGridLayout.addWidget(&dialogBottomLineEdit, 0, 1);

                        // 按下空格的速度
                        QLabel dialogSpaceLabel("变速速度(1-1000):");
                        QLineEdit dialogSpaceLineEdit;
                        dialogSpaceLineEdit.setText(QString::number(1000 - speedAdd));
                        dialogGridLayout.addWidget(&dialogSpaceLabel, 1, 0);
                        dialogGridLayout.addWidget(&dialogSpaceLineEdit, 1, 1);

                        QPushButton dialogEnsureButton("确定");
                        QPushButton dialogCancelButton("取消");
                        dialogGridLayout.addWidget(&dialogEnsureButton, 2, 0);
                        dialogGridLayout.addWidget(&dialogCancelButton, 2, 1);

                        connect(&dialogEnsureButton,
                                &QPushButton::clicked,
                                [&]()
                                {
                                    bool ok1;
                                    bool ok2;
                                    int speed = dialogBottomLineEdit.text().toInt(&ok1);
                                    int speedAdd = dialogSpaceLineEdit.text().toInt(&ok2);
                                    if (!ok1)
                                    {
                                        QMessageBox::warning(this, "警告", "速度请输入数字");
                                        return;
                                    }
                                    if (!ok2)
                                    {
                                        QMessageBox::warning(this, "警告", "加速速度请输入数字");
                                        return;
                                    }

                                    if (!(speed <= 1000 && speed >= 1))
                                    {
                                        QMessageBox::warning(this, "警告", "速度范围为1-1000");
                                        return;
                                    }
                                    if (!(speedAdd <= 1000 && speedAdd >= 1))
                                    {
                                        QMessageBox::warning(this, "警告", "加速速度范围为1-1000");
                                        return;
                                    }
                                    speed = 1000 - speed;
                                    speedAdd = 1000 - speedAdd;
                                    gameArea->setSpeed(speed);
                                    this->speed = speed;
                                    this->speedAdd = speedAdd;
                                    dialog.close();
                                });

                        connect(&dialogCancelButton,
                                &QPushButton::clicked,
                                [&]()
                                {
                                    dialog.close();
                                });

                        dialog.setLayout(&dialogGridLayout);

                        dialog.exec();
                    });

            connect(gameArea,
                    &SnakeGameWidget::gameOver,
                    this,
                    [=]()
                    {
                        changeGameMode(1);
                    });
            connect(gameArea,
                    &SnakeGameWidget::getScore,
                    [=](int score)
                    {
                        gameScoreNumLabel->setText(QString::number(score));
                    });

            setLayout(layout);
        }

        ~GameShowWidget()
        {
        }

        void changeGameMode(int mode = 0)
        {
            // qDebug() << "chageGameMode";
            QString str = gameStartGameButton->text();
            // qDebug() << str;
            if (mode == 0)
            {
                if (str == "开始游戏")
                {
                    gameStartGameButton->setText("结束游戏");
                    gameArea->setSpeed(speed);
                    gameArea->start();
                    gameScoreNumLabel->setText("0");
                }
            }
            else if (mode == 1)
            {
                if (str == "结束游戏")
                {
                    // qDebug() << gameStartGameButton->text();
                    gameStartGameButton->setText("开始游戏");
                    gameArea->end();
                    gameArea->setSpeed(speed);

                    // 吃完全部食物
                    QString score = gameScoreNumLabel->text();
                    if (score.toInt()
                        == gameArea->getWidgetAllBlockWidth() * gameArea->getWidgetAllBlockHeight()
                               - 1)
                    {
                        QMessageBox::warning(this,
                                             "游戏结束",
                                             "恭喜你吃完全部食物!",
                                             QMessageBox::Ok);
                    }
                    else
                    {
                        QMessageBox::warning(this,
                                             "游戏结束",
                                             "得分:" + gameScoreNumLabel->text(),
                                             QMessageBox::Ok);
                    }
                    gameScoreNumLabel->setText("0");
                }
            }
        }

        void keyPressEvent(QKeyEvent *event)
        {
            if (event->key() == Qt::Key_Up)
            {
                gameArea->setSnakeDirection("u");
            }
            else if (event->key() == Qt::Key_Down)
            {
                gameArea->setSnakeDirection("d");
            }
            else if (event->key() == Qt::Key_Left)
            {
                gameArea->setSnakeDirection("l");
            }
            else if (event->key() == Qt::Key_Right)
            {
                gameArea->setSnakeDirection("r");
            }

            // 变速
            if (!event->isAutoRepeat())
            {
                QString direction = gameArea->getSnakeDirection();
                if (direction == "u" && event->key() == Qt::Key_Up)
                {
                    gameArea->setSpeed(speedAdd);
                }
                else if (direction == "d" && event->key() == Qt::Key_Down)
                {
                    gameArea->setSpeed(speedAdd);
                }
                else if (direction == "l" && event->key() == Qt::Key_Left)
                {
                    gameArea->setSpeed(speedAdd);
                }
                else if (direction == "r" && event->key() == Qt::Key_Right)
                {
                    gameArea->setSpeed(speedAdd);
                }
            }
        }

        void keyReleaseEvent(QKeyEvent *event)
        {
            // 恢复速度
            if (!event->isAutoRepeat())
            {
                QString direction = gameArea->getSnakeDirection();
                if ((direction == "u" || direction == "d") && event->key() == Qt::Key_Up)
                {
                    gameArea->setSpeed(speed);
                }
                else if ((direction == "u" || direction == "d") && event->key() == Qt::Key_Down)
                {
                    gameArea->setSpeed(speed);
                }
                else if ((direction == "l" || direction == "r") && event->key() == Qt::Key_Left)
                {
                    gameArea->setSpeed(speed);
                }
                else if ((direction == "l" || direction == "r") && event->key() == Qt::Key_Right)
                {
                    gameArea->setSpeed(speed);
                }
            }
        }

    private:
        QVBoxLayout *layout;
        QHBoxLayout *gameTopLayout;
        QLabel *gameScoreNameLabel;
        QLabel *gameScoreNumLabel;
        QPushButton *gameStartGameButton;
        QPushButton *gameSettingButton;
        SnakeGameWidget *gameArea;
        QPushButton *gameHelpButton;

        int speed;
        int speedAdd;
};
