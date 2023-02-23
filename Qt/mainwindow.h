#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pokemon.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //    void closeEvent(QCloseEvent *event);

    void loadPokemons();

    void viewPokedexWindow();

    void fillOrRefillTable();

    void onClickButton();

    void setupCompare();

    void checkboxStateChange(int state);

    void viewCompareWindow();

    void createOrRecreateChart(Pokemon firstPokemon, Pokemon secondPokemon);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
