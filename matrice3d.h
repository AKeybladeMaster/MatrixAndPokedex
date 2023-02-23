#ifndef MATRICE3D_H
#define MATRICE3D_H

///////////////////////// BEGIN MATRICE3D CLASS //////////////////////////

// template <typename T, typename Equals>
template <typename T, typename Equals>
class Matrice3D {

private:
    
    // Profondità, righe e colonne della matrice
    unsigned int _depth, _rows, _cols;

    // funtore di confronto
    Equals _equals;

    // Puntatore alla matrice
    T *_matrix;

public:


    /**
     * @brief Costruttore di default
    */
    Matrice3D() : _matrix(nullptr), _depth(0), _rows(0), _cols(0) {
        // throw std::length_error("Errore: dimensioni matrice non valide, creata matrice vuota");
    }

    
    /**
     * @brief Costruttore con parametri
     * 
     * @param depth profondità della matrice
     * @param rows numero di righe della matrice
     * @param cols numero di colonne della matrice
    */
    Matrice3D(unsigned int depth, unsigned int rows, unsigned int cols) {
        try {
            if (depth == 0 || rows == 0 || cols == 0)
            throw std::length_error("Errore: dimensioni matrice non valide. Inizializzazione fallita");
            _depth = depth;
            _rows = rows;
            _cols = cols;
            _matrix = new T[_depth * _rows * _cols];
        }
        catch (std::bad_alloc &e) {
            std::cerr << "Errore main construct: " << e.what() << std::endl;
            _matrix = nullptr;
            throw;
        }
    }

    
    /**
     * @brief Copy constructor
     * 
     * @param other matrice da copiare
     * 
     * @return Matrice3D& reference alla matrice copiata
    */
    Matrice3D(const Matrice3D<T, Equals> &other) {
        try {
            _depth = other._depth;
            _rows = other._rows;
            _cols = other._cols;
            _matrix = new T[_depth * _rows * _cols];
            for (unsigned int i = 0; i < _depth * _rows * _cols; i++)
                _matrix[i] = other._matrix[i];
        }
        catch (std::bad_alloc &e) {
            std::cerr << "Errore copy construct: " << e.what() << std::endl;
            delete[] _matrix;
            _matrix = nullptr;
            throw;
        }
    }

    
    /**
     * @brief Distruttore
    */
    ~Matrice3D() {
        delete[] _matrix;
        _matrix = nullptr;
        // std::cout << "Distruttore chiamato" << std::endl;
    }

    /**
     * @brief Overloading dell'operatore di assegnazione
     * 
     * @param other matrice da copiare
     * 
     * @return Matrice3D& reference alla matrice copiata
    */
     Matrice3D &operator=(const Matrice3D<T, Equals> &other) {
        if (this != &other) {
            Matrice3D tmp(other);
            this -> _matrix = tmp._matrix;
            std::swap(this -> _matrix, tmp._matrix);
            std::swap(this -> _depth, tmp._depth);
            std::swap(this -> _rows, tmp._rows);
            std::swap(this -> _cols, tmp._cols);
        }
        return *this;
    }


    /**
     * Operatore di accesso in lettura
     * 
     * @param depth indice di profondita'
     * @param row indice della riga
     * @param col indice della colonna
     * 
     * @return reference all'elemento della matrice dati i parametri
     * 
     * @throw std::out_of_range se gli indici sono fuori range
    */
    T &operator()(unsigned int depth, unsigned int row, unsigned int col) {
        // try {
            if (depth >= _depth || row >= _rows || col >= _cols || depth < 0 || row < 0 || col < 0) {
                throw std::out_of_range("Errore operator(): indice fuori range");
            }
        // }
        // catch (std::out_of_range &e) {
        //     std::cerr << e.what() << std::endl;
        // }
        return _matrix[depth * _rows * _cols + row * _cols + col];
    }


    /**
     * Operatore di accesso in lettura (const)
     * 
     * @param depth indice di profondita'
     * @param row indice della riga
     * @param col indice della colonna
     * 
     * @return reference all'elemento della matrice dati i parametri
     * 
     * @throw std::out_of_range se gli indici sono fuori range
    */
    const T &operator()(unsigned int depth, unsigned int row, unsigned int col) const {
            if (depth >= _depth || row >= _rows || col >= _cols || depth < 0 || row < 0 || col < 0) {
                throw std::out_of_range("Errore operator() const: indice fuori range");
            }
        return _matrix[depth * _rows * _cols + row * _cols + col];
    }

    
    /**
     * @brief Overloading dell'operatore di uguaglianza
     * 
     * @param other matrice da confrontare
     * 
     * @return true se le matrici sono uguali
     * @return false se le matrici sono diverse
    */
        bool operator==(const Matrice3D &other) const {
            Matrice3D<T, Equals> tmp(other);
            if (_depth != tmp._depth || _rows != tmp._rows || _cols != tmp._cols) {
                throw std::length_error("Errore operator==: dimensioni diverse");
            }
            for (unsigned int i = 0; i < _depth * _rows * _cols; i++)
                if (!(_equals (_matrix[i], tmp._matrix[i])))
                    return false;
            return true;
        }
    

    /**
     * @brief Overloading dell'operatore di output
     * 
     * @param os stream di output
     * @param m matrice da stampare
     * 
     * @return std::ostream& stream di output
    */
    friend std::ostream &operator<<(std::ostream &os, const Matrice3D &m) {
        os << "DEPTH: " << m._depth << " RIGHE: " << m._rows << " COLONNE: " << m._cols << std::endl;
        for (unsigned int i = 0; i < m._depth; i++) {
            for (unsigned int j = 0; j < m._rows; j++) {
                for (unsigned int k = 0; k < m._cols; k++)
                    os << m._matrix[i * m._rows * m._cols + j * m._cols + k] << " ";
                    // os << m(i, j, k) << " ";
                os << std::endl;
            }
            os << std::endl;
        }
        return os;
    }

    // metodo per inizializzare le celle della matrice con un valore T
    void initialize_matrix(const T &value) {
        for (unsigned int i = 0; i < _depth * _rows * _cols; i++)
            _matrix[i] = value;
    }

    // getters
    unsigned int get_depth() const { return _depth; }
    unsigned int get_rows() const { return _rows; }
    unsigned int get_cols() const { return _cols; }


    /**
     * @brief che converte una matrice 3D di tipo T in una matrice 3D di tipo U
     * 
     * @tparam U tipo di dato della matrice convertita
     * 
     * @return Matrice3D<U> matrice convertita
     * 
     * @throw std::length_error se le dimensioni delle matrici sono diverse
    */
    template <typename U, typename UEquals>
    Matrice3D<U, UEquals> convert() {
        Matrice3D<U, UEquals> mat_converted(_depth, _rows, _cols);
        for (int i = 0; i < _depth; ++i) {
            for (int j = 0; j < _rows; ++j) {
                for (int k = 0; k < _cols; ++k) {
                    try {
                        mat_converted(i, j, k) = static_cast<U>((*this)(i, j, k));
                    }
                     catch (...) {
                        throw;
                    }
                }
            }
        }
        return mat_converted;
    }

    
    /**
     * @brief che ritorna una sotto-matrice tagliata di una matrice 3D
     * 
     * @param z1 indice di profondita' iniziale
     * @param z2 indice di profondita' finale
     * @param y1 indice della riga iniziale
     * @param y2 indice della riga finale
     * @param x1 indice della colonna iniziale
     * @param x2 indice della colonna finale
     * 
     * @return sotto-matrice tagliata
     * 
     * @throw std::out_of_range se gli indici sono fuori range
    */
    Matrice3D<T, Equals> slice(unsigned int z1, unsigned int z2, unsigned int y1, unsigned int y2,  unsigned int x1, unsigned int x2) {
        int z = z2 - z1;
        int y = y2 - y1;
        int x = x2 - x1;

        if(z < 0 || y < 0 || x < 0 || z > _depth || y > _rows || x > _cols) {
            throw std::out_of_range("Errore slice(): indice fuori range");
        }

        Matrice3D<T, Equals> submatrix(z, y, x);
        for (int i = 0; i < z; i++) {
            for (int j = 0; j < y; j++) {
                for (int k = 0; k < x; k++) {
                    submatrix(i, j, k) = (*this)(z1 + i, y1 + j, x1 + k);
                }
            }
        }
        return submatrix;
    }


    /**
     * @brief metodo che riempie la matrice con gli elementi di un iteratore
     * puntante ad un altra matrice
     * 
     * @tparam Iter tipo di iteratore
     * 
     * @param first iteratore all'inizio della matrice
     * @param last iteratore alla fine della matrice 
     * 
     * @throw std::length_error se la matrice è vuota o se la matrice è piu' grande
    */
    template <typename Iter>
    void fill(Iter first, Iter last) {
        // int i = 0;
        if (_matrix == nullptr || (_depth == 0 && _rows == 0 && _cols == 0) || (last - first) > (_depth * _rows * _cols))
            throw std::length_error("Errore fill(): matrice con dimensioni non valide o vuota");
        for (unsigned int i = 0; first != last; ++first, ++i) {
            try {
                _matrix[i] = static_cast<T>(*first);
            }
            catch (std::length_error &e) {
                std::cerr << e.what() << std::endl;
                throw;
            }
        }
    }

////////////////////////// START ITERATOR CLASS //////////////////////////

// Solo se serve anche const_iterator aggiungere la seguente riga
class const_iterator; // forward declaration

class iterator {

    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef T*                              pointer;
        typedef T&                              reference;


        iterator() : mat_ptr(nullptr), _depth(0), _rows(0), _cols(0) {}

        iterator(const iterator &other) : mat_ptr(other.mat_ptr), _depth(other._depth), _rows(other._rows), _cols(other._cols) {}

        iterator &operator=(const iterator &other) {
            mat_ptr = other.mat_ptr;
            _depth = other._depth;
            _rows = other._rows;
            _cols = other._cols;
            return *this;
        }

        ~iterator() {}

        // ritorna il dato riferito dall'iteratore (dereferenziazione)
        reference operator*() const {
            return *mat_ptr;
        }

        // ritorna il puntatore al dato riferito dall'iteratore
        pointer operator->() const {
            return &mat_ptr;
        }

        // operatore di accesso random
        reference operator[](int index) {
            return *(mat_ptr + index);
            // return mat_ptr[index];
        }

        // operatore di iterazione post-incremento
        iterator operator++(int) {
            iterator tmp(*this, _depth, _rows, _cols);
            mat_ptr = mat_ptr + 1;
            return tmp;
        }

        // operatore di iterazione pre-incremento
        iterator &operator++() {
            mat_ptr = mat_ptr + 1;
            return *this;
        }

        // operatore di iterazione post-decremento
        iterator operator--(int) {
            iterator tmp(*this, _depth, _rows, _cols);
            mat_ptr = mat_ptr - 1;
            return tmp;
        }

        // operatore di iterazione pre-decremento
        iterator &operator--() {
            mat_ptr = mat_ptr - 1;
            return *this;
        }

        // spostamento in avanti della posizione
        iterator operator+(int offset) {
            return iterator(mat_ptr + offset); 
        }

        // spostamento indietro della posizione
        iterator operator-(int offset) {
            return iterator(mat_ptr - offset);
        }

        // spostamento in avanti della posizione
        iterator &operator+=(int offset) {
            mat_ptr = mat_ptr + offset;
            return *this;
        }

        // spostamento all'indietro della posizione
        iterator &operator-=(int offset) {
            mat_ptr = mat_ptr - offset;
            return *this;
        }

        // Numero di elementi tra due iteratori
		difference_type operator-(const iterator &other) {
            return (mat_ptr - other.mat_ptr);
		}

        // uguaglianza
        bool operator==(iterator &other) const {
            return (mat_ptr == other.mat_ptr);
        }

        // disuguaglianza
        bool operator!=(iterator &other) const {
            return (mat_ptr != other.mat_ptr);
        }

        // confronto
        bool operator<=(const iterator &other) const {
            return (mat_ptr <= other.mat_ptr);
        }

        bool operator>=(const iterator &other) const {
            return (mat_ptr >= other.mat_ptr);
        }

        bool operator<(const iterator &other) const {
            return (mat_ptr < other.mat_ptr);
        }

        bool operator>(const iterator &other) const {
            return (mat_ptr > other.mat_ptr);
        }
        
        // Solo se serve anche const_iterator aggiungere le seguenti definizioni
		friend class const_iterator;

        // Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (mat_ptr == other.mat_ptr);
		}

		// Diversita'
		bool operator!=(const const_iterator &other) const {
		    return (mat_ptr == other.mat_ptr);
		}

        // Confronto
        bool operator<(const const_iterator &other) const {
            return (mat_ptr < other.mat_ptr);
        }

        bool operator>(const const_iterator &other) const {
            return (mat_ptr > other.mat_ptr);
        }

        bool operator>= (const const_iterator &other) const {
            return (mat_ptr >= other.mat_ptr);
        }

        bool operator<= (const const_iterator &other) const {
            return (mat_ptr <= other.mat_ptr);
        }

    private:

        /**
         * @brief classe container con 'friend' dell'iteratore per poter
         * usare il costruttore di inizializzazione
        */
        friend class Matrice3D;

        /**
         * @brief Costruttore privato di inizializzazione usato dalla classe container
         *  tipicamente nei metodi begin e end
         * 
         * @param p puntatore (costante) al dato (Matrice3D) riferito dall'iteratore
         * @param depth profondita' della matrice
         * @param row numero di righe della matrice
         * @param col numero di colonne della matrice
         * 
        */
        iterator(T *p, unsigned int depth, unsigned int row, unsigned int col) : mat_ptr(p), _depth(depth), _rows(row), _cols(col) {}

        /**
         * @brief puntatore al dato (Matrice3D) riferito dall'iteratore
        */
        T *mat_ptr;

        /**
         * @brief dati per la gestione della matrice
        */
        unsigned int _depth, _rows, _cols;

};

////////////////////////// END ITERATOR CLASS ///////////////////////////

    /**
     * @brief ritorna l'iteratore all'inizio della sequenza dati
    */
    iterator begin() {
        return iterator(_matrix, _depth, _rows, _cols);
    }

    /**
     * @brief ritorna l'iteratore alla fine della sequenza dati
    */
    iterator end() {
        return iterator((_matrix + _depth * _rows * _cols), _depth, _rows, _cols);
    }

////////////////////// START CONST_ITERATOR CLASS ///////////////////////

class const_iterator {

    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;


        const_iterator() : mat_ptr(nullptr), _depth(0), _rows(0), _cols(0) {}

        const_iterator(const const_iterator &other) : mat_ptr(other.mat_ptr), _depth(other._depth), _rows(other._rows), _cols(other._cols) {}

        const_iterator &operator=(const const_iterator &other) {
            mat_ptr = other.mat_ptr;
            return *this;
        }

        ~const_iterator() {}

        // ritorna il dato riferito dall'iteratore (dereferenziazione)
        reference operator*() const {
            return *mat_ptr;
        }

        // ritorna il puntatore al dato riferito dall'iteratore
        pointer operator->() const {
            return &mat_ptr;
        }

        // operatore di accesso random
        reference operator[](int index) const {
            return *(mat_ptr + index);
            // return mat_ptr[index]; 
        }

        // operatore di iterazione post-incremento
        const_iterator operator++(int) {
            const_iterator tmp(*this, _depth, _rows, _cols);
            mat_ptr = mat_ptr + 1;
            return tmp;
        }

        // operatore di iterazione pre-incremento
        const_iterator &operator++() {
            mat_ptr = mat_ptr + 1;
            return *this;
        }

        // post-decremento
        const_iterator operator--(int) { 
            const_iterator tmp(*this, _depth, _rows, _cols);
            mat_ptr = mat_ptr - 1; 
            return tmp; 
        }

        // pre-decremento
        const_iterator &operator--() { 
            mat_ptr = mat_ptr - 1; 
            return *this; 
        }

        // Spostamento in avanti della posizione
        const_iterator operator+(int offset) const {
            return const_iterator(mat_ptr + offset); 
        }

        // Spostamento all'indietro della posizione
        const_iterator operator-(int offset) const { 
            return const_iterator(mat_ptr - offset); 
        }

        // Spostamento in avanti della posizione
        const_iterator &operator+=(int offset) { 
            mat_ptr += offset; 
            return *this;
        }

        // Spostamento all'indietro della posizione
        const_iterator &operator-=(int offset) {
            mat_ptr -= offset; 
            return *this; 
        }

        // Numero di elementi tra due iteratori
		difference_type operator-(const const_iterator &other) {
			return (mat_ptr - other.mat_ptr);
		}

        // uguaglianza
        bool operator==(const const_iterator &other) const {
            return (mat_ptr == other.mat_ptr);
        }

        // disuguaglianza
        bool operator!=(const const_iterator &other) const {
            return (mat_ptr != other.mat_ptr);
        }

        // confronto
        bool operator>(const const_iterator &other) const {
            return (mat_ptr > other.mat_ptr);
		}

        bool operator>=(const const_iterator &other) const {
            return (mat_ptr >= other.mat_ptr);
		}

        bool operator<(const const_iterator &other) const {
            return (mat_ptr < other.mat_ptr);
        }

        bool operator<=(const const_iterator &other) const {
            return (mat_ptr <= other.mat_ptr);
        }

        // Solo se serve anche iterator aggiungere le seguenti definizioni
		friend class iterator;

        // uguaglianza (w/ iterator)
        bool operator==(const iterator &other) const {
            return (mat_ptr == other.mat_ptr);
        }

        // disuguaglianza (w/ iterator)
        bool operator!=(const iterator &other) const {
            return (mat_ptr != other.mat_ptr);
        }

        // confronto (w/ iterator)
        bool operator>(const iterator &other) const {
            return (mat_ptr > other.mat_ptr);
        }

        bool operator>=(const iterator &other) const {
            return (mat_ptr >= other.mat_ptr);
        }

        bool operator<(const iterator &other) const {
            return (mat_ptr < other.mat_ptr);
        }

        bool operator<=(const iterator &other) const {
            return (mat_ptr <= other.mat_ptr);
        } 

        // costruttore di conversione iterator -> const_iterator
        const_iterator(const iterator &other) : mat_ptr(other.mat_ptr), _depth(other._depth), _rows(other._rows), _cols(other._cols) {}

        // assegnamento di un iterator ad un const_iterator
        // const_iterator &operator=(const iterator &other) const : _depth(other._depth), _rows(other._rows), _cols(other._cols) {
           const_iterator &operator=(const iterator &other) const {
            mat_ptr = other.mat_ptr;
            _depth = other._depth;
            _rows = other._rows;
            _cols = other._cols;    
            return *this;
        }

    private:
        /**
         * @brief classe container con 'friend' dell'iteratore per poter
         * usare il costruttore di inizializzazione
        */
        friend class Matrice3D;

        /**
         * @brief Costruttore privato di inizializzazione usato dalla classe container
         *  tipicamente nei metodi begin e end
         * 
         * @param p puntatore (costante) al dato (Matrice3D) riferito dall'iteratore
         * @param depth profondita' della matrice
         * @param row numero di righe della matrice
         * @param col numero di colonne della matrice
         * 
        */
        const_iterator(const T *p, unsigned int depth, unsigned int row, unsigned int col) : mat_ptr(p), _depth(depth), _rows(row), _cols(col) {}

        // puntatore al dato (Matrice3D) riferito dall'iteratore (const)
        const T *mat_ptr;

        // dati per la gestione della matrice
        unsigned int _depth, _rows, _cols;

};

/////////////////////// END CONST_ITERATOR CLASS ////////////////////////

    /**
     * @brief ritorna l'iteratore costante all'inizio della sequenza dati
    */
    const_iterator begin() const {
        return const_iterator(_matrix, _depth, _rows, _cols);
    }

    /**
     * @brief ritorna l'iteratore costante alla fine della sequenza dati
    */
    const_iterator end() const {
        return const_iterator((_matrix + _depth * _rows * _cols), _depth, _rows, _cols);
    }

};

////////////////////////// END MATRICE3D CLASS ///////////////////////////


/**
 * @brief metodo che trasforma una Matrice3D di tipo T a tipo Q usando un funtore F (con iteratori)
 * @tparam Q tipo della matrice di output
 * @tparam T tipo della matrice di input
 * @tparam F tipo del funtore
 * 
 * @param other_mat matrice di input
 * @param fun funtore di input
 * 
 * @return Matrice3D<Q> - matrice di output
 * 
 * @throw std::exception - eccezione lanciata se la matrice di input non e' valida
*/
    template <typename Q, typename Equals2, typename T, typename Equals, typename F>
    Matrice3D<Q, Equals2> trasform(const Matrice3D<T, Equals> &other_mat, F fun) {
        Matrice3D<Q, Equals2> mat_converted(other_mat.get_depth(), other_mat.get_rows(), other_mat.get_cols());
        // try {
        typename Matrice3D<T, Equals>::const_iterator begin = other_mat.begin();
        typename Matrice3D<T, Equals>::const_iterator end = other_mat.end();
        typename Matrice3D<Q, Equals2>::iterator begin_wr = mat_converted.begin();

        for (; begin != end; ++begin, ++begin_wr) {
            *begin_wr = fun(*begin);
        }
        return mat_converted;
    }

#endif