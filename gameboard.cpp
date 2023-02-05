/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#include "gameboard.hh"
#include "gameengine.hh"

#include <QPalette>
#include <QColor>

GameBoard::GameBoard(QObject* parent):
    QGraphicsScene(parent)
{
    setSceneRect(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
    setBackgroundBrush(QColor(120, 0, 0));
}

void GameBoard::showWidgets(int x, int y, int w, int h, std::shared_ptr<QWidget> widget)
{
    addWidget(widget.get());
    widget.get()->setGeometry(x, y, w, h);
}

void GameBoard::displayCard(int x, int y, int w, int h, std::shared_ptr<Card> card)
{
    addWidget(card.get());
    card.get()->setGeometry(GRID + x * (GRID + w),
                      TOP_BAR_HEIGHT + GRID + y * (GRID + h),
                      w, h);
}
