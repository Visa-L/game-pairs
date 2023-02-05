/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#include "card.hh"
#include "gameengine.hh"

Card::Card(int x, int y, int ltrNbr, GameEngine* engine)
    : x_(x),
      y_(y),
      letterNumber_(ltrNbr),
      letter_(),
      visibility_(),
      engine_(engine)
{
    initializeCard();
}

void Card::initializeCard()
{
    // Initialize letter shown in game board
    letter_ = letters.at(letterNumber_);
    visibility_ = HIDDEN;

    setStyleSheet("background-color: rgb(200, 0, 255);");
    QObject::connect(this, &QPushButton::clicked, this, [this]{showCard();});
}

int Card::getLetterNumber()
{
    return letterNumber_;
}

int Card::getX()
{
    return x_;
}

int Card::getY()
{
    return y_;
}

// Hide visible card and change it's color
void Card::hideCard()
{
    visibility_ = HIDDEN;
    setText("");
    setStyleSheet("background-color: rgb(200, 0, 255);");
}

// Turn card open if it is hidden and no more that 1 card is already
// open, tells game engine if card was turned and it's coordinates
void Card::showCard()
{
    if(visibility_ == OPEN) {
        return;
    }

    bool turnAllowed = engine_->cardTurnAllowed();
    if(visibility_ == HIDDEN and turnAllowed) {
        visibility_ = OPEN;
        setText(letter_);
        setStyleSheet("background-color: rgb(255, 255, 255);");
    }

    engine_->turnCard(x_, y_);
}
