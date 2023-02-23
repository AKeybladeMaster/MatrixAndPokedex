#ifndef POKEMON_H
#define POKEMON_H
#include <QString>
#include <QList>
#include <QLabel>

class Pokemon {

public:

    /*
     * @brief empty constructor
    */
    Pokemon();

    Pokemon(int debug_id, QString img, int poke_id, QString name, QList<QString> types, int total, int hp, int attack, int defense, int spAtk, int spDef, int speed);

    int getDebugId() const;
    QString getImg() const;
    int getPokeId() const;
    QString getName() const;
    QList<QString> getTypes() const;
//    QString getTypeLabel(QString type) const;
    int getTotal() const;
    int getHp() const;
    int getAttack() const;
    int getDefense() const;
    int getSpAtk() const;
    int getSpDef() const;
    int getSpeed() const;
    QLabel *getRoundedTypeLab(const QString type);

    bool operator==(const Pokemon &p2) const;
//    Pokemon &operator=(const Pokemon &p2);

private:
    int _debug_id;
    QString _img;
    int _poke_id;
    QString _name;
    QList<QString> _types;
    int _total;
    int _hp;
    int _attack;
    int _defense;
    int _spAtk;
    int _spDef;
    int _speed;

};

#endif // POKEMON_H
