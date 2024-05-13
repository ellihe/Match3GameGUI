/* Match3 (luotu 2020)
 *
 * Kuvaus:
 *  Tiedosto toteuttaa MainWindow luokan muunmuassa tiedoston mainwindow.cpp
 * käytettäväksi. Luokassa on määriteltynä sekä slotteja, että yksityisiä funktioita.
 * Luokan attribuutteja muuttamalla, käyttäjä voi tehdä pelistään omannäköisensä
 *
 * Ohjelman kirjoittaja
 * Nimi: Ellinoora Hetemaa
 * Palautteen kieli (fi/en): fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 * Ohjelman tarkempi toteutusratkaisu ja käyttöohje on erillisenä tiedostona
 *
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>
#include <deque>
#include <random>
#include <QTimer>
#include <QLabel>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void dropSquaresHorizontally();

    void dropSquaresVertically();

    void checkLinesHorizontally();

    void checkLinesVertically();

    void on_colSpinBox_valueChanged(int arg1);

    void on_rowSpinBox_valueChanged(int arg1);

    void on_directionPushButton_clicked();

    void on_goPushButton_clicked();

    void on_initializeBoardButton_clicked();

    void on_noDelayButton_toggled(bool checked);

    void on_delayButton_toggled(bool checked);

    void on_fillBoardcheckBox_toggled(bool checked);

    void fillBoard();

private:

    Ui::MainWindow *ui_;

    // Scene for the game grid
    QGraphicsScene* scene_;

    QGraphicsRectItem* square_;

    std::vector <std::vector<QGraphicsRectItem*>> board_;

    int selectedCol_;

    int selectedRow_;

    bool colChosen_ = false;

    bool rowChosen_ = false;

    int delay_ = 1000;

    bool is_delay_ = false;

    bool fill_board_ = false;

    // The column where removing or dropping starts
    int current_col_;
    // The row where removing or dropping starts
    int current_row_;
    // Amount of same colored squares in same verticall or horizontal line
    int current_amount_;

    // Margins for the drawing area (the graphicsView object)
    // You can change the values as you wish
    const int TOP_MARGIN = 80;
    const int LEFT_MARGIN = 80;

    // Size of a square containing a fruit
    const int SQUARE_SIDE = 40; // give your own value here
    // Number of vertical cells (places for fruits)
    const int ROWS = 10; // give your own value here
    // Number of horizontal cells (places for fruits)
    const int COLUMNS = 10; // give your own value here

    // Constants describing scene coordinates
    const int BORDER_UP = 0;
    const int BORDER_DOWN = SQUARE_SIDE * ROWS;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = SQUARE_SIDE * COLUMNS;

    // Constants for different fruits and the number of them
    // Add/remove fruits as you wish, or you can remove the whole enum type
    // Actually only the value NUMBER_OF_FRUITS is needed in the
    // template code
    std::vector <QBrush> Fruit_colors_ {Qt::red, Qt::magenta, Qt::darkMagenta,
                QColor(255, 0, 0, 127), Qt::green, Qt::blue, Qt::yellow,
                Qt::darkRed, QColor(255,165,0)};

    int NUMBER_OF_FRUITS = 9; //maximum 9

    // For randomly selecting fruits for the grid
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distr_;

    // For initialize board vector with white squares
    void initializeBoard();

    // For setting color to all squares at beginning
    void coloreBoard();

    void coloreSquare(int col, int row);

    // Writes the titles for the grid rows and columns
    void initTitles();

    // Removes squares if there is equal or more than three same
    //color square in same horizontal or vertical line
    void removeSquares(int col, int row, int amount, bool is_horizontal);

};
#endif // MAINWINDOW_HH
