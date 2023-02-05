/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#ifndef GAMEMENU_HH
#define GAMEMENU_HH

#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <memory>
#include <vector>

class GameMenu : public QGraphicsScene
{
    Q_OBJECT
public:
    GameMenu(QObject* parent = nullptr);

    // Initialize button in main menu to given coordinates.
    // Button size is in gameengine's constant
    void buttonLayout(int x, int y, std::shared_ptr<QPushButton> button);

    // Initialize main menu widget to given coordinates as given size
    void widgetLayout(int x, int y, int width, int height, std::shared_ptr<QWidget> widget);
};

#endif // GAMEMENU_HH
