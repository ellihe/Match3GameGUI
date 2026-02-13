/* Match3 (luotu 2020)
 *
 * Kuvaus:
 *  Ohjelma toteuttaa Match3 - hedelmäpelin. Pelilauta on kooltaan ROWS x COLUMNS
 * (5x4), jota voi muokata luokan määrittelytiedostossa. Pelissä on tarkoitus
 * saada niin sanottuja matcheja site, että pelaaja saa siirrettyä hedelmiä niin, että
 * niitä on vähintään kolme vierekkäin tai päällekkäin. Tässä versiossa hedelmien
 * paikalla toimiii värikkäät neliöt.
 *
 * Pelaaja voi käyttöliittymässä valita vaihdettavat neliöt syöttämällä rivin
 * ja sarakkeen numerot. Kun kolme tai enemmän hedelmää päätyy vierekkäin tai päällekkäin,
 * hedelmät poistuvat ja tyhjät kohdat täyttyvät yläpuolella olevilla hedelmillä.
 *
 * Ohjelman kirjoittaja
 * Nimi: EH

 * Palautteen kieli (fi/en): fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 * Ohjelman tarkempi toteutusratkaisu ja käyttöohje on erillisenä tiedostona
 *
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>
#include <QPixmap>
#include <vector>
#include <string>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    // We need a graphics scene in which to draw rectangles
    scene_ = new QGraphicsScene(this);

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui_->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a fruit is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);
    int seed = time(0); // You can change seed value for testing purposes
    randomEng_.seed(seed);
    distr_ = std::uniform_int_distribution<int>(0, NUMBER_OF_FRUITS - 1);
    distr_(randomEng_); // Wiping out the first random number (which is almost always 0)

    initTitles();

    initializeBoard();

    coloreBoard();

    ui_->goPushButton->setStyleSheet("background-color: green");
    ui_->goPushButton->setEnabled(false);

    on_directionPushButton_clicked();

    ui_->noDelayButton->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

/* Initializes board vektor board_
 * No parameters or return value
 */
void MainWindow::initializeBoard()
{
    //Initializes the board vector
    for ( int col = 0; col < COLUMNS; ++col ) {
        board_.push_back({});
        for (int row = ROWS-1; row >= 0; --row) {
            QBrush redBrush(Qt::white);
            QPen blackPen(Qt::black);
            square_ = scene_->addRect(SQUARE_SIDE * col,SQUARE_SIDE * row, SQUARE_SIDE, SQUARE_SIDE, blackPen, redBrush);
            board_.at(col).push_back(square_);
        }
    }
}

/* Colores the scene and updates board_ vector also
 * No parameters or return value
 */
void MainWindow::coloreBoard()
{
    //Initializes the board with colored squares
    for ( int col = 0; col < COLUMNS; ++col ) {
        for (int row = ROWS-1; row >= 0; --row) {
            coloreSquare(col, row);
        }
    }
}

/* Colores one square with random color from Fruit_colors_
 * Parameter:
 *    int col, the column of square
 *    int row, the row of square
 * No return values
 */
void MainWindow::coloreSquare(int col, int row) {

    QGraphicsRectItem* up = board_.at(col).at(row);
    QGraphicsRectItem* down = board_.at(col).at(row);
    QGraphicsRectItem* left = board_.at(col).at(row);
    QGraphicsRectItem* right = board_.at(col).at(row);

    //Checks up-down - direction and sets items
    if (scene_->sceneRect().contains(SQUARE_SIDE * col, SQUARE_SIDE * (row-2))) {
        down = board_.at(col).at(row-2);
    }

    if (scene_->sceneRect().contains(SQUARE_SIDE * col, SQUARE_SIDE * (row+2))) {
        up = board_.at(col).at(row+2);
    }

    //Checks right-left - direction and sets items
    if (scene_->sceneRect().contains(SQUARE_SIDE * (col-2), SQUARE_SIDE * row)){
         left = board_.at(col-2).at(row);
    }

    if (scene_->sceneRect().contains(SQUARE_SIDE * (col+2), SQUARE_SIDE * row)) {
         right = board_.at(col+2).at(row);
    }

    // Init random color
    QBrush wantedBrush(Fruit_colors_.at(distr_(randomEng_)));

    // Checks if there would be three same colored squares side by side or
    // one on the other and changes square's color if so
    while (wantedBrush == up->brush() or wantedBrush == down->brush()
           or wantedBrush == left->brush() or wantedBrush == right->brush()) {
           wantedBrush = Fruit_colors_.at(distr_(randomEng_));
    }

    // Sets brush to square
    board_.at(col).at(row)->setBrush(wantedBrush);
}

/* Initializes titles for columns and rows
 * No parameters or return value
 */
void MainWindow::initTitles()
{
    // Displaying column titles, starting from A
    for(int i = 0, letter = '0'; i < COLUMNS; ++i, ++letter)
    {
        int shift = 5;
        QString letter_string = "";
        letter_string.append(letter);
        QLabel* label = new QLabel(letter_string, this);
        label->setGeometry(LEFT_MARGIN + shift + i * SQUARE_SIDE,
                           TOP_MARGIN - SQUARE_SIDE,
                           SQUARE_SIDE, SQUARE_SIDE);
    }

    // Displaying row titles, starting from A
    for(int i = 0, letter = '0'; i < ROWS; ++i, ++letter)
    {
        QString letter_string = "";
        letter_string.append(letter);
        QLabel* label = new QLabel(letter_string, this);
        label->setGeometry(LEFT_MARGIN - SQUARE_SIDE,
                           TOP_MARGIN + i * SQUARE_SIDE,
                           SQUARE_SIDE, SQUARE_SIDE);
    }
}

/* Checks if there is equal or more than three same colored squares in
 * horizontal lines
 *  No paramers or return values
 */
void MainWindow::checkLinesHorizontally()
{
    // Checks if there is match at one loop
    bool is_found = false;
    // Keeps track of how many same colore found side by side
    int how_many_same = 0;

    for ( int col = 0; col < COLUMNS; ++col ) {
        for (int row = ROWS-1; row >= 0; --row) {
            how_many_same = 0;

                // Goes to find match only if these terms come true
                if ( col != COLUMNS -1 and col != COLUMNS - 2
                     and board_.at(col).at(row) != nullptr and is_found == false) {

                    // Finds the first match
                    for (int n = 0; n < COLUMNS - col; ++n) {
                        if (board_.at(col + n).at(row) == nullptr) {
                            break;
                        }
                            if (board_.at(col + n).at(row)->brush()
                                == board_.at(col).at(row)->brush()) {
                                ++how_many_same;
                            } else {
                                break;
                            }
                    }
                    // If match found, calls function that removes same -colored squares
                    if (how_many_same >= 3) {
                        // If found match at current loop, sets is_found true
                        removeSquares(col, row, how_many_same, true);
                        is_found = true;
                    }
                }
        }
    }

}

/* Checks if there is equal or more than three same colored squares in
 * vertical lines
 *   No paramers or return values
 */
void MainWindow::checkLinesVertically()
{
    // Keeps track of how many same colore found side by side
    int how_many_same = 0;
    // Checks if there is match at one loop
    bool is_found = false;

    for ( int col = 0; col < COLUMNS; ++col ) {
        for (int row = 0; row  < ROWS; ++row) {
            how_many_same = 0;

                // Goes to find match only if these terms come true
                if ( row != ROWS-1 and row != ROWS-2 and is_found == false and board_.at(col).at(row) != nullptr) {
                    // Loop that finds the first match
                    for (int n = 0; n < ROWS-row; ++n) {
                        if ( board_.at(col).at(row + n) == nullptr) {
                            break;
                        }
                            if (board_.at(col).at(row + n)->brush()
                                == board_.at(col).at(row)->brush()) {
                                ++how_many_same;
                            } else {
                                break;
                            }
                    }
                    // If match found, calls function that removes same -colored squares
                    if (how_many_same >= 3) {
                        // If found match at current loop, sets is_found true
                        removeSquares(col, row, how_many_same, false);
                        is_found = true;
                    }

             }
        }
    }
}

/* Checks if there is equal or more than three same colored squares in
 * vertical lines
 *   No paramers or return values
 */
void MainWindow::removeSquares(int col, int row, int amount, bool is_horizontal) {

    // Current imformation for dropping slots
    current_col_ = col;
    current_row_ = row;
    current_amount_ = amount;

    // Chooses way of removing concerning to position of line
    if (is_horizontal) {
        int i = 0;
        while (i < amount) {
            // Removes from scene_ and sets it nullptr at board_
            scene_->removeItem(board_.at(col + i).at(row));
            board_.at(col+i).at(row) = nullptr;
            ++i;
        }
        if (is_delay_) {
            QTimer::singleShot(delay_, this, SLOT(dropSquaresHorizontally()));
        } else {
            dropSquaresHorizontally();
        }


    } else {
        int i = 0;
        while (i < amount) {
            // Removes from scene_ and sets it nullptr at board_
            scene_->removeItem(board_.at(col).at(row + i));
            board_.at(col).at(row + i) = nullptr;
            ++i;
        }
        if (is_delay_) {
            QTimer::singleShot(delay_, this, SLOT(dropSquaresVertically()));
        } else {
            dropSquaresVertically();
        }

    }
}

/* Drop squares if there is removed horizontal ones
 *   No parameters or return values
 */
void MainWindow::dropSquaresHorizontally()
{
    int i = 0;
    while (i < current_amount_) {
        int k = 1;
        while (k <= ROWS-1-current_row_ and board_.at(current_col_ + i).at(current_row_ + k) != nullptr) {
             board_.at(current_col_ + i).at(current_row_ + k)->moveBy(0, SQUARE_SIDE);
             board_.at(current_col_ + i).at(current_row_+(k-1)) = board_.at(current_col_ + i).at(current_row_ + k);
             board_.at(current_col_ + i).at(current_row_ + k) = nullptr;
             ++k;
        }
        ++i;
    }

    // Calls check-functions in case that there have became new matches
    // Also calls fillBoard function if user wants to
    if (is_delay_) {
        QTimer::singleShot(delay_, this, SLOT(checkLinesHorizontally()));
        QTimer::singleShot(delay_, this, SLOT(checkLinesVertically()));
        if (fill_board_) {
            QTimer::singleShot(delay_, this, SLOT(fillBoard()));
        }
    } else {
        checkLinesHorizontally();
        checkLinesVertically();
        if (fill_board_) {
            fillBoard();
        }
    }

}

/* Drop squares if there is removed vertical ones
 *   No parameters or return values
 */
void MainWindow::dropSquaresVertically() {

    int up_row = current_row_ + current_amount_-1;
    int i = 1;
    while ((i <= (ROWS-1) - (up_row)) and (board_.at(current_col_).at(up_row + i) != nullptr)) {
        board_.at(current_col_).at(up_row + i)->moveBy(0, current_amount_ * SQUARE_SIDE);
        board_.at(current_col_).at(up_row + (i-current_amount_)) = board_.at(current_col_).at(up_row + i);
        board_.at(current_col_).at(up_row + i) =  nullptr;
        ++i;
    }

    // Calls check-functions in case that there have became new matches
    // Also calls fillBoard function if user wants to
    if (is_delay_) {
        QTimer::singleShot(delay_, this, SLOT(checkLinesHorizontally()));
        QTimer::singleShot(delay_, this, SLOT(checkLinesVertically()));
        if (fill_board_) {
            QTimer::singleShot(delay_, this, SLOT(fillBoard()));
        }
    } else {
        checkLinesHorizontally();
        checkLinesVertically();
        if (fill_board_) {
            fillBoard();
        }
    }

}

/* Changes the direction of switching two squares with each other
 *   No parameters or return values
 */
void MainWindow::on_directionPushButton_clicked()
{
    if (ui_->directionPushButton->text() == "Vertical") {
        ui_->directionPushButton->setText("Horizontal");
        ui_->colSpinBox->setMaximum(COLUMNS - 1);
        ui_->rowSpinBox->setMaximum(ROWS - 2);


    } else {
        ui_->directionPushButton->setText("Vertical");
        ui_->colSpinBox->setMaximum(COLUMNS - 2);
        ui_->rowSpinBox->setMaximum(ROWS - 1);
    }
}

/* Important slot for playing game, starts the game
 * with switching two squares
 */
void MainWindow::on_goPushButton_clicked()
{
    // Column and row below and next to chosen column and row
    int replaceableCol;
    int replaceableRow;
    ui_->testiTextEdit->setText("");
    if (ui_->directionPushButton->text() == "Horizontal") {

        replaceableCol = selectedCol_;
        replaceableRow = selectedRow_ - 1;
        // Moves squares if both of are inequal to nullptr
        if (board_.at(selectedCol_).at(selectedRow_) != nullptr
                and board_.at(replaceableCol).at(replaceableRow) != nullptr) {
            board_.at(selectedCol_).at(selectedRow_)->moveBy(0,SQUARE_SIDE);
            board_.at(replaceableCol).at(replaceableRow)->moveBy(0,-SQUARE_SIDE);
        } else {
            ui_->testiTextEdit->setText("Error, cannot move empty place");
        }
    } else {
        replaceableCol = selectedCol_ + 1;
        replaceableRow = selectedRow_;
        // Moves squares if both of are inequal to nullptr
        if (board_.at(selectedCol_).at(selectedRow_) != nullptr
                and board_.at(replaceableCol).at(replaceableRow) != nullptr) {
            board_.at(selectedCol_).at(selectedRow_)->moveBy(SQUARE_SIDE,0);
            board_.at(replaceableCol).at(replaceableRow)->moveBy(-SQUARE_SIDE, 0);
        } else {
            ui_->testiTextEdit->setText("Error, cannot move empty place");
        }
    }
    // Changes vector board_ if both of squares are inequal to nullptr
    if (board_.at(selectedCol_).at(selectedRow_) != nullptr
            and board_.at(replaceableCol).at(replaceableRow) != nullptr) {
        QGraphicsRectItem* first = board_.at(selectedCol_).at(selectedRow_);
        QGraphicsRectItem* second = board_.at(replaceableCol).at(replaceableRow);

        board_.at(selectedCol_).at(selectedRow_) = second;
        board_.at(replaceableCol).at(replaceableRow) = first;
    }

   // calls check-slots for finding matches
    checkLinesHorizontally();
    checkLinesVertically();

    // Fills the board if user is chosen it
    if (fill_board_) {
        if (is_delay_) {
            QTimer::singleShot(delay_, this, SLOT(fillBoard()));
        } else {
            fillBoard();
        }
    }
}


/* Fills the board after there is removed squares
 *   No parameters or return values
 */
void MainWindow::fillBoard() {
    //Initializes the board with colored squares
    QBrush whiteBrush(Qt::white);
    QPen blackPen(Qt::black);
    for ( int col = 0; col < COLUMNS; ++col ) {
        for (int row = 0; row < ROWS; ++row) {
            if (board_.at(col).at(row) == nullptr) {
                square_ = scene_->addRect(SQUARE_SIDE*col, SQUARE_SIDE*(ROWS-1-row), SQUARE_SIDE, SQUARE_SIDE, blackPen, whiteBrush);
                board_.at(col).at(row) = square_;
             }
        }
    }

    for ( int col = 0; col < COLUMNS; ++col ) {
        for (int row = 0; row < ROWS; ++row) {
            if (board_.at(col).at(row)->brush() == Qt::white) {
                coloreSquare(col, row);
             }
        }
    }
}

/* Initializes board, with randomly colored squares
 *   No parameters or return values
 */
void MainWindow::on_initializeBoardButton_clicked()
{
    delete scene_;
    scene_ = new QGraphicsScene(this);
    ui_->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a fruit is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    board_ = {};
    initializeBoard();
    coloreBoard();
    is_delay_ = false;
    ui_->noDelayButton->setChecked(true);
    fill_board_ = false;
    ui_->fillBoardcheckBox->setChecked(false);
    ui_->rowSpinBox->setValue(0);
    ui_->colSpinBox->setValue(0);
}

/* Slot for choosing No Delay - radiobutton
 *   Parameter:
 *     bool checked, tells if the button is choosen or not
 */
void MainWindow::on_noDelayButton_toggled(bool checked)
{
    ui_->noDelayButton->setChecked(checked);
    is_delay_ = false;
}

/* Slot for choosing Delay - radiobutton
 *   Parameter:
 *     bool checked, tells if the button is choosen or not
 */
void MainWindow::on_delayButton_toggled(bool checked)
{
    ui_->delayButton->setChecked(checked);
    is_delay_ = true;
}

/* Slot for choosing Fill Board - checkBox
 *   Parameter:
 *     bool checked, tells if the box is choosen or not
 */
void MainWindow::on_fillBoardcheckBox_toggled(bool checked)
{
    ui_->fillBoardcheckBox->setChecked(checked);
    if (checked == true) {
        fill_board_ = true;
     } else {
        fill_board_ = false;
    }
}

/* Slot for taking users input to attribute selectedCol_
 *   Parameter:
 *     int arq1, users input for column
 */
void MainWindow::on_colSpinBox_valueChanged(int arg1)
{
    selectedCol_ = arg1;
    colChosen_ = true;
    if (colChosen_ and rowChosen_) {
        ui_->goPushButton->setEnabled(true);
    }
}

/* Slot for taking users input to attribute selectedCol_
 *   Parameter:
 *     int arq1, users input for row
 */
void MainWindow::on_rowSpinBox_valueChanged(int arg1)
{
    selectedRow_ = (ROWS-1) - arg1;
    rowChosen_ = true;
    if (colChosen_ and rowChosen_) {
        ui_->goPushButton->setEnabled(true);
    }
}
