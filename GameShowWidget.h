
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
        GameShowWidget(QWidget *parent = nullptr);

        ~GameShowWidget() = default;

        void changeGameMode(int mode = 0);
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;

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
