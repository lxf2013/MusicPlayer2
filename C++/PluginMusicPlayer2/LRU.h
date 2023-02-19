#include <unordered_map>
#include <string>
using namespace std;

class LRU{
private:
    struct Node{
        string name;
        Node  *last;
        Node  *next;
        Node(const string &name);
    };
private:
    unordered_map<string, Node *> m_hash_data;
    Node m_head;
    Node m_tail;
    int  m_capacity;
private:
    void erase(Node *node);
    void insert(Node *head, Node *node);
public:
    LRU(int capacity);
    ~LRU();
    bool Update(uint32_t capacityy);
    string insert(const string &name);
    bool empty();
};
