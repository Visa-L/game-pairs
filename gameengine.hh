/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include "gameboard.hh"
#include "gamemenu.hh"
#include "card.hh"

#include <QObject>
#include <QSpinBox>
#include <QLCDNumber>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>

#include <vector>
#include <memory>
#include <random>

const int MAX_CARDS = 26;
const int GAME_WINDOW_WIDTH = 800;
const int GAME_WINDOW_HEIGHT = 500;
const int GRID = 20;
const int TOP_BAR_HEIGHT = 80;
const int RIGHT_BAR_WIDTH = 120;
const int SCOREBOARD_HEIGHT = 60;
const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 30;

struct Player{
    QString name_;
    std::shared_ptr<Player> next_;
    int collectedPairs_;
};

class GameEngine : public QObject
{
    Q_OBJECT
public:
    GameEngine(GameMenu& gameMenu, GameBoard& gameBoard, int seedValue = 0);

    // Initialize widgets needed to initialize once per run
    void initialize();

    // Initialize single game related cards and widgets
    void initializeGame();

    // Remove found pair from the gameboard
    void removePair();

    // Check how many cards with given value gameboard already has and
    // return amount in question. Numbers are used to get letter from a vector
    int givenCardsInBoard(int letterNumber);

    // Return true if less than 2 cards are visible and
    // so user can turn a new card. Otherwise return false
    bool cardTurnAllowed();

    // Check if board is empty and therefore game is over
    bool isGameOver();

    // Handle winner announcement and ask if user want to play again
    void gameOver();

signals:
    // Exit program
    void quitGame();

    // Change scene between main menu and gameboard as game engine order
    void changeScene();

public slots:
    // When active scene is main menu, change scene to gameboard and
    // starts a game
    void startGame();

    // Start game with new cards and their order
    void restart();

    // Delete dynamic memory and emit signal to exit program
    void sendQuitGame();

    // Change scene to main menu
    void goToMenu();

    // Handle display of time in game board
    void onTimerTimeout();

    // Get coordinates of card turned and handle information
    void turnCard(int x, int y);

    // Hide all (2) visible cards in game board
    void hideCards();

private:
    // Pointers to all cards in gameboard
    std::vector<std::vector<std::shared_ptr<Card>>> board_;

    // Draving area showing game status
    GameBoard& graphicalGameBoard_;

    // Show game menu
    GameMenu& gameMenu_;

    // Obvious variables
    int playerAmount_;
    int cardAmount_;

    int rows_;
    int columns_;

    int cardWidth_;
    int cardHeight_;

    // Game time, timer and widget used for display time
    int sec_;
    int min_;
    QTimer* timer_;
    std::shared_ptr<QLCDNumber> lcdTimer_;

    // Show game scoreboard
    std::shared_ptr<QTextEdit> scoreboard_;

    // Data structure for players, player in turn and
    // widget for displaying player in turn
    std::vector<std::shared_ptr<Player>> players_;
    std::shared_ptr<Player> inTurn_;
    std::shared_ptr<QLabel> inTurnLabel_;

    // Contain pointers to cards currently visible
    std::vector<std::shared_ptr<Card>> visibleCards_;

    // Menu and game widgets excluding cards
    std::vector<std::shared_ptr<QWidget>> menuWidgets_;
    std::vector<std::shared_ptr<QSpinBox>> menuSpinBoxes_;
    std::vector<std::shared_ptr<QWidget>> gameWidgets_;

    // Random number generator
    std::default_random_engine generator;
};

#endif // GAMEENGINE_HH
