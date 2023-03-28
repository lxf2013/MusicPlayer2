#pragma once
#include <unordered_map>
#include <string>

class LRU{
private:
    struct Node{
        std::string name;
        Node  *last;
        Node  *next;
        Node(const std::string &name);
    };
private:
    std::unordered_map<std::string, Node *> m_hash_data;
    Node m_head;
    Node m_tail;
    int  m_capacity;
private:
    void Erase(Node *node);
    void Insert(Node *head, Node *node);
public:
    LRU(int capacity=1);
    ~LRU();
    bool SetCapacity(uint32_t capacityy);
    std::string Insert(const std::string &name);
    bool Query(const std::string &name);
    bool Empty();
};
