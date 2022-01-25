//
// Created by Я on 14.11.2021.
//

#ifndef TRITS_TRIT_H
#define TRITS_TRIT_H
#include <unordered_map>

enum Trit{
    UNKNOWN,
    FALSE,
    TRUE = 3
};

class TritSet {
private:
    unsigned int *arr;
    unsigned int capacity = 0;
    class ProxyTrit{
    private:
        TritSet* trit_set;
        unsigned int index;
        bool out_of_set;
    public:
        void setProxyIndex(unsigned int _ind);
        const TritSet* getTritSet()  {return trit_set;};
        unsigned int getIndex() const {return index;};
        ProxyTrit(TritSet* set, unsigned int index_, bool set_state);
        void operator=(Trit value);
        bool operator==(Trit value) const;
        bool operator==(const ProxyTrit & other) const;
        bool operator!=(Trit value) const;
        bool operator!=(const ProxyTrit & other) const;
        void operator=(const ProxyTrit & other);
        Trit operator~() const;
        Trit operator&(const ProxyTrit & other) const;
        Trit operator&(Trit value) const;
        Trit operator|(const ProxyTrit & other) const;
        Trit operator|(Trit value)   const;
    };
public:
    ~TritSet();
    TritSet(unsigned int trit_count);
    TritSet(const TritSet &other);
    Trit operator[](unsigned int index) const;
    bool operator==(const TritSet & set) const;
    TritSet operator&(const TritSet & set);
    TritSet operator|(const TritSet & set);
    TritSet operator~();
    std::unordered_map< Trit, int, std::hash<int> > cardinality();
    unsigned int get_capacity() const;
    unsigned get_length();
    void trim(unsigned last_index);
    unsigned int cardinality(Trit value);
    void change_size(unsigned int new_capacity);

    ProxyTrit operator[](unsigned int index);
    void shrink();
    class iterator{
    private:
        ProxyTrit proxy;
    public:
        iterator(ProxyTrit _proxy);
        bool operator!=(iterator it);
        bool operator==(iterator it);
        bool operator!=(Trit value);
        bool operator==(Trit value);
        iterator* operator*(){return this;};
        void operator++(int);
        void operator++();
        void operator--(int);
        ProxyTrit operator&(){return this->proxy;};
    };
    iterator begin();
    iterator end();
};


#endif //TRITS_TRIT_H
