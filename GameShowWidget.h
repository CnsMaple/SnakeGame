
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
                            "按方向键控制蛇的移动，吃到食物得分，撞到墙壁或者自己结束游戏\n游戏开始"
                            "前请调整合适的窗口大小，游戏布局随窗口大小变化\n游戏设置可以设置速度大小");
                    });

            connect(gameSettingButton,
                    &QPushButton::clicked,
                    [=]()
                    {
                        changeGameMode(1);
                        QDialog dialog(this);

                        QGridLayout dialogGridLayout;

                        QLabel dialogBottomLabel("游戏速度(1-1000):");
                        QLineEdit dialogBottomLineEdit;
                        dialogBottomLineEdit.setText(QString::number(gameArea->getSpeed()));
                        dialogGridLayout.addWidget(&dialogBottomLabel, 0, 0);
                        dialogGridLayout.addWidget(&dialogBottomLineEdit, 0, 1);

                        QPushButton dialogEnsureButton("确定");
                        QPushButton dialogCancelButton("取消");
                        dialogGridLayout.addWidget(&dialogEnsureButton, 1, 0);
                        dialogGridLayout.addWidget(&dialogCancelButton, 1, 1);

                        connect(&dialogEnsureButton,
                                &QPushButton::clicked,
                                [&]()
                                {
                                    bool ok;
                                    int speed = dialogBottomLineEdit.text().toInt(&ok);
                                    if (!ok)
                                    {
                                        QMessageBox::warning(this, "警告", "速度请输入数字");
                                        return;
                                    }

                                    if (!(speed <= 1000 && speed >= 1))
                                    {
                                        QMessageBox::warning(this, "警告", "速度范围为1-1000");
                                        return;
                                    }
                                    gameArea->setSpeed(speed);
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
                }
            }
        }

        // 键盘监听上下左右
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
};
