/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"
#include "gamemenu.hh"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(GameMenu& gameMenu, GameBoard& scene, QWidget *parent = nullptr);

public slots:
    // Change scene between main menu and game board
    // when game engine order so
    void changeScene();

private:
    // Program view
    QGraphicsView view_;

    // Scenes main menu and game board
    GameMenu& gameMenu_;
    GameBoard& scene_;
};
#endif // MAINWINDOW_HH
