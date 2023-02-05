/* Pairs
 *
 * Description:
 *  This program generates a pairs (memory) game. The game has a variant
 * number of cards and players. At the beginning, the program opens main
 * menu, where program asks for amount of players and cards. Game starts
 * by clicking button "Start".
 *  On each round, the player in turn can click a card to turn it visible.
 * Only 2 cards can be visible at the same time and a card cannot be turned
 * hidden until turn has ended. Turn ends when user clicks the button "CLOSE",
 * which hide visible cards if they wasn't a pair. If they are pairs, they
 * are removed from the game board and the score of the player is increased,
 * and a new turn is given for the player.
 *  Program displays time used in current game and scoreboaed of pairs
 * collected by each player. Also player in turn is shown in top of the window.
 *  User can start a new game with same amount of players and cards by
 * clicking button "Restart". The order of cards will be suffled.
 *  The cards are described as letters, starting from A and lasting so far
 * as there are cards. In the game board, a visible card is shown as its
 * letter, a hidden one as empty box, and a removed one as a empty slot.
 *  Game will end when all pairs have been found, and the game board is
 * empty. The program tells who has/have won, i.e. collected most pairs. At
 * any point of game user can also end game by clicking "Quit" button, which
 * exit the program, or click "Main menu" to return main menu.
 *
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 *
 * Notes:
 * Detailed description in text file "instructions.txt"
 *
 */

#include "mainwindow.hh"
#include "gameboard.hh"
#include "gameengine.hh"
#include "gamemenu.hh"

#include <QApplication>
#include <QObject>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameMenu gameMenu;
    GameBoard graphicalGameBoard;
    GameEngine engine(gameMenu, graphicalGameBoard);

    MainWindow w(gameMenu, graphicalGameBoard);

    QObject::connect(&engine, SIGNAL(quitGame()), &w, SLOT(close()));
    QObject::connect(&engine, SIGNAL(changeScene()), &w, SLOT(changeScene()));

    w.show();
    return a.exec();
}
