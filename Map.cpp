#include "Map.h"

Map::Map() { //constructor
    //create empty linked list
    m_size = 0;
    
    //create dummy node
    head = new Node;
    head->next = head;
    head->prev = head;
}

Map::~Map() { //destructor
        Node *p = head;
        while (p->next != head) {
            //create temp variable to delete each node
            Node *d = p;
            p = p->next;
            delete d;
        }
    //delete head
    delete p;
}

Map::Map(const Map& old) { //copy constructor
        
    //create new dummy node for new linked list
    head = new Node;
    head->next = head;
    head->prev = head;
    
    //if old map only has dummy node, stop and return here
    if (old.m_size == 0) {
        m_size = 0;
        return;
    }
    
    //create temp pointers
    Node *p = head;
    Node *o = old.head->next;
    
    //iterate through old map and copy nodes
    while (o != old.head) {
        Node *newnode = new Node;
        newnode->m_key = o->m_key;
        newnode->m_value = o->m_value;
        p->next = newnode;
        newnode->prev = p;
        o = o->next;
        p = p->next;
    }
    p->next = head;
    head->prev = p;
    m_size = old.m_size;
}

Map& Map::operator=(const Map& other) { //assignment operator
    //make sure if maps are equal to just return this, if not use the swap function and return the changed map
    if (this != &other) {
        Map temp(other);
        swap(temp);
    }
    return *this;
}

bool Map::empty() const {
    
    //if no valid nodes, i.e. only dummy node, return true
    if (m_size == 0)
        return true;
    else
        return false;
}

int Map::size() const {
    
    //return number of valid items in linked list
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    
    //if the key is in the map, make no changes, return false
    if (contains(key))
        return false;
    
    //if key is not in the map
    else {
        //create temp node
        Node *p;
        //if only dummy node in map, add new node with key pair after dummy node
        if (m_size == 0) {
            p = new Node;
            p->m_key = key;
            p->m_value = value;
            p->next = head;
            head->next = p;
            head->prev = p;
            p->prev = head;
            m_size++; //increase size
            return true;
        }
        //if map is not empty
        else if (m_size > 0) {
            //iterate through loop until reach node with key value greater than key value passed in or until reach head
            p = head->next;
            while (p != head && p->m_key < key)
                p = p->next;
            
            //key value in node is greater than key value passed in
            if (p->m_key > key) {
                //insert new node with key pair before existing node
                Node* newnode = new Node;
                newnode->m_key = key;
                newnode->m_value = value;
                newnode->next = p;
                newnode->prev = p->prev;
                p->prev = newnode;
                newnode->prev->next = newnode;
                m_size++; //increase size
                return true;
            }
            
            //if reached head of list
            else if (p == head) {
                //insert new node with key pair at end of list, before head
                Node* newnode = new Node;
                newnode->m_key = key;
                newnode->m_value = value;
                newnode->next = head;
                newnode->prev = head->prev;
                head->prev = newnode;
                newnode->prev->next = newnode;
                m_size++; //increase size
                return true;
            }
        }
    }
    
    //return something, so no error
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
    
    //check if key exists in map
    if (contains(key)) {
        Node* p = head->next;
        
        //iterate through map to find key
        while (p->m_key != key && p != head)
            p = p->next;
        
        //once key is found update the value and return true
        if (p->m_key == key && p != head) {
            p->m_value = value;
            return true;
        }
    }
    //if here means no matching key exists
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
   //check if key exists in map, if so update key
    if (contains(key))
       return update(key, value);
    else //if not insert key
        return insert(key, value);
}

bool Map::erase(const KeyType& key) {
    //check if key exists in map
    if (contains(key)) {
        Node* p = head->next;
        
        //iterate through map to find key
        while (p->m_key != key && p != head)
            p = p->next;
        
        //delete node with key, decrement size
        if (p->m_key == key && p != head) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
            m_size--;
            return true;
        }
    }
    //if here means no key in map so return false
    return false;
}

bool Map::contains(const KeyType& key) const {
    //set temp variable
    Node* p = head->next;
    
    //traverse through linked list if we have not found a matching key value and we have not reached beginning yet
    while (p->m_key != key && p != head)
        p = p->next;

    //if found matching key value, return true
    if (p->m_key == key && p != head) {
        return true;
    }
    
    //if ended up at the beginning, no match found, return false
    if (p == head)
        return false;
    
return false; //should never reach here
}


bool Map::get(const KeyType& key, ValueType& value) const {
    
    //check if map contains key
    if (contains(key)) {
        Node* p = head;
        
        //iterate through map to find key
        while (p->m_key != key)
            p = p->next;
        
        //once key is found, set value to value associated with that key and return true
        if (p->m_key == key) {
            value = p->m_value;
            return true;
        }
    }
    //if here means no such key exists in the map so return false
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    
    //if i is greater than the number of nodes in list or negative, return false
   if (i >= m_size || i < 0)
       return false;
    
    //create temp node and counter
    Node* p = head->next;
    
    //iterate through linked list
    for (int j = 0; j < i; j++) {
        p = p->next;
    }

    //set key to node's key and value to node's value
        key = p->m_key;
        value = p->m_value;
    
    //once here means the key and value are updated so return true
    return true;
}

void Map::swap(Map& other) {
    
    //swap sizes
    int temp = m_size;
    m_size = other.m_size;
    other.m_size = temp;
    
    //swap head pointers
    Node *p = head;
    head = other.head;
    other.head = p;
}

bool merge(const Map& m1, const Map& m2, Map& result) {
    //set result to be m1
    result = m1;
        
    //create variables to pass into functions
    KeyType key2;
    ValueType value1, value2;
    
    //create flag for whether the there are matching keys with different values
    bool conflict = false;
    
    //loop through m2
    for (int j = 0; j < m2.size(); j++) {
        //get key and value for every node in m2
        m2.get(j, key2, value2);
        //if the key does not exist in the result map, insert it
        if (!result.contains(key2))
            result.insert(key2, value2);
        //if the key exists in result map get the value of the key in the result map
        else {
            result.get(key2, value1);
            //if the values are equal leave the result map as is
            //if the values are not equal, erase the key/value pair from result map, and increase counter
            if (value1 != value2) {
                result.erase(key2);
                conflict = true;
            }
        }
    }
    //return type correct for situation
    return !conflict;
}

void reassign(const Map& m, Map& result) {
    //set result to me
    result = m;
    KeyType key1, key2;
    ValueType value, temp;

    int i = 0;
    result.get(i, key1, temp);
    
    for (i = 1; i < m.size(); i++) {
        result.get(i, key2, value);
        result.update(key1, value);
        key1 = key2;
    }
    
    result.update(key2, temp);
}

