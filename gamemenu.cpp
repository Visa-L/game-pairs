/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#include "gamemenu.hh"
#include "gameengine.hh"

GameMenu::GameMenu(QObject* parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
    setBackgroundBrush(QColor(0, 50, 0));
}

void GameMenu::buttonLayout(int x, int y, std::shared_ptr<QPushButton> button)
{
    addWidget(button.get());
    button->setGeometry(x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
}

void GameMenu::widgetLayout(int x, int y, int w, int h, std::shared_ptr<QWidget> widget)
{
    addWidget(widget.get());
    widget->setGeometry(x, y, w, h);
}
