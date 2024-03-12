#ifndef hashmap_h
#define hashmap_h
#include <vector>
#include <list>
#include <string>

template <typename T>
class HashMap
{
public:
    //constructor
    HashMap(double max_load = 0.75) : m_nElements(0), m_maxLoad(max_load), buckets(10)
    {
        if(max_load <= 0) m_maxLoad = 0.75;
    }
    
    //TODO: IMPLEMENT
    // destructor; deletes all of the items in the hashmap
    ~HashMap(){
        for(int i = 0; i < buckets.size(); i++){
            for(typename std::list<Node*>::iterator it = buckets[i].begin(); it != buckets[i].end(); it++)
                delete (*it);
        }
    }
    
    // return the number of associations in the hashmap
    int size() const{
        return m_nElements;
    }
    
    // The insert method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void insert(const std::string& key, const T& value);
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value or modify it directly within the map.
     const T* find(const std::string& key) const;
    
    T* find(const std::string& key)
    {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
    // Defines the bracket operator for HashMap, so you can use your map like this:
    // your_map["david"] = 2.99;
    // If the key does not exist in the hashmap, this will create a new entry in
    // the hashmap and map it to the default value of type T. Then it will return a
    // reference to the newly created value in the map.
    T& operator[](const std::string& key){
        
        if(find(key) == nullptr){ insert(key, T()); }
        
        int index = hash(key) % buckets.size();
        
        typename std::list<Node*>::iterator it;
        
        for(it = buckets[index].begin(); it != buckets[index].end(); it++){
            if((*it)->first == key){
                break;
            }
        }
        return (*it)->second;
    }
    
    
private:
    struct Node{
        Node(std::string key, T value){
            first = key;
            second = value;
        }
        std::string first;
        T second;
    };
    
    double m_nElements, m_maxLoad;
    
    std::vector<std::list<Node*>> buckets;
        
    size_t hash(const std::string &key) const { return std::hash<std::string>()(key); }
    
    
    void rehash(){
        std::vector<std::list<Node*>> newBuckets (buckets.size()*2);
        
        for(int i = 0; i < buckets.size(); i++){
            if(!buckets[i].empty()){
                for(typename std::list<Node*>::iterator it = buckets[i].begin(); it != buckets[i].end(); it++){
                    int index = hash((*it)->first) % newBuckets.size();
                    newBuckets[index].push_back(*it);
                    *it = nullptr;
                }
            }
        }
        std::swap(buckets, newBuckets);
    }
};

template <typename T>
const T* HashMap<T>::find(const std::string& key) const
{
    int index = hash(key) % buckets.size();
    
    if(buckets[index].empty()) return nullptr;

        
    for(typename std::list<Node*>::const_iterator it = buckets[index].begin(); it != buckets[index].end(); it++){
        if((*it)->first == key){
            return &((*it)->second);
        }
    }
    
    return nullptr;
}


template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value)
{
    if((m_nElements+1)/buckets.size() > m_maxLoad){ rehash(); }
    
    T* ptr = find(key);
    
    int index = hash(key) % buckets.size();
    
    if(ptr == nullptr){
        buckets[index].push_back(new Node(key, value));
        m_nElements++;
        return;
    }
    
    *find(key) = value;
}


#endif
/*
#ifndef hashmap_h
#define hashmap_h

#include <vector>
#include <list>
#include <string>
template <typename ValueType>
class HashMap{
public:
    HashMap(double max_load = 0.75); // constructor
    ~HashMap(); // destructor; deletes all of the items in the hashmap
    
    int size() const; // return the number of associations in the hashmap
    
    // The insert method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void insert(const std::string& key, const ValueType& value);
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value or modify it directly within the map.
    const ValueType* find(const std::string& key) const;
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value or modify it directly within the map.
    ValueType* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<ValueType*>(hm.find(key));
    }
    
    // Defines the bracket operator for HashMap, so you can use your map like this:
    // your_map["david"] = 2.99;
    // If the key does not exist in the hashmap, this will create a new entry in
    // the hashmap and map it to the default value of type T. Then it will return a
    // reference to the newly created value in the map.
    ValueType& operator[](const std::string& key);
    
private:
    int m_size;
    double m_max_load;
    struct Node{
        std::string key;
        ValueType value;
    };
    std::vector<std::list<Node*>> m_buckets;
    int hash(const std::string key) const;
    void checkRehash();
    void rehash();
    
};

template<typename ValueType>
HashMap<ValueType>::HashMap(double max_load):m_size(0), m_max_load(max_load), m_buckets(10){};

template<typename ValueType>
HashMap<ValueType>::~HashMap(){
    for(int i = 0; i < m_buckets.size(); i++){
        for(typename std::list<Node*>::iterator iter = m_buckets[i].begin(); iter != m_buckets[i].end(); iter++){
            delete *iter;
        }
    }
}

template <typename ValueType>
int HashMap<ValueType>::size() const{
    return this->m_size;
}

template<typename ValueType>
int HashMap<ValueType>::hash(const std::string key) const{
    size_t hashvalue = std::hash<std::string>()(key);
    return int(hashvalue%this->m_buckets.size());
}

template<typename ValueType>
void HashMap<ValueType>::checkRehash() {
    if (static_cast<double>(this->m_size)/this->m_buckets.size() > m_max_load){
        this->rehash();
    }
}

template<typename ValueType>                        //const ValueType* find(const std::string& key) const
const ValueType* HashMap<ValueType>::find(const std::string& key) const {
    int hashvalue = this->hash(key);
    if(m_buckets[hashvalue].size()==0){
        return nullptr;
    }
    
    for(typename std::list<Node*>::const_iterator it = m_buckets[hashvalue].begin(); it!= m_buckets[hashvalue].end(); it++){
        if ((*it)->key == key) {
            return &((*it)->value);
        }
    }
    return nullptr;
        
}

template<typename ValueType>
void HashMap<ValueType>::insert(const std::string& key, const ValueType& value){
    ValueType* valuePtr = find(key);
    if (valuePtr == nullptr){
        int hashvalue = this->hash(key);
        m_buckets[hashvalue].push_back(new Node{key, value});
        this->m_size +=1;
        checkRehash();
        return;
    } else {
        *valuePtr = value;
    }
}

template<typename ValueType>
ValueType& HashMap<ValueType>::operator[](const std::string& key){
    int hashvalue = hash(key);
    if(m_buckets[hashvalue].size()==0){
        Node* nodeInsert = new Node{key, ValueType()};
        m_buckets[hashvalue].push_back(nodeInsert);
        this->m_size+=1;
        checkRehash();
        return nodeInsert->value;
    } else {
        for(typename std::list<Node*>::iterator it = m_buckets[hashvalue].begin(); it!= m_buckets[hashvalue].end(); it++){
            if ((*it)->key == key) {
                return (*it)->value;
            }
        }
    }
    Node* nodeInsert = new Node{key, ValueType()};
    m_buckets[hashvalue].push_back(nodeInsert);
    this->m_size+=1;
    checkRehash();
    return nodeInsert->value;
}


template<typename ValueType>
void HashMap<ValueType>::rehash(){
    std::vector<std::list<Node*>> m_newBuckets(m_buckets.size()*2);
    for(int i = 0; i<m_buckets.size(); i++){
        if(m_buckets[i].size()!=0){
            for(typename std::list<Node*>::iterator iterL = m_buckets[i].begin(); iterL!=m_buckets[i].end(); iterL++){
                int newHash = int(std::hash<std::string>()((*iterL)->key)%m_newBuckets.size());
                m_newBuckets[newHash].push_back(*iterL);
                *iterL = nullptr;
            }
            m_buckets[i].clear();
        }
    }
    
    std::swap(m_buckets, m_newBuckets);
    m_newBuckets.clear();
}
*/
//TODO: to test the hashmap, remove before submission
//SO far hashmap compiles on g++ without memory leaks
/*
#include <iostream>
#include <cassert>
int main(){
    HashMap<int> hashmap(0.5);
    std::string keys[10] = {"ahello", "bhello", "chello", "dhello", "ehello", "fhello","ghello", "hhello", "ihello", "jhello"};
    int values[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(int i = 0; i<10;i++){
        hashmap.insert(keys[i], values[i]);
    }
    assert(*hashmap.find("ahello")==1);
    assert(hashmap["dhello"] == 4);
    hashmap["dhello"] = 12;
    assert(hashmap["dhello"] == 12);
    hashmap["khello"] = 11;
    assert(hashmap["khello"] == 11);
    hashmap["xhello"] = 13;
    assert(hashmap["xhello"] == 13);
    assert(hashmap.size() == 12); //12 elements in the hashmap
    std::cout<<"All tests passed!"<<std::endl;
}
*/





