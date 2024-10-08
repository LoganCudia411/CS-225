/**
 * @file dhhashtable.cpp
 * Implementation of the DHHashTable class.
 */

#include "dhhashtable.h"

template <class K, class V>
DHHashTable<K, V>::DHHashTable(size_t tsize)
{
    if (tsize <= 0)
        tsize = 17;
    size = findPrime(tsize);
    table = new std::pair<K, V>*[size];
    should_probe = new bool[size];
    for (size_t i = 0; i < size; i++) {
        table[i] = NULL;
        should_probe[i] = false;
    }
    elems = 0;
}

template <class K, class V>
DHHashTable<K, V>::~DHHashTable()
{
    destroy();
}

template <class K, class V>
DHHashTable<K, V> const& DHHashTable<K, V>::operator=(DHHashTable const& rhs)
{
    if (this != &rhs) {
        destroy();

        copy(rhs);
    }
    return *this;
}

template <class K, class V>
DHHashTable<K, V>::DHHashTable(DHHashTable<K, V> const& other)
{
    copy(other);
}


template <class K, class V>
void DHHashTable<K, V>::destroy()
{
    for (size_t i = 0; i < size; i++)
        delete table[i];
    delete[] table; table = nullptr;
    delete[] should_probe; should_probe = nullptr;
}

template <class K, class V>
void DHHashTable<K, V>::copy(const DHHashTable<K, V>& other)
{
    table = new std::pair<K, V>*[other.size];
    should_probe = new bool[other.size];
    for (size_t i = 0; i < other.size; i++) {
        should_probe[i] = other.should_probe[i];
        if (other.table[i] == NULL)
            table[i] = NULL;
        else
            table[i] = new std::pair<K, V>(*(other.table[i]));
    }
    size = other.size;
    elems = other.elems;
}

template <class K, class V>
void DHHashTable<K, V>::insert(K const& key, V const& value)
{

    /**
     * @todo Implement this function.
     *
     * @note Remember to resize the table when necessary (load factor >=
     *  0.7). **Do this check *after* increasing elems!!** Also, don't
     *  forget to mark the cell for probing with should_probe!
     */

    (void) key;   // prevent warnings... When you implement this function, remove this line.
    (void) value; // prevent warnings... When you implement this function, remove this line.
    elems++;
    if(shouldResize()){
        resizeTable(); 
    }
    
    size_t cur = hashes::hash(key, size);
    size_t next = hashes::secondary_hash(key, size);
    size_t index = cur;
    size_t i = 0; 

    while (should_probe[index]){
        ++i;
        index = (cur + next*i) % size;
    }


    if(table[index] == NULL)
        table[index] = new std::pair<K,V>(key,value);
    else{
       table[index]->first = key;
       table[index]->second = value;
    }
    should_probe[index] = true;
}

template <class K, class V>
void DHHashTable<K, V>::remove(K const& key)
{
    /**
     * @todo Implement this function
     */

     auto index = findIndex(key);
     if( index == -1){
        return; 
     }
     should_probe[index] = false; 
}

template <class K, class V>
int DHHashTable<K, V>::findIndex(const K& key) const
{
    /**
     * @todo Implement this function
     */
      size_t i = 0;
    size_t next = hashes::secondary_hash(key, size);
    size_t cur= hashes::hash(key, size);
    size_t index = cur;
    do{
    if(should_probe[index])
    if(table[index]->first == key)
    return index;
            ++i;
            index = (cur + next * i) % size;
        }
    while(index != cur);
    return -1;
}

template <class K, class V>
V DHHashTable<K, V>::find(K const& key) const
{
    int index = findIndex(key);
    if (index != -1)
        return table[index]->second;
    return V();
}

template <class K, class V>
V& DHHashTable<K, V>::operator[](K const& key)
{
    // First, attempt to find the key and return its value by reference
    int index = findIndex(key);
    if (index == -1) {
        // otherwise, insert the default value and return it
        insert(key, V());
        index = findIndex(key);
    }
    return table[index]->second;
}

template <class K, class V>
bool DHHashTable<K, V>::keyExists(K const& key) const
{
    return findIndex(key) != -1;
}

template <class K, class V>
void DHHashTable<K, V>::clear()
{
    destroy();

    table = new std::pair<K, V>*[17];
    should_probe = new bool[17];
    for (size_t i = 0; i < 17; i++)
        should_probe[i] = false;
    size = 17;
    elems = 0;
}

template <class K, class V>
void DHHashTable<K, V>::resizeTable()
{
    size_t newSize = findPrime(size * 2);
    std::pair<K, V>** temp = new std::pair<K, V>*[newSize];
    delete[] should_probe;
    should_probe = new bool[newSize];
    for (size_t i = 0; i < newSize; i++) {
        temp[i] = NULL;
        should_probe[i] = false;
    }

    for (size_t slot = 0; slot < size; slot++) {
        if (table[slot] != NULL) {
            size_t h = hashes::hash(table[slot]->first, newSize);
            size_t jump = hashes::secondary_hash(table[slot]->first, newSize);
            size_t i = 0;
            size_t index = h; 
            while (temp[index] != NULL)
            {
                ++i;
                index = (h + jump*i) % newSize;
            }
            temp[index] = table[slot];
            should_probe[index] = true;
        }
    }

    delete[] table;
    // don't delete elements since we just moved their pointers around
    table = temp;
    size = newSize;
}
