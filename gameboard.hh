/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "card.hh"

#include <QGraphicsScene>
#include <QLCDNumber>
#include <QWidget>
#include <memory>

class GameBoard : public QGraphicsScene
{
    Q_OBJECT
public:
    GameBoard(QObject* parent = nullptr);

    // Add labels and buttons to graphical gameboart at given position
    void showWidgets(int x, int y, int width, int height, std::shared_ptr<QWidget> widget);

    // Add a new card graphics to given coordinates
    void displayCard(int x, int y, int w, int h, std::shared_ptr<Card> card);
};

#endif // GAMEBOARD_HH
