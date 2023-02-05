/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#ifndef CARD_HH
#define CARD_HH

#include <QPushButton>
#include <QString>
#include <vector>

enum visibilityType {HIDDEN, OPEN};

const std::vector<QString> letters = {"A", "B", "C", "D", "E", "F", "G",
                                   "H", "I", "J", "K", "L", "M", "N",
                                   "O", "P", "Q", "R", "S", "T", "U",
                                   "V", "W", "X", "Y", "Z"};

class GameEngine;

class Card : public QPushButton
{
public:
    Card(int x, int y, int letterNumber, GameEngine* engine);

    // Initializes a new card
    void initializeCard();

    // Get index matching letters vector element for gameboard analyze
    int getLetterNumber();

    // Get coordinates of the card in the gameboard
    int getX();
    int getY();

    // Turn card to be hidden
    void hideCard();

    // Turn card to be open
Q_SLOTS void showCard();

private:
    // Coordinates, letter and number matching the letter of the card
    int x_;
    int y_;

    int letterNumber_;
    QString letter_;

    // Contain information if cards value is visible in the gameboard or not
    visibilityType visibility_;

    // Pointer to the game engine
    GameEngine* engine_;
};

#endif // CARD_HH
