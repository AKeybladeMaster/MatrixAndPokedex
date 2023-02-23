#include "pokemon.h"
//#include <QLabel>


/**
 * @brief empty constructor (allocate)
 */
Pokemon::Pokemon() {

}

/**
 * @brief default constructor
 */
Pokemon::Pokemon(int debug_id, QString img, int poke_id, QString name, QList<QString> types, int total, int hp, int attack, int defense, int spAtk, int spDef, int speed)
        : _debug_id(debug_id), _img(img), _poke_id(poke_id), _name(name), _types(types), _total(total), _hp(hp), _attack(attack), _defense(defense), _spAtk(spAtk), _spDef(spDef), _speed(speed) {}

int Pokemon::getDebugId() const {
    return _debug_id;
}

QString Pokemon::getImg() const {
    return _img;
}

int Pokemon::getPokeId() const {
    return _poke_id;
}

QString Pokemon::getName() const {
    return _name;
}

QList<QString> Pokemon::getTypes() const {
    return _types;
}

int Pokemon::getTotal() const {
    return _total;
}

int Pokemon::getHp() const {
    return _hp;
}

int Pokemon::getAttack() const {
    return _attack;
}

int Pokemon::getDefense() const {
    return _defense;
}

int Pokemon::getSpAtk() const {
    return _spAtk;
}

int Pokemon::getSpDef() const {
    return _spDef;
}

int Pokemon::getSpeed() const {
    return _speed;
}

/**
* @brief overload dell'operatore di uguaglianza per confrontare due pokemon
*
* @param p2 pokemon da confrontare
*
* @return true se i due pokemon hanno lo stesso pokemon id e lo stesso nome
*/
bool Pokemon::operator==(const Pokemon &p2) const {
    return this -> _poke_id == p2.getPokeId() && this -> _name == p2.getName();
}

// overload dell'operatore di assegnamento per assegnare un pokemon ad un altro
//Pokemon& Pokemon::operator=(const Pokemon &p2) {
//    this -> _debug_id = p2.getDebugId();
//    this -> _img = p2.getImg();
//    this -> _poke_id = p2.getPokeId();
//    this -> _name = p2.getName();
//    this -> _types = p2.getTypes();
//    this -> _total = p2.getTotal();
//    this -> _hp = p2.getHp();
//    this -> _attack = p2.getAttack();
//    this -> _defense = p2.getDefense();
//    this -> _spAtk = p2.getSpAtk();
//    this -> _spDef = p2.getSpDef();
//    this -> _speed = p2.getSpeed();
//    return *this;
//}

/**
 * @brief metodo per ottenere un label con il tipo del pokemon
 *
 * @param type tipo del pokemon
 *
 * @return label con css del tipo del pokemon
 */
QLabel *Pokemon::getRoundedTypeLab(const QString type) {

    QLabel *typeLabel = new QLabel;
    typeLabel -> setFixedSize(80, 40);
    typeLabel -> setText(type);
    typeLabel -> setAlignment(Qt::AlignCenter);

    // colore del testo + angoli (bolla)
    // Map semplice dove associare un colore a una stringa
    QMap<QString, QString> typeColors;
    typeColors.insert("fire", "#ea7a3c");
    typeColors.insert("water", "#539ae2");
    typeColors.insert("grass", "#71c558");
    typeColors.insert("electric", "#e5c531");
    typeColors.insert("ice", "#70cbd4");
    typeColors.insert("fighting", "#cb5f48");
    typeColors.insert("poison", "#b468b7");
    typeColors.insert("ground", "#cc9f4f");
    typeColors.insert("flying", "#7da6de");
    typeColors.insert("psychic", "#e5709b");
    typeColors.insert("bug", "#94bc4a");
    typeColors.insert("rock", "#b2a061");
    typeColors.insert("ghost", "#846ab6");
    typeColors.insert("dragon", "#6a7baf");
    typeColors.insert("dark", "#736C75");
    typeColors.insert("steel", "#89a1b0");
    typeColors.insert("fairy", "#e397d1");
    typeColors.insert("normal", "#aab09f");

    typeLabel -> setStyleSheet("QLabel { background-color: " + typeColors.value(type.toLower()) + "; color: white; font-weight: bold; border-radius: 15px; }");
    return typeLabel;
}
