//
// Created by Я on 14.11.2021.
//
#include "trit.h"
#include <cmath>
#include <algorithm>

TritSet::TritSet(unsigned int trit_count) {
    capacity = trit_count;
    unsigned int uint_count = trit_count*2/(8*sizeof(unsigned int));
    if (((trit_count * 2) % (8 * sizeof(unsigned int))) != 0) uint_count++;
    arr = (unsigned int*) malloc(sizeof(unsigned int) * uint_count);
    std::fill(arr,arr+uint_count,0);
}

TritSet::ProxyTrit  TritSet::operator[](unsigned int index) {
    if (index <= (this->capacity - 1)){
            TritSet::ProxyTrit proxy(this, index, false);
            return proxy;
    }else{
        TritSet::ProxyTrit proxy(this, index, true);
        return proxy;
    }
}

void TritSet::change_size(unsigned int new_capacity) {
    unsigned int uint_count = this->capacity*2/(8*sizeof(unsigned int));
    if (((this->capacity * 2) % (8 * sizeof(unsigned int))) != 0) uint_count++;
    unsigned int new_uint_count = new_capacity*2/(8*sizeof(unsigned int));
    if (((new_capacity * 2) % (8 * sizeof(unsigned int))) != 0) new_uint_count++;
    if (new_uint_count != uint_count) {
        this->arr = (unsigned int *) realloc(this->arr, sizeof(unsigned int) * new_uint_count);
        if (new_uint_count > uint_count) {
            std::fill(this->arr + uint_count + 1, this->arr + new_uint_count, 0);
        }
    }
    this->capacity = new_capacity;
}

TritSet::iterator TritSet::begin() {
    ProxyTrit begin(this, 0, false);
    return {begin};
}

TritSet::iterator TritSet::end() {
    ProxyTrit end(this, capacity , false);
    return {end};
}

void TritSet::shrink() {
    int last_valuable_index = 0;
    int i = 0;
    for (iterator it = this->begin(); it != this->end(); it.operator++(), i++){
        if (it != UNKNOWN){
            last_valuable_index = i;
        }
    }
    this->change_size(last_valuable_index + 1);
}

TritSet TritSet::operator&(TritSet &other){
    unsigned int new_capacity;
    if (this->capacity > other.capacity){
        new_capacity = this->capacity;
    }else{
        new_capacity = other.capacity;
    }
    TritSet new_set(new_capacity);
    for (int i = 0; i < new_capacity - 1; i++){
        new_set[i] = (*this)[i] & other[i];
    }
    return new_set;
}

TritSet TritSet::operator|(TritSet &other) {
    unsigned int new_capacity;
    if (this->capacity > other.capacity){
        new_capacity = this->capacity;
    }else{
        new_capacity = other.capacity;
    }
    TritSet new_set(new_capacity);
    for (int i = 0; i < new_capacity - 1; i++){
        new_set[i] = (*this)[i] | other[i];
    }
    return new_set;
}

TritSet TritSet::operator~() {
    TritSet new_set(this->capacity);
    int i = 0;
    for (iterator it = new_set.begin(); it != new_set.end(); it.operator++(), i++){
        new_set[i] = ~(*this)[i];
    }
    return new_set;
}

unsigned int TritSet::cardinality(Trit value) {
    int num = 0;
    for (iterator it = this->begin(); it != this->end(); it.operator++()){
        if (it == value){
            num++;
        }
    }
    return num;
}

void TritSet::trim(unsigned int last_index) {
    int i = 0;
    for (iterator it = this->begin(); it != this->end(); it.operator++(), i++){
        if (i >= last_index){
            (*this)[i] = UNKNOWN;
        }
    }
}

unsigned TritSet::get_length() {
    unsigned int last_valuable_index = 0;
    int i = 0;
    for (iterator it = this->begin(); it != this->end(); it.operator++(), i++){
        if (it != UNKNOWN){
            last_valuable_index = i;
        }
    }
    return last_valuable_index + 1;
}

std::unordered_map<Trit, int, std::hash<int>> TritSet::cardinality() {
    std::unordered_map<Trit, int, std::hash<int>> map;
    int i = 0;
    for (iterator it = this->begin(); it != this->end(); it.operator++(), i++){
        if ((*this)[i] == TRUE){
            map[TRUE]++;
        }else if((*this)[i] == FALSE){
            map[FALSE]++;
        }else{
            map[UNKNOWN]++;
        }
    }
    return map;
}

unsigned int TritSet::get_capacity() const {
    return this->capacity;
}

TritSet::ProxyTrit::ProxyTrit(TritSet* set, unsigned int index_, bool set_state) {
    trit_set = set;
    index = index_;
    out_of_set = set_state;
}

void TritSet::ProxyTrit::operator=(Trit value) {
    unsigned int uint_number = (this->index + 1) * 2 / (8 * sizeof(unsigned int));
    if ((((this->index+1) * 2) % (8 * sizeof(unsigned int))) != 0) uint_number++;
    unsigned int trit_index = (this->index + 1) % ((uint_number) * sizeof(unsigned int) * 4);
    unsigned int current_trit = -1;
    current_trit -= TRUE * (unsigned int) pow(2, (trit_index-1) * 2);
    if (!this->out_of_set) {
        this->trit_set->arr[uint_number - 1] = this->trit_set->arr[uint_number - 1] & current_trit;
        if (value == TRUE) {
            this->trit_set->arr[uint_number - 1] = this->trit_set->arr[uint_number - 1] | (TRUE << (trit_index - 1) * 2);
        } else if (value == FALSE) {
            this->trit_set->arr[uint_number - 1] = this->trit_set->arr[uint_number - 1] | (FALSE << (trit_index - 1) * 2);
        } else {
            this->trit_set->arr[uint_number - 1] = this->trit_set->arr[uint_number - 1] | (UNKNOWN << (trit_index - 1) * 2);
        }
    }else{
        if ((value == FALSE) or (value == TRUE)){
            this->trit_set->change_size(this->index + 1);
            this->trit_set->arr[uint_number - 1] = this->trit_set->arr[uint_number - 1] & current_trit;
            if (value == TRUE) {
                this->trit_set->arr[uint_number - 1] = this->trit_set->arr[uint_number - 1] | (TRUE << (trit_index - 1) * 2);
            }else{
                this->trit_set->arr[uint_number - 1] = this->trit_set->arr[uint_number - 1] | (FALSE << (trit_index - 1) * 2);
            }
        }
    }
}

bool TritSet::ProxyTrit::operator==(Trit value) const{
    if (!this->out_of_set){
        unsigned int uint_number = (this->index + 1) * 2 / (8 * sizeof(unsigned int));
        if ((((this->index+1) * 2) % (8 * sizeof(unsigned int))) != 0) uint_number++;
        unsigned int trit_index = (this->index + 1) % ((uint_number) * sizeof(unsigned int) * 4);
        unsigned int trit_value = TRUE;
        trit_value = trit_value & (this->trit_set->arr[uint_number - 1] >> (trit_index - 1) * 2);
        if (trit_value == value) {
            return true;
        }
        return false;
    }else{
        if (value == UNKNOWN){
            return true;
        }
        return false;
    }
}

bool TritSet::ProxyTrit::operator==(const TritSet::ProxyTrit & other) const{
    if ((*this == TRUE) && (other == TRUE)){
        return true;
    }else if((*this == FALSE) && (other == FALSE)){
        return true;
    }else if ((*this == UNKNOWN) && (other == UNKNOWN)){
        return true;
    }
    return false;
}

void TritSet::ProxyTrit::operator=(const ProxyTrit & other) {
    if (other == TRUE){
        *this = TRUE;
    }else if(other == FALSE){
        *this = FALSE;
    }else{
        *this = UNKNOWN;
    }
}

Trit TritSet::ProxyTrit::operator~()const {
    if (*this == TRUE){
        return FALSE;
    }else if (*this == FALSE){
        return TRUE;
    }else return UNKNOWN;
}

Trit TritSet::ProxyTrit::operator&(const ProxyTrit & other) const {
    if ((*this == FALSE) || (other == FALSE)){
        return FALSE;
    }else if ((*this == UNKNOWN) || (other == UNKNOWN)){
        return UNKNOWN;
    }else return TRUE;
}

bool TritSet::ProxyTrit::operator!=(Trit value) const{
    if (*this == value){
        return false;
    }
    return true;
}

bool TritSet::ProxyTrit::operator!=(const TritSet::ProxyTrit & other)const{
    if (*this == other){
        return false;
    }
    return true;
}

Trit TritSet::ProxyTrit::operator&(Trit value) const{
    if ((*this == FALSE) || (value == FALSE)){
        return FALSE;
    }else if ((*this == UNKNOWN) || (value == UNKNOWN)){
        return UNKNOWN;
    }else return TRUE;
}

Trit TritSet::ProxyTrit::operator|(const TritSet::ProxyTrit & other) const {
    if ((*this == TRUE) || (other == TRUE)){
        return TRUE;
    }else if ((*this == UNKNOWN) || (other == UNKNOWN)){
        return UNKNOWN;
    }else return FALSE;
}

Trit TritSet::ProxyTrit::operator|(Trit value) const {
    if ((*this == TRUE) || (value == TRUE)){
        return TRUE;
    }else if ((*this == UNKNOWN) || (value == UNKNOWN)){
        return UNKNOWN;
    }else return FALSE;
}

void TritSet::ProxyTrit::setProxyIndex(unsigned int _ind) {
    index = _ind;
}


TritSet::iterator::iterator(TritSet::ProxyTrit _proxy) : proxy(_proxy) {
    proxy = _proxy;
}

bool TritSet::iterator::operator!=(TritSet::iterator it) {
    if ((this->proxy.getTritSet() == it.proxy.getTritSet()) && (this->proxy.getIndex() == it.proxy.getIndex())){
        return false;
    }else {
        return true;
    }
}

void TritSet::iterator::operator++() {
    this->proxy.setProxyIndex(this->proxy.getIndex()+1);
}

bool TritSet::iterator::operator==(Trit value) {
    if (this->proxy == value){
        return true;
    }else{
        return false;
    }
}

bool TritSet::iterator::operator==(TritSet::iterator it) {
    return !(*this != it);
}

bool TritSet::iterator::operator!=(Trit value) {
    return !(this->proxy == value);
}

void TritSet::iterator::operator--() {
    this->proxy.setProxyIndex(this->proxy.getIndex()-1);
}
