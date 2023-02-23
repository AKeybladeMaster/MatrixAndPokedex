#include "mainwindow.h"
#include "pokemon.h"
#include "ui_mainwindow.h"
#include <QIODevice>
#include <QFile>
#include <QLabel>
#include <QList>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QCheckBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QComboBox>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QPixmap>
#include <QSize>
#include <QPainter>
#include <QWindow>
//#include <QCloseEvent>

QWidget *compareScreenWidget, *pokedexScreenWidget;
QGridLayout *compareGridLayout, *pokedexGridLayout;
QList<Pokemon> pokemonsList;
QStringList pokemonTypes = {"Fire", "Water", "Grass", "Electric", "Ice", "Fighting", "Poison", "Ground", "Flying", "Psychic", "Bug", "Rock", "Ghost", "Dragon", "Dark", "Steel", "Fairy", "Normal"};
QStringList pokemonNames;

// usato in onClickButton per 'cambiare window' nella stessa schermata
bool swapToCompareWindow = true;

// parte compare
QBarSeries *series;
QBarCategoryAxis *barStatAxis;
QChart *chart;
QChartView *chartView;

QVBoxLayout *firstPokemonLayout, *secondPokemonLayout;

QHBoxLayout *midPokemonsChartLayout, *topHorizontalComboBoxLayout, *firstPokemonTypesLayout, *secondPokemonTypesLayout;

QLabel *firstImgLab, *secondImgLab, *firstPokemonType1, *firstPokemonType2, *secondPokemonType1, *secondPokemonType2;

Pokemon firstPokemon, secondPokemon;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui -> setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::closeEvent(QCloseEvent *event) {
//        delete firstPokemonComboBoxChoice;
//        delete secondPokemonComboBoxChoice;
//        delete compareScreenWidget;
//        firstPokemonComboBoxChoice = nullptr; // gia memory vuota
//        secondPokemonComboBoxChoice = nullptr;    // gia memory vuota
//        compareScreenWidget = nullptr;
//        event -> accept();
//}


/**
 * @brief Carica i pokemon dal file csv e li salva in una QList
 *
 * @return void
*/
void MainWindow::loadPokemons() {

    QFile file(":/dataset/pokedex.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }
    int augmentId = 0;

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (augmentId == 0) {
            augmentId++;
        }
        else {
        if (line.contains("\"")) {
            int firstEncounter = line.indexOf("\"");
            int secondEncounter = line.indexOf("\"", firstEncounter + 1);

            QString middle = line.mid(firstEncounter, secondEncounter - firstEncounter);

            middle.replace("\"", "");

            QString  middle2 = middle;

            middle2.replace(",", "/");

            line.replace("\"" + middle + "\"", middle2.replace(" ", ""));

            line.replace("\"", "");
            }

        QStringList fields = line.split(',');

          int generic_id = augmentId++;
          QString img = fields[0];
          int poke_id = fields[1].toUInt();
          QString name = QString(fields[2]);
          QList<QString> types = QString(fields[3]).split('/');
          int total = QString(fields[4]).toUInt();
          int hp = QString(fields[5]).toUInt();
          int attack = QString(fields[6]).toUInt();
          int defense = QString(fields[7]).toUInt();
          int spAtk = QString(fields[8]).toUInt();
          int spDef = QString(fields[9]).toUInt();
          int speed = QString(fields[10]).toUInt();

          Pokemon pokemon(generic_id, img, poke_id, name, types, total, hp, attack, defense, spAtk, spDef, speed);

          pokemonsList.append(pokemon);
            }
        }
    file.close();
}


/**
 * @brief Crea la schermata del pokedex e la mostra (uso di widget unico che contiene layout)
 *
 * @return void
*/
void MainWindow::viewPokedexWindow() {

    pokemonTypes = {"Fire", "Water", "Grass", "Electric", "Ice", "Fighting", "Poison", "Ground", "Flying", "Psychic", "Bug", "Rock", "Ghost", "Dragon", "Dark", "Steel", "Fairy", "Normal"};

    pokedexScreenWidget = new QWidget;
    pokedexGridLayout = new QGridLayout;

    // load dei pokemon da file
    if (pokemonsList.isEmpty()) {
        loadPokemons();
    }

    // setta tabella a posizione 1, 0 nel gridLayout
    fillOrRefillTable();

    QList<QCheckBox *> checkboxes;

    for (const QString &type : pokemonTypes) {
        QCheckBox *checkbox = new QCheckBox(type);
        checkbox -> setChecked(true);
        checkboxes.append(checkbox);
    }

    QVBoxLayout *checkboxesLayout = new QVBoxLayout;

    for (QCheckBox *checkbox : checkboxes) {
        checkboxesLayout -> addWidget(checkbox);
    }

    pokedexGridLayout->addLayout(checkboxesLayout, 0, 1);

    for (QCheckBox *checkbox : checkboxes) {
        connect(checkbox, &QCheckBox::stateChanged, this, &MainWindow::checkboxStateChange);
    }

    QPushButton *openCompareWinButton = new QPushButton("COMPARE 2 POKEMONS WITH THEIR RESPECTIVE STATS!");

    pokedexGridLayout->addWidget(openCompareWinButton, 1, 0);

    QObject::connect(openCompareWinButton, &QPushButton::clicked, this, &MainWindow::onClickButton);

    // setta il layout del widget principale e lo setta come widget centrale
    pokedexScreenWidget -> setLayout(pokedexGridLayout);
    this -> setCentralWidget(pokedexScreenWidget);
}


/**
 * @brief Crea e riempie il widget tabellare di pokemon (caricati precedentemente)
 *
 * @return void
*/
void MainWindow::fillOrRefillTable() {

    // da usare per evitare di avere celle vuote in tabella
    QList<Pokemon> filteredPokemonsList;

    // se il tipo e' presente tra i selezionati lo aggiunge alla lista filtrata
    for (int i = 0; i < pokemonsList.size(); i++) {
            bool exists = true;
        for (int p = 0; p < pokemonsList.at(i).getTypes().length(); p++) {
            if (!pokemonTypes.contains(pokemonsList.at(i).getTypes().at(p)))
                exists = false;
        }
        if (exists)
            filteredPokemonsList.append(pokemonsList.at(i));
    }

    // fatto per matchare il numero di pokemon filtrati e non avere celle vuote visibili
    QTableWidget *pokedexTable = new QTableWidget(filteredPokemonsList.size(), 10);

    // adattare automaticamente i size degli header orizzontali (e anche della finestra)
    pokedexTable->setHorizontalHeaderLabels(QStringList() << "Name" << "Pokemon ID"  << "Type" << "Total" << "HP" << "Attack" << "Defense" << "Sp. Atk" << "Sp. Def" << "Speed");
    pokedexTable->horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

    // riempiamo la tabella filtrata in base ai tipi selezionati
    for (int i = 0; i < filteredPokemonsList.size(); i++) {
        Pokemon pokemon = filteredPokemonsList.at(i);

        // uso vbox con i pokemon type per metterli nella cella della tabella
        QVBoxLayout *vboxType = new QVBoxLayout;

        for (int k = 0; k < pokemon.getTypes().length(); k++) {
            QLabel *typeModernLabel = pokemon.getRoundedTypeLab(pokemon.getTypes().at(k));
            vboxType -> addWidget(typeModernLabel);
            typeModernLabel -> setAlignment(Qt::AlignCenter);
        }

        QWidget *widget = new QWidget;
        widget -> setLayout(vboxType);

        QTableWidgetItem *tabWidItem = new QTableWidgetItem;

        pokedexTable -> setItem(i, 0, new QTableWidgetItem(pokemon.getName()));

        tabWidItem -> setData(Qt::DisplayRole, pokemon.getPokeId());
        tabWidItem -> setText(QString::number(pokemon.getPokeId()));
        pokedexTable -> setItem(i, 1, tabWidItem);

        // non e' possibile fare sorting con questa colonna dei tipi
        pokedexTable -> setCellWidget(i, 2, widget);

        tabWidItem = new QTableWidgetItem;
        tabWidItem -> setData(Qt::DisplayRole, pokemon.getTotal());
        tabWidItem -> setText(QString::number(pokemon.getTotal()));
        pokedexTable -> setItem(i, 3, tabWidItem);

        tabWidItem = new QTableWidgetItem;
        tabWidItem -> setData(Qt::DisplayRole, pokemon.getHp());
        tabWidItem -> setText(QString::number(pokemon.getHp()));
        pokedexTable -> setItem(i, 4, tabWidItem);

        tabWidItem = new QTableWidgetItem;
        tabWidItem -> setData(Qt::DisplayRole, pokemon.getAttack());
        tabWidItem -> setText(QString::number(pokemon.getAttack()));
        pokedexTable -> setItem(i, 5, tabWidItem);

        tabWidItem = new QTableWidgetItem;
        tabWidItem -> setData(Qt::DisplayRole, pokemon.getDefense());
        tabWidItem -> setText(QString::number(pokemon.getDefense()));
        pokedexTable -> setItem(i, 6, tabWidItem);

        tabWidItem = new QTableWidgetItem;
        tabWidItem -> setData(Qt::DisplayRole, pokemon.getSpAtk());
        tabWidItem -> setText(QString::number(pokemon.getSpAtk()));
        pokedexTable -> setItem(i, 7, tabWidItem);

        tabWidItem = new QTableWidgetItem;
        tabWidItem -> setData(Qt::DisplayRole, pokemon.getSpDef());
        tabWidItem -> setText(QString::number(pokemon.getSpDef()));
        pokedexTable -> setItem(i, 8, tabWidItem);

        tabWidItem = new QTableWidgetItem;
        tabWidItem -> setData(Qt::DisplayRole, pokemon.getSpeed());
        tabWidItem -> setText(QString::number(pokemon.getSpeed()));
        pokedexTable -> setItem(i, 9, tabWidItem);

    }

    // allineamento del testo e altezza riga
    for (int x = 0; x < pokedexTable -> rowCount(); x++) {
        for (int y = 0; y < pokedexTable -> columnCount(); y++) {
            QTableWidgetItem *item = pokedexTable -> item(x, y);
            pokedexTable -> setRowHeight(x, 100);
            if (item) {
                item -> setTextAlignment(Qt::AlignCenter);
                // make items not editable
                item -> setFlags(item -> flags() ^ Qt::ItemIsEditable);
            }
        }
    }

    // sorting attivato + sorting iniziale in base al pokemon id (col 1)
    pokedexTable -> setSortingEnabled(true);
    pokedexTable -> sortItems(1, Qt::AscendingOrder);

    // aggiungendo si sovrascrive la tabella precedente
    pokedexGridLayout -> setContentsMargins(20, 20, 20, 20);
    pokedexGridLayout -> addWidget(pokedexTable, 0, 0);

}


/**
 * @brief controlla che un checkbox sia stato selezionato o deselezionato e aggiorna la lista dei tipi
 *
 * @param state
 *
 * @return void
*/
void MainWindow::checkboxStateChange(int state) {
    // prende la sorgente dell'oggetto checkbox che ha subito cambiamento ad ogni check/uncheck
    QCheckBox *checkbox = qobject_cast<QCheckBox*>(sender());

    // controllando lo stato dell'oggetto esegue certe funzioni
    // almeno 1 tipo deve essere attivo o si ottiene un warning
    if (state == Qt::Unchecked) {
        if (pokemonTypes.length() > 1)
            pokemonTypes.removeOne(checkbox -> text());
        else {
            pokemonTypes.removeOne(checkbox -> text());
            checkbox -> setCheckState(Qt::Checked);
            QMessageBox::warning(this, "Error", "Almeno un tipo deve essere selezionato!");
        }
    }
    else if (state == Qt::Checked) {
        // una checkbox e' stata selezionata sul momento, mettiamo il suo valore (text del pokemon type) nella lista
        pokemonTypes.append(checkbox -> text());
    }

    // aggiorna la tabella
    fillOrRefillTable();
}


/**
 * @brief sul click di un designato bottone cambia la schermata tra quella della pokedex e quella di confronto
 *
 * @return void
*/
void MainWindow::onClickButton() {
    if (true == swapToCompareWindow) {
        swapToCompareWindow = false;
        viewCompareWindow();
    }
    else {
        swapToCompareWindow = true;
        viewPokedexWindow();
    }
}


/**
 * @brief crea la schermata di confronto tra due pokemon e la mostra
 *
 * @return void
*/
void MainWindow::viewCompareWindow() {

    // faccio uso di (quasi) tutte variabili globali per aggiornare la schermata di confronto
    compareScreenWidget = new QWidget;
    compareGridLayout = new QGridLayout;

    firstPokemon = pokemonsList.at(0);
    secondPokemon = pokemonsList.at(4);

    QPushButton *goBackButton = new QPushButton("GO BACK TO THE POKEDEX LIST");
    QComboBox *firstPokemonComboBoxChoice = new QComboBox();
    QComboBox *secondPokemonComboBoxChoice = new QComboBox();

    topHorizontalComboBoxLayout = new QHBoxLayout;
    midPokemonsChartLayout = new QHBoxLayout;

    firstImgLab = new QLabel;
    secondImgLab = new QLabel;

    firstPokemonType1 = new QLabel();
    firstPokemonType2 = new QLabel();

    secondPokemonType1 = new QLabel();
    secondPokemonType2 = new QLabel();

    firstPokemonLayout = new QVBoxLayout;
    secondPokemonLayout = new QVBoxLayout;

    firstPokemonTypesLayout = new QHBoxLayout;
    secondPokemonTypesLayout = new QHBoxLayout;

    series = new QBarSeries();
    chart = new QChart();
    barStatAxis = new QBarCategoryAxis();
    chartView = new QChartView(chart);
    QStringList barStats;
    barStats << "Hp" << "Attack" << "Defense" << "Sp.Atk" << "Sp.Def" << "Speed";
    barStatAxis -> append(barStats);
    // setAxisX deprecated - non ho trovato altro modo per settare l'asse delle x
    chart -> setAxisX(barStatAxis, series);
    chart -> setAnimationOptions(QChart::SeriesAnimations);
    chart -> setTitle("Pokemon Stats/Diffs Comparison");
    chartView -> setRenderHint(QPainter::Antialiasing);

    firstPokemonTypesLayout -> addWidget(firstPokemonType1);
    firstPokemonTypesLayout -> addWidget(firstPokemonType2);

    secondPokemonTypesLayout -> addWidget(secondPokemonType1);
    secondPokemonTypesLayout -> addWidget(secondPokemonType2);

    firstPokemonLayout -> addWidget(firstImgLab);
    firstPokemonLayout -> addLayout(firstPokemonTypesLayout);

    secondPokemonLayout -> addWidget(secondImgLab);
    secondPokemonLayout -> addLayout(secondPokemonTypesLayout);

    // pokemonsList e pokemonNames son globali
    for (int i = 0; i < pokemonsList.size(); i++)
        pokemonNames.append(pokemonsList.at(i).getName());

    firstPokemonComboBoxChoice -> addItems(pokemonNames);
    secondPokemonComboBoxChoice -> addItems(pokemonNames);
    secondPokemonComboBoxChoice -> setCurrentIndex(4);

    topHorizontalComboBoxLayout -> addWidget(firstPokemonComboBoxChoice);
    topHorizontalComboBoxLayout -> addWidget(goBackButton);
    topHorizontalComboBoxLayout -> addWidget(secondPokemonComboBoxChoice);

    midPokemonsChartLayout -> addLayout(firstPokemonLayout);
    midPokemonsChartLayout -> addWidget(chartView);
    midPokemonsChartLayout -> addLayout(secondPokemonLayout);

    compareGridLayout -> addLayout(topHorizontalComboBoxLayout, 0, 0);
    compareGridLayout -> addLayout(midPokemonsChartLayout, 1, 0);

    createOrRecreateChart(firstPokemon, secondPokemon);

    connect(firstPokemonComboBoxChoice, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        firstPokemon = pokemonsList.at(index);
//        qDebug() << "firstPokemonComboBoxChoice selezionata: " << firstPokemonComboBoxChoice -> currentText();
        createOrRecreateChart(firstPokemon, secondPokemon);
    });

    connect(secondPokemonComboBoxChoice, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        secondPokemon = pokemonsList.at(index);
//        qDebug() << "secondPokemonComboBoxChoice selezionata: " << secondPokemonComboBoxChoice -> currentText();
        createOrRecreateChart(firstPokemon, secondPokemon);
    });

    QObject::connect(goBackButton, &QPushButton::clicked, this, &MainWindow::onClickButton);

    // setta il layout del widget principale e lo setta come widget centrale
    compareScreenWidget ->setLayout(compareGridLayout);
    this -> setCentralWidget(compareScreenWidget);
}


/**
 * @brief Crea o ricrea il grafico in base ai pokemon selezionati; aggiorna le immagini e i tipi dei pokemon
 *
 * @param firstPokemon
 * @param secondPokemon
 *
 * @return void
*/
void MainWindow::createOrRecreateChart(Pokemon firstPokemon, Pokemon secondPokemon) {

    // se i pokemon sono uguali (overload operator==) da warning
    if (firstPokemon == secondPokemon)
        QMessageBox::warning(this, "Errore", "Non e' possibile selezionare lo stesso pokemon per la comparazione!");
    else {

        QList<QString> types = firstPokemon.getTypes();

        QPixmap pixmap(":/images/" + firstPokemon.getImg());

        QSize size(300, 300);

        pixmap = pixmap.scaled(size, Qt::KeepAspectRatio);

        firstImgLab -> setPixmap(pixmap);

        // assegnamento non funzionante; sostituito con setText e setStyleSheet
        firstPokemonType1 -> setText(firstPokemon.getRoundedTypeLab(types.at(0)) ->text());
        firstPokemonType1 -> setStyleSheet(firstPokemon.getRoundedTypeLab(types.at(0)) -> styleSheet());
        firstPokemonType1 -> setFixedSize(80, 40);
//        firstPokemonType1 -> setTextFormat(Qt::RichText);
        firstPokemonType1 -> setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

        if (types.length() == 1) {
            firstPokemonType2 -> setVisible(false);
//            qDebug() << "entrato nel PRIMO if dei types layout POKEMON TYPES 1";
        } else {
            firstPokemonType2 -> setVisible(true);
            firstPokemonType2 -> setText(firstPokemon.getRoundedTypeLab(types.at(1)) -> text());
            firstPokemonType2 -> setStyleSheet(firstPokemon.getRoundedTypeLab(types.at(1)) -> styleSheet());
            firstPokemonType2 -> setFixedSize(80, 40);
    //        firstPokemonType2 -> setTextFormat(Qt::RichText);
            firstPokemonType2 -> setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
//            qDebug() << "entrato nel SECONDO if dei types layout POKEMON TYPES 1";
        }

        types = secondPokemon.getTypes();

        QPixmap pixmap2(":/images/" + secondPokemon.getImg());

        QSize size2(300, 300);

        pixmap2 = pixmap2.scaled(size2, Qt::KeepAspectRatio);

        secondImgLab -> setPixmap(pixmap2);

        // assegnamento non funzionante; sostituito con setText e setStyleSheet
        secondPokemonType1 -> setText(secondPokemon.getRoundedTypeLab(types.at(0)) ->text());
        secondPokemonType1 -> setStyleSheet(secondPokemon.getRoundedTypeLab(types.at(0)) -> styleSheet());
        secondPokemonType1 -> setFixedSize(80, 40);
//        secondPokemonType1 -> setTextFormat(Qt::RichText);
        secondPokemonType1 -> setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

        if (types.length() == 1) {
            secondPokemonType2 -> setVisible(false);
//            qDebug() << "entrato nel PRIMO if dei types layout POKEMON TYPES 2";
        } else {
            secondPokemonType2 -> setVisible(true);
            secondPokemonType2 -> setText(secondPokemon.getRoundedTypeLab(types.at(1)) -> text());
            secondPokemonType2 -> setStyleSheet(secondPokemon.getRoundedTypeLab(types.at(1)) -> styleSheet());
            secondPokemonType2 -> setFixedSize(80, 40);
    //        firstPokemonType2 -> setTextFormat(Qt::RichText);
            secondPokemonType2 -> setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
//            qDebug() << "entrato nel SECONDO if dei types layout POKEMON TYPES 2";
        }

        //        delete per poter istanziare di nuovo una nuova chart con
        //        i nuovi pokemon selezionati (errato, leak)
        //        delete series;
        //        chart ->removeAllSeries();
        //        series = new QBarSeries();

        // svuotamento della series e della chart per poter riempirle di nuovo
        while (chart -> series().count()) {
            chart -> removeSeries(chart -> series().first());
        }
        series -> clear();

        //creazione delle barre con le statistiche dei pokemon
        QBarSet *firstPokemonBar = new QBarSet(firstPokemon.getName());
        *firstPokemonBar << firstPokemon.getHp() << firstPokemon.getAttack() << firstPokemon.getDefense() << firstPokemon.getSpAtk() << firstPokemon.getSpDef() << firstPokemon.getSpeed();
        series -> append(firstPokemonBar);

        QBarSet *secondPokemonBar = new QBarSet(secondPokemon.getName());
        *secondPokemonBar << secondPokemon.getHp() << secondPokemon.getAttack() << secondPokemon.getDefense() << secondPokemon.getSpAtk() << secondPokemon.getSpDef() << secondPokemon.getSpeed();
        series -> append(secondPokemonBar);

        chart -> addSeries(series);
    }
}
