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
            if(!buckets[i].empty())
            for(typename std::list<Node*>::iterator it = traversal.begin(); it != traversal.end(); it++){
                delete (*it);
                it = traversal.erase(it);
            }
        }
        buckets.clear();
        traversal.clear();
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
    T* find(const std::string& key);
    
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
    
    std::list<Node*> traversal;
    
    size_t hash(const std::string &key) const { return std::hash<std::string>()(key); }
    
    
    void rehash(){
        std::vector<std::list<Node*>> newBuckets (buckets.size()*2);
        std::list<Node*> newTraversal;
        
        for(typename std::list<Node*>::iterator it = traversal.begin(); it != traversal.end(); it++){
            int index = hash((*it)->first) % newBuckets.size();
            Node *n = new Node((*it)->first, (*it)->second);
            newBuckets[index].push_back(n);
            newTraversal.push_back(n);

        }
        
        swap(buckets, newBuckets);
        swap(traversal, newTraversal);

        for(int i = 0; i < newBuckets.size(); i++){
            if(!newBuckets[i].empty())
            for(typename std::list<Node*>::iterator it = newTraversal.begin(); it != newTraversal.end(); it++){
                delete (*it);
                it = newTraversal.erase(it);
            }
        }
        newBuckets.clear();
        newTraversal.clear();
    }
    
};

template <typename T>
T* HashMap<T>::find(const std::string& key)
{
    int index = hash(key) % buckets.size();
    
    if(buckets[index].empty()) return nullptr;
    
    typename std::list<Node*>::iterator it;
        
    for(it = buckets[index].begin(); it != buckets[index].end(); it++){
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
        Node *n = new Node(key, value);
        buckets[index].push_back(n);
        traversal.push_back(n);
        m_nElements++;
        return;
    }
    
    *find(key) = value;
}


#endif
