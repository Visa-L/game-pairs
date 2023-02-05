/*
 * Program author:
 * Name: Visa Laakso
 * E-Mail: visa.laakso@tuni.fi
 */

#include "gameengine.hh"

#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QTime>
#include <QMessageBox>

GameEngine::GameEngine(GameMenu& gameMenu, GameBoard& gameBoard, int seedValue)
    : QObject(0),
      board_(),
      graphicalGameBoard_(gameBoard),
      gameMenu_(gameMenu),
      playerAmount_(),
      cardAmount_(),
      rows_(),
      columns_(),
      cardWidth_(),
      cardHeight_(),
      sec_(),
      min_(),
      timer_(),
      lcdTimer_(),
      scoreboard_(),
      players_(),
      inTurn_(),
      inTurnLabel_(),
      visibleCards_(),
      menuWidgets_(),
      menuSpinBoxes_(),
      gameWidgets_()
{
    // Random number generator's seed value from computer's current time
    if(seedValue == 0) {
        generator.seed(time(NULL));
    } else {
        generator.seed(seedValue);
    }

    initialize();
    initializeGame();
}

void GameEngine::initialize()
{
    // Initialize menu buttons
    std::shared_ptr<QPushButton> newButton =
            std::make_shared<QPushButton>("Start");
    QObject::connect(newButton.get(),
                     SIGNAL(clicked()),
                     this,
                     SLOT(startGame()));
    gameMenu_.widgetLayout((GAME_WINDOW_WIDTH - BUTTON_WIDTH) / 2,
                           GAME_WINDOW_HEIGHT / 2,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT,
                           newButton);
    menuWidgets_.push_back(newButton);

    newButton = std::make_shared<QPushButton>("Quit");
    QObject::connect(newButton.get(),
                     SIGNAL(clicked()),
                     this,
                     SLOT(sendQuitGame()));
    gameMenu_.widgetLayout((GAME_WINDOW_WIDTH - BUTTON_WIDTH) / 2,
                           GAME_WINDOW_HEIGHT / 2 + GRID + BUTTON_HEIGHT,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT,
                           newButton);
    menuWidgets_.push_back(newButton);

    // Initialize menu title and labels
    std::shared_ptr<QLabel> newLabel =
            std::make_shared<QLabel>("PAIRS MEMORY GAME");
    QFont font = newLabel.get()->font();
    font.setPointSize(30);
    font.setBold(true);
    newLabel.get()->setFont(font);
    newLabel.get()->setStyleSheet("QLabel {color : red;}");
    newLabel.get()->setAttribute(Qt::WA_NoSystemBackground);
    gameMenu_.widgetLayout((GAME_WINDOW_WIDTH - 4 * BUTTON_WIDTH) / 2,
                           GRID,
                           5 * BUTTON_WIDTH,
                           2 * BUTTON_HEIGHT,
                           newLabel);
    gameWidgets_.push_back(newLabel);

    newLabel = std::make_shared<QLabel>("Players:");
    gameMenu_.widgetLayout(GAME_WINDOW_WIDTH / 2 - BUTTON_WIDTH,
                           GAME_WINDOW_HEIGHT / 4,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT,
                           newLabel);
    menuWidgets_.push_back(newLabel);

    newLabel = std::make_shared<QLabel>("Cards:");
    gameMenu_.widgetLayout(GAME_WINDOW_WIDTH / 2 - BUTTON_WIDTH,
                           GAME_WINDOW_HEIGHT / 4 + GRID + BUTTON_HEIGHT,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT,
                           newLabel);
    menuWidgets_.push_back(newLabel);

    // Initialize menu spin boxes
    std::shared_ptr<QSpinBox> newSpinBox = std::make_shared<QSpinBox>();
    newSpinBox->setValue(2);
    newSpinBox->setMinimum(1);
    newSpinBox->setMaximum(20);
    gameMenu_.widgetLayout(GAME_WINDOW_WIDTH / 2,
                           GAME_WINDOW_HEIGHT / 4,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT,
                           newSpinBox);
    menuSpinBoxes_.push_back(newSpinBox);

    newSpinBox = std::make_shared<QSpinBox>();
    newSpinBox->setValue(20);
    newSpinBox->setMinimum(2);
    newSpinBox->setMaximum(26);
    newSpinBox->setSingleStep(2);
    gameMenu_.widgetLayout(GAME_WINDOW_WIDTH / 2,
                           GAME_WINDOW_HEIGHT / 4 + GRID + BUTTON_HEIGHT,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT,
                           newSpinBox);
    menuSpinBoxes_.push_back(newSpinBox);

    // Initialize buttons for gameboard
    std::shared_ptr<QPushButton> newWidget =
            std::make_shared<QPushButton>("Quit");
    QObject::connect(newWidget.get(),
                     SIGNAL(clicked()),
                     this,
                     SLOT(sendQuitGame()));
    graphicalGameBoard_.showWidgets(GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH,
                                    GAME_WINDOW_HEIGHT - BUTTON_HEIGHT - GRID,
                                    BUTTON_WIDTH,
                                    BUTTON_HEIGHT,
                                    newWidget);
    gameWidgets_.push_back(newWidget);

    newWidget = std::make_shared<QPushButton>("Restart");
    QObject::connect(newWidget.get(),
                     SIGNAL(clicked()),
                     this,
                     SLOT(restart()));
    graphicalGameBoard_.showWidgets(GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH,
                                    GAME_WINDOW_HEIGHT -3*(BUTTON_HEIGHT+GRID),
                                    BUTTON_WIDTH,
                                    BUTTON_HEIGHT,
                                    newWidget);
    gameWidgets_.push_back(newWidget);

    newWidget = std::make_shared<QPushButton>("Main menu");
    QObject::connect(newWidget.get(),
                     SIGNAL(clicked()),
                     this,
                     SLOT(goToMenu()));
    graphicalGameBoard_.showWidgets(GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH,
                                    GAME_WINDOW_HEIGHT -2*(BUTTON_HEIGHT+GRID),
                                    BUTTON_WIDTH,
                                    BUTTON_HEIGHT,
                                    newWidget);
    gameWidgets_.push_back(newWidget);

    newWidget = std::make_shared<QPushButton>("CLOSE");
    newWidget->setDisabled(true);
    QObject::connect(newWidget.get(),
                     SIGNAL(clicked()),
                     this,
                     SLOT(hideCards()));
    graphicalGameBoard_.showWidgets(GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH,
                                    GAME_WINDOW_HEIGHT -5*(BUTTON_HEIGHT+GRID),
                                    BUTTON_WIDTH,
                                    BUTTON_HEIGHT,
                                    newWidget);
    gameWidgets_.push_back(newWidget);

    // Initialize labels for game board
    newLabel = std::make_shared<QLabel>("Time:");
    graphicalGameBoard_.showWidgets(GRID,
                                    GRID,
                                    BUTTON_WIDTH,
                                    BUTTON_HEIGHT,
                                    newLabel);
    gameWidgets_.push_back(newLabel);

    newLabel = std::make_shared<QLabel>("In turn:");
    graphicalGameBoard_.showWidgets(GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH -
                                    BUTTON_WIDTH - GRID,
                                    GRID,
                                    BUTTON_WIDTH, BUTTON_HEIGHT, newLabel);
    gameWidgets_.push_back(newLabel);

    // Initialize active widgets for game board
    inTurnLabel_ = std::make_shared<QLabel>("Player1");
    graphicalGameBoard_.showWidgets
            (GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH - BUTTON_WIDTH - GRID,
             GRID + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, inTurnLabel_);
    gameWidgets_.push_back(inTurnLabel_);

    timer_ = new QTimer();

    lcdTimer_ = std::make_shared<QLCDNumber>();
    lcdTimer_->display("00:00");
    QObject::connect(timer_, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    graphicalGameBoard_.showWidgets(GRID,
                                    GRID + BUTTON_HEIGHT,
                                    BUTTON_WIDTH,
                                    BUTTON_HEIGHT,
                                    lcdTimer_);
    gameWidgets_.push_back(lcdTimer_);

    scoreboard_ = std::make_shared<QTextEdit>();
    scoreboard_.get()->setReadOnly(true);
    graphicalGameBoard_.showWidgets(GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH,
                                    GRID,
                                    RIGHT_BAR_WIDTH - GRID,
                                    GRID + 2 * GRID,
                                    scoreboard_);
    gameWidgets_.push_back(scoreboard_);
}

// Initialize single game related cards and data
void GameEngine::initializeGame()
{
    visibleCards_.clear();
    players_.clear();
    playerAmount_ = 0;
    cardAmount_ = 0;

    // Get amount of players and cards
    cardAmount_ = menuSpinBoxes_.at(1).get()->value();
    playerAmount_ = menuSpinBoxes_.at(0).get()->value();
    if(cardAmount_ % 2 != 0) {
        --cardAmount_;
    }

    // Initialize timer
    sec_ = 0;
    min_ = 0;
    lcdTimer_->display("00:00");
    timer_->start(1000);

    // Initialize players
    for(int i = 1; i <= playerAmount_; ++i) {
        std::shared_ptr<Player> newPlayer = std::make_shared<Player>();
        newPlayer->name_ = QString("Player") + QString::number(i);
        newPlayer->collectedPairs_ = 0;
        newPlayer->next_ = nullptr;
        if(i != 1) {
            players_.back().get()->next_ = newPlayer;
        }

        players_.push_back(newPlayer);
    }

    inTurn_ = players_.at(0);
    inTurnLabel_->setText(inTurn_.get()->name_);

    // Initialize scoreboard height according to amount of players
    int h = SCOREBOARD_HEIGHT + playerAmount_ * GRID * 2;
    if(h > GAME_WINDOW_HEIGHT - 5 * (GRID + BUTTON_HEIGHT) - GRID) {
        h = GAME_WINDOW_HEIGHT - 5 * (GRID + BUTTON_HEIGHT) - GRID;
    }

    scoreboard_.get()->setFixedHeight(h);

    // Initialize scoreboard content
    scoreboard_.get()->setText("Scoreboard:\n");
    for(int i = 0; i < playerAmount_; ++i) {
        QString name = players_.at(i).get()->name_;
        scoreboard_.get()->append(name + ": 0");
    }

    board_.clear();

    if(cardAmount_ > MAX_CARDS) {
                cardAmount_ = MAX_CARDS;
    }

    // Get closest factors of card amount
    for(int i = 1; i * i <= cardAmount_; ++i) {
        if(cardAmount_ % i == 0) {
            rows_ = i;
        }
    }
    columns_ = cardAmount_ / rows_;

    // Get width and height of single card so cards size scale to game board
    cardWidth_ =(GAME_WINDOW_WIDTH - RIGHT_BAR_WIDTH - GRID - columns_ * GRID)
            / columns_;
    cardHeight_ = (GAME_WINDOW_HEIGHT - TOP_BAR_HEIGHT - GRID - rows_ * GRID)
            / rows_;

    // Initialize empty game board
    board_.clear();

    for(int y = 0; y < rows_; ++y) {
        std::vector<std::shared_ptr<Card>> rowVector;

        for(int x = 0; x < columns_; ++x) {
            std::shared_ptr<Card> emptyPlace = nullptr;
            rowVector.push_back(emptyPlace);
        }
        board_.push_back(rowVector);
    }

    // Create new cards and initialize game board with new cards
    for(int y = 0; y < rows_; ++y) {
        for(int x = 0; x < columns_; ++x) {
            std::uniform_int_distribution<int>
                    distribution(0, cardAmount_ / 2 - 1);

            // Draw one number unnecessarily to avoid first number being
            // lower limit of distribution
            int letterNumber = distribution(generator);
            letterNumber = distribution(generator);
            do {
                --letterNumber;
                if(letterNumber < 0) {
                    letterNumber = cardAmount_ / 2 - 1;
                }
            }
            while(givenCardsInBoard(letterNumber) >= 2);

            std::shared_ptr<Card> newCard =
                    std::make_shared<Card>(x, y, letterNumber, this);

            graphicalGameBoard_.displayCard
                    (x, y, cardWidth_, cardHeight_, newCard);

            board_.at(y).at(x) = newCard;
        }
    }
}

// Remove found pair from game board, add point to player in turn
// and give another turn player in question. Also refresh scoreboard.
void GameEngine::removePair()
{
    ++inTurn_->collectedPairs_;
    board_.at(visibleCards_.at(0).get()->getY()).at
            (visibleCards_.at(0).get()->getX()) = nullptr;

    board_.at(visibleCards_.at(1).get()->getY()).at
            (visibleCards_.at(1).get()->getX()) = nullptr;

    QString str = inTurn_.get()->name_.at(inTurn_.get()->name_.size() - 1);
    int i = str.toInt() - 2;
    if(i < 0) {
        i = playerAmount_ - 1;
    }

    inTurn_ = players_.at(i);

    scoreboard_->clear();
    scoreboard_.get()->setText("Scoreboard:");
    for(int i = 0; i < playerAmount_; ++i) {
        QString name = players_.at(i).get()->name_;
        scoreboard_.get()->append("\n" +
                                  name +
                                  ": " +
                                  QString::number
                                  (players_.at(i).get()->collectedPairs_));
    }
}

// Return amount of cards with given letter. Letter is pointed by number
int GameEngine::givenCardsInBoard(int letterNumber)
{
    int amount = 0;
    for(int y = 0; y < rows_; ++y) {
        for(int x = 0; x < columns_; ++x) {
            if(board_.at(y).at(x).get() != nullptr and
                    board_.at(y).at(x).get()->getLetterNumber() ==
                    letterNumber) {
                ++amount;
            }
        }
    }
    return amount;
}

// Return boolean if user is allowed to turn new card.
bool GameEngine::cardTurnAllowed()
{
    return visibleCards_.size() < 2;
}

// Check if game board is empty i.e. game is over
bool GameEngine::isGameOver()
{
    for(int y = 0; y < rows_; ++y) {
        for(int x = 0; x < columns_; ++x) {
            if(board_.at(y).at(x) != nullptr) {
                return false;
            }
        }
    }
    return true;
}

// Handle winner(s) announcement and ask if user want to play again
void GameEngine::gameOver()
{
    timer_->stop();
    std::shared_ptr<Player> winner = players_.at(0);
    std::vector<std::shared_ptr<Player>> multipleWinners = {};

    for(int i = 1; i < playerAmount_; ++i) {
        int score = players_.at(i).get()->collectedPairs_;

        if(winner != nullptr and score > winner.get()->collectedPairs_) {
            winner = players_.at(i);

        } else if(winner != nullptr and
                  score == winner.get()->collectedPairs_) {
            multipleWinners.push_back(winner);
            multipleWinners.push_back(players_.at(i));
            winner = nullptr;

        } else if(winner == nullptr and
                  score == multipleWinners.at(0).get()->collectedPairs_) {
            multipleWinners.push_back(players_.at(i));

        } else if(winner == nullptr and
                  score > multipleWinners.at(0).get()->collectedPairs_) {
            multipleWinners.clear();
            winner = players_.at(i);
        }
    }

    int messageBoxResult = QMessageBox::No;
    if(winner != nullptr) {
        QString text = QString("Winner is ") +
                winner.get()->name_ + QString(" with ") +
                QString::number(winner.get()->collectedPairs_) +
                QString(" pairs collected. Play again?");

        messageBoxResult = QMessageBox::question
                (0, "GAME OVER!", text, QMessageBox::Yes, QMessageBox::No);

    } else {
        QString winners = {multipleWinners.at(0).get()->name_};
        for(int i = 1; i < int(multipleWinners.size()); ++i) {
            winners += " and ";
            winners += multipleWinners.at(i).get()->name_;
        }

        QString text = QString("Tie between ") + winners +
                QString(" with ") +
                QString::number(multipleWinners.at(0).get()->collectedPairs_) +
                QString(" pairs collected. Play again?");

        messageBoxResult = QMessageBox::question
                (0,  "GAME OVER!", text, QMessageBox::Yes, QMessageBox::No);
    }

    if(messageBoxResult == QMessageBox::Yes) {
        restart();

    } else {
        sendQuitGame();
    }
}

void GameEngine::startGame()
{
    emit changeScene();
    initializeGame();
}

void GameEngine::restart()
{
    initializeGame();
}

void GameEngine::sendQuitGame()
{
    delete timer_;
    emit quitGame();
}

void GameEngine::goToMenu()
{
    emit changeScene();
}

// Handle timer functioning and display of time in game board
void GameEngine::onTimerTimeout()
{
    ++sec_;
    if (sec_ >= 60) {
        sec_ = 0;
        ++min_;
    }

    QString min = QString::number(min_);
    QString sec = QString::number(sec_);

    if (min.size() == 1) {
        min = "0" + min;
    }

    if (sec.size() == 1) {
        sec = "0" + sec;
    }

    QString num = min + ":" + sec;
    lcdTimer_->display(num);

}

void GameEngine::turnCard(int x, int y)
{
    if(not cardTurnAllowed()) {
        return;
    }

    std::shared_ptr<Card> card = board_.at(y).at(x);
    visibleCards_.push_back(card);

    // Check if 2 cards are visible and if their values(letters) are same
    if(not cardTurnAllowed() and
            visibleCards_.at(0).get()->getLetterNumber() ==
            visibleCards_.at(1).get()->getLetterNumber()) {
        removePair();
        if(isGameOver()) {
            gameOver();
        }
    }

    // Set CLOSE button enabled if 2 cards are visible
    if(not cardTurnAllowed()) {
        gameWidgets_.at(4)->setEnabled(true);
    }
}

void GameEngine::hideCards()
{
    for(std::shared_ptr<Card> card : visibleCards_) {
        card->hideCard();
    }
    visibleCards_.clear();

    // Set CLOSE button disabled
    gameWidgets_.at(4)->setDisabled(true);

    inTurn_ = inTurn_->next_;
    if(inTurn_ == nullptr) {
        inTurn_ = players_.at(0);
    }

    inTurnLabel_->setText(inTurn_->name_);
}
