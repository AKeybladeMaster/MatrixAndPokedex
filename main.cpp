#include <iostream>
#include "matrice3d.h"
#include <cassert>


/**
 * @brief Class customizzata di double per testare la matrice
*/
class doubleCustom {

public:
    doubleCustom(double d) : d(d) {}
    doubleCustom(): d(0) {}
    doubleCustom(const doubleCustom &other) : d(other.d) {}
    doubleCustom(doubleCustom &&other) : d(other.d) {}
    ~doubleCustom() {}

    operator double() const {
        return d;
    }
    
    doubleCustom &operator=(const doubleCustom &other) {
        d = other.d;
        return *this;
    }

    bool operator==(const doubleCustom &other) const {
        return d == other.d;
    }

    double getD() const {
        return d;
    }

private:
    double d;
};


/**
 * @brief Funtore per confronto tra celle di tipo doubleCustom
*/
struct doubleCustomEquals {
    bool operator()(const doubleCustom &a, const doubleCustom &b) const {
        return a.getD() == b.getD();
    }
};


/**
 * @brief Class customizzata di User per testare la matrice
*/
class userCustom {

public:
    userCustom(std::string name, unsigned int age) : _name(name), _age(age) {}
    userCustom(): _name(""), _age(0) {}
    userCustom(const userCustom &other) : _name(other._name), _age(other._age) {}
    userCustom(userCustom &&other) : _name(other._name), _age(other._age) {}
    ~userCustom() {}

    operator std::string() const {
        return _name;
    }
    
    userCustom &operator=(const userCustom &other) {
        _name = other._age;
        _age = other._age;
        return *this;
    }

    bool operator==(const userCustom &other) const {
        return _name == other._name && _age == other._age;
    }

    std::string getName() const {
        return _name;
    }

    unsigned int getAge() const {
        return _age;
    }

private:
    std::string _name;
    unsigned int _age;
};


/**
 * @brief Funtore per confronto tra celle di tipo userCustom
*/
struct userCustomEquals {
    bool operator()(const userCustom &a, const userCustom &b) const {
        return a.getName() == b.getName() && a.getAge() == b.getAge();
    }
};


/**
 * @brief Funtore per confronto tra celle di tipo char (primitive)
*/
struct charEquals {
    bool operator()(const char &a, const char &b) const {
        return a == b;
    }
};


/**
 * @brief Funtore per confronto tra celle di tipo int (primitive)
*/
struct intEquals {
    bool operator()(const int &a, const int &b) const {
        return a == b;
    }
};


/**
 * @brief Funtore per confronto tra celle di tipo double (primitive)
*/
struct doubleEquals {
    bool operator()(const double &a, const double &b) const {
        return a == b;
    }
};


/**
 * @brief Test di creazione, assegnamento e confronto celle matrice tipo T
*/
void testCreazioneAssegnamentoMatrice() {
    std::cout << "----- Creazione, assegnamento e confronto celle matrice tipo T -----" << std::endl;

    // Matrice con costruttore di default
    Matrice3D<char, charEquals> mat;
    assert(mat.get_rows() == 0);
    assert(mat.get_cols() == 0);
    assert(mat.get_depth() == 0);

    std::cout << "Matrice creata (tipo char) con dimensioni a 0: " << typeid(mat).name() << std::endl << mat << std::endl;

    // Matrici create con vari tipi di costruttori e/o assegnamenti + assert dimensioni
    Matrice3D<doubleCustom, doubleCustomEquals> gi(1, 2, 2);
    assert(gi.get_depth() == 1);
    assert(gi.get_cols() == 2);
    assert(gi.get_rows() == 2);

    doubleCustom d1; // = 0
    doubleCustom d2(2.0);
    doubleCustom d3(3.0);

    gi(0, 0, 0) = d3;
    gi(0, 0, 1) = d2;

    Matrice3D<doubleCustom, doubleCustomEquals> gi2(gi);

    Matrice3D<doubleCustom, doubleCustomEquals> gi3 = gi;

    Matrice3D<doubleCustom, doubleCustomEquals> gigi(1, 2, 2);
    
    gigi(0, 0, 0) = d3;
    gigi(0, 0, 1) = d2;

    std::cout << "Matrice 1 creata (tipo doubleCustom): " << typeid(gi).name() << std::endl << gi << std::endl;

    std::cout << "Matrice 2 creata per testare uguaglianza: " << typeid(gigi).name() << std::endl << gigi << std::endl;

    // assert per verificare l'uguaglianza tra matrici
    assert(gi == gigi);
    assert(gi2 == gi);
    assert(gi3 == gi);

    // stesso processo di assegnamento con matrici di tipo custom diverso
    Matrice3D<userCustom, userCustomEquals> user(1, 2, 2);
    assert(user.get_depth() == 1);
    assert(user.get_cols() == 2);
    assert(user.get_rows() == 2);

    userCustom u1("Mario", 20);
    userCustom u2("Luigi", 21);
    userCustom u3("Pippo", 22);
    userCustom u4("Pluto", 23);

    user(0, 0, 0) = u1;
    user(0, 0, 1) = u2;
    user(0, 1, 0) = u3;
    user(0, 1, 1) = u4;

    Matrice3D<userCustom, userCustomEquals> user2(user);

    Matrice3D<userCustom, userCustomEquals> user3 = user;

    Matrice3D<userCustom, userCustomEquals> useruser(1, 2, 2);

    useruser(0, 0, 0) = u1;
    useruser(0, 0, 1) = u2;
    useruser(0, 1, 0) = u3;
    useruser(0, 1, 1) = u4;

    // non stampa (consente solo char)
    // std::cout << "Matrice 1 creata con tipo custom: " << typeid(user).name() << std::endl << user << std::endl;

    // std::cout << "Matrice 2 creata per testare uguaglianza: " << typeid(useruser).name() << std::endl << useruser << std::endl;

    // debug (son dello stesso tipo)
    std::cout << "Creata matrice userCustom 1 (user): " << typeid(user).name() << std::endl;
    
    std::cout << "Creata matrice userCustom 2 (useruser): " << typeid(useruser).name() << std::endl;

    //assert per verificare l'uguaglianza tra matrici
    assert(user == useruser);   // FAIL solo se non inizializzato completamente (riempi tutta la matrice)
    assert(user2 == user);
    assert(user3 == user);
}


/**
 * @brief Test di conversione da U a T
*/
void test1() {
    std::cout << "----- TEST PUNTO 1 (conversione da U a T) -----" << std::endl;

    Matrice3D<int, intEquals> mat(2, 2, 3);
    mat.initialize_matrix(98.2);    // tronca a 98

    Matrice3D<doubleCustom, doubleCustomEquals> mat_converted(mat.convert<doubleCustom, doubleCustomEquals>());

    std::cout << "Matrice base creata (tipo int): " << typeid(mat).name() << std::endl << mat << std::endl;

    std::cout << "Matrice convertita (tipo doubleCustom): " << typeid(mat_converted).name() << std::endl << mat_converted << std::endl << std::endl;

    Matrice3D<char, charEquals> mat_char_converted = mat_converted.convert<char, charEquals>();

    std::cout << "Matrice convertita (tipo char da tipo doubleCustom): " << typeid(mat_char_converted).name() << std::endl << mat_char_converted << std::endl << std::endl;

    assert(mat(0, 0, 0) == 98);
    assert(mat_converted(0, 0, 0) == doubleCustom(98));
    assert(mat_char_converted(0, 0, 0) == char(98));
}


/**
 * @brief Test di assegnamento con operator()
*/
void test2() {
    std::cout << "----- TEST PUNTO 2 (operator()) -----" << std::endl;

    Matrice3D<int, intEquals> mat(3, 2, 2);
    mat.initialize_matrix(14);

    std::cout << "Matrice base creata (tipo int): " << typeid(mat).name() << std::endl << mat << std::endl;

    mat(0, 0, 0) = 99;
    mat(0, 0, 1) = mat(2, 1, 1);
    mat(0, 1, 1) = mat(1, 1, 1);  
    mat(0, 1, 0) = mat(0, 0, 1);
    mat(1, 1, 1) = mat(0, 1, 1);
    mat(2, 0, 0) = mat(0, 0, 1);

    std::cout << "Matrice modificata con assegnamento (): " << typeid(mat).name() << std::endl << mat << std::endl;
}


/**
 * @brief Test di assegnamento con iteratori
*/
void test3() {
    std::cout << "----- TEST PUNTO 3 (iteratori) -----" << std::endl;
    
    Matrice3D<int, intEquals> mat(3, 2, 2);
    
    typename Matrice3D<int, intEquals>::iterator begin_wr = mat.begin();
    typename Matrice3D<int, intEquals>::iterator end_wr = mat.end();

    for (int i = 0; begin_wr != end_wr; ++begin_wr, ++i) {
        *begin_wr = i;
    }

    std::cout << "Matrice creata assegnando i valori con iteratori: " << typeid(mat).name() << std::endl << mat << std::endl;
}


/**
 * @brief Test di slice()
*/
void test4() {
    std::cout << "----- TEST PUNTO 4 (slice) -----" << std::endl;
    
    Matrice3D<int, intEquals> mat(3, 5, 4);
    mat.initialize_matrix(1);

    Matrice3D<int, intEquals> sliced_mat = mat.slice(1, 3, 1, 4, 1, 3);

    std::cout << "Matrice base creata (tipo int): " << typeid(mat).name() << std::endl << mat << std::endl;

    std::cout << "Matrice slice creata (tipo int): " << typeid(sliced_mat).name() << std::endl << sliced_mat << std::endl;
    
    Matrice3D<doubleCustom, doubleCustomEquals> mat_double(2, 4, 4);
    mat_double.initialize_matrix(15);

    Matrice3D<doubleCustom, doubleCustomEquals> sliced_mat_double = mat_double.slice(1, 2, 1, 3, 1, 4);

    std::cout << "Matrice base creata (tipo doubleCustom): " << typeid(mat_double).name() << std::endl << mat_double << std::endl;

    std::cout << "Matrice slice creata (tipo doubleCustom): " << typeid(sliced_mat_double).name() << std::endl << sliced_mat_double << std::endl;
}


/**
 * @brief Test di fill()
*/
void test5() {
    std::cout << "----- TEST PUNTO 5 (fill) -----" << std::endl;
    
    Matrice3D<int, intEquals> mat(3, 3, 3);
    mat.initialize_matrix(99);

    Matrice3D<int, intEquals> mat2(2, 2, 4);
    mat2.initialize_matrix(6);

    std::cout << "Matrice base creata (tipo int): " << typeid(mat).name() << std::endl << mat << std::endl;

    std::cout << "Matrice 2 con cui fillare (tipo int): " << typeid(mat2).name() << std::endl << mat2 << std::endl;

    mat.fill(mat2.begin(), mat2.end());

    std::cout << "Matrice fill creata (tipo int): " << typeid(mat).name() << std::endl << mat << std::endl;
}


/**
 * @brief Funtore di trasformazione per la funzione trasform
*/
struct PerDue {
        int operator()(int x) {
            return x * 2;
        }
    };


/**
 * @brief Funtore di trasformazione per la funzione trasform
*/
struct DivisoDue {
        int operator()(int x) {
            return x / 2;
        }
    };


/**
 * @brief Funtore di trasformazione per la funzione trasform
*/
struct SommaDue {
        int operator()(int x) {
            return x + 2;
        }
    };


/**
 * @brief Funtore di trasformazione per la funzione trasform
*/
struct ToChar {
        char operator()(int x) {
            return static_cast<char>(x);
        }
    };


/**
 * @brief Test di trasform()
*/
void test6() {
    std::cout << "----- TEST PUNTO 6 (trasform) -----" << std::endl;
    
    Matrice3D<int, intEquals> mat(3, 3, 3);
    mat.initialize_matrix(1);

    std::cout << "Matrice base creata (tipo int): " << typeid(mat).name() << std::endl << mat << std::endl;

    Matrice3D<int, intEquals> mat2 = trasform<int, intEquals, int, intEquals>(mat, PerDue());

    std::cout << "Matrice trasform creata (tipo int e functor PerDue): " << typeid(mat2).name() << std::endl << mat2 << std::endl;
    
    Matrice3D<int, intEquals> mat3 = trasform<int, intEquals, int, intEquals>(mat, DivisoDue());

    std::cout << "Matrice trasform creata (tipo int e functor DivisoDue): " << typeid(mat3).name() << std::endl << mat3 << std::endl;

    Matrice3D<int, intEquals> mat4 = trasform<int, intEquals, int, intEquals>(mat, SommaDue());

    std::cout << "Matrice trasform creata (tipo int e functor SommaDue): " << typeid(mat4).name() << std::endl << mat4 << std::endl;

    Matrice3D<int, intEquals> mat_int_to_char(3, 3, 3);
    mat_int_to_char.initialize_matrix(65);
    
    Matrice3D<char, charEquals> mat5 = trasform<char, charEquals, int, intEquals>(mat_int_to_char, ToChar());

    std::cout << "Matrice trasform creata (tipo char e functor ToChar): " << typeid(mat5).name() << std::endl << mat5 << std::endl;
}


int main() {

    testCreazioneAssegnamentoMatrice();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}