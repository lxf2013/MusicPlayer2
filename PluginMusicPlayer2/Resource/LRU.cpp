#include "LRU.h"
#include "../common.h"

LRU::Node::Node(const std::string &name): name(name), last(nullptr), next(nullptr){
}

void LRU::Erase(Node *node){
    node->last->next = node->next;
    node->next->last = node->last;
}

void LRU::Insert(Node *head, Node *node){
    head->next->last = node;
    node->next = head->next;

    head->next = node;
    node->last = head;
}

LRU::LRU(int capacity): m_head("null"), m_tail("null"), m_capacity(capacity){
    if(capacity < 1){
        m_capacity = 1;
    }
    m_head.next = &m_tail;
    m_tail.last = &m_head;
}

LRU::~LRU(){
    for(Node *it=m_head.next; it!=&m_tail;){
        Node *temp = it;
        it=it->next;
        MemoryCount::FreeNode();
        delete temp;
    }
}

bool LRU::SetCapacity(uint32_t capacity){
    if(capacity <= m_capacity){
        return false;
    }
    m_capacity = capacity;
    return true;
}

std::string LRU::Insert(const std::string &name){
    std::string ret;
    auto it = m_hash_data.find(name);
    if(it != m_hash_data.end()){
        Node *temp = it->second;
        Erase(temp);
        Insert(&m_head, temp);
        return ret;
    }
    if(m_hash_data.size() >= m_capacity){
        Node *temp = m_tail.last;
        m_hash_data.erase(temp->name);
        Erase(temp);
        ret = move(temp->name);
        MemoryCount::FreeNode();
        delete temp;
    }
    MemoryCount::AllocNode();
    Node *temp = new Node(name);
    Insert(&m_head, temp);
    m_hash_data[name] = temp;
    return ret;
}

bool LRU::Query(const std::string &name){
    auto it = m_hash_data.find(name);
    if(it != m_hash_data.end()){
        Node *temp = it->second;
        Erase(temp);
        Insert(&m_head, temp);
        return true;
    }
    return false;
}

bool LRU::Empty(){
    return m_hash_data.empty();
}