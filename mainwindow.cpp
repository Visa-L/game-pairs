/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#include "mainwindow.hh"

MainWindow::MainWindow(GameMenu& gameMenu, GameBoard& scene, QWidget *parent)
    : QMainWindow(parent),
      view_(this),
      gameMenu_(gameMenu),
      scene_(scene)
{
    view_.setScene(&gameMenu_);
    setCentralWidget(&view_);
}

void MainWindow::changeScene()
{
    if(gameMenu_.isActive()) {
        view_.setScene(&scene_);
    } else {
        view_.setScene(&gameMenu_);
    }
}
