#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <set>
#include <iomanip>
#include <unordered_set>
#include <map>
#include <random>
#include <chrono>
#include <unordered_map>

struct Node {
public:
    bool is_free_ = false;
    int64_t size_ = 0;
    int64_t id_ = -1;
    Node* pref_ = nullptr;
    Node* next_ = nullptr;
    Node(int64_t size, int64_t id, bool is_free) {
        size_ = size;
        id_ = id;
        is_free_ = is_free;
    }
    Node() {
        size_ = 0;
        id_ = -1;
    }
};

class List {
public:
    List();
    explicit List(Node* node);
    void delNode(Node* node);
    void insAfter(Node* node, Node* cur);
    void coutList();
private:
    Node* L = nullptr;
};

List::List() {
    L = nullptr;
}

List::List(Node *node) {
    L = node;
}

void List::delNode(Node *node) {
    if (node != nullptr) {
        if (node->pref_ != nullptr) {
            Node* next = node->next_;
            if (next != nullptr) {
                next->pref_ = node->pref_;
            }
            node->pref_->next_ = next;
        } else {
            L = node->next_;
            if (L != nullptr)
                L->pref_ = nullptr;
        }
        node->next_ = nullptr;
        node->pref_ = nullptr;
        node = nullptr;
    }
}

void List::insAfter(Node *cur, Node* node) {
    Node* next = cur->next_;
    node->next_ = next;
    if (next != nullptr) {
        next->pref_ = node;
    }
    node->pref_ = cur;
    cur->next_ = node;
}

void List::coutList() {
    Node* cur = L;
    Node* pref = nullptr;
    while (cur != nullptr) {
        std::cout << cur->id_ << ' ' << cur->id_ + cur->size_ - 1 << "   ";
        pref = cur;
        cur = cur->next_;
    }
    std::cout << '\n';
    if (pref != nullptr)
        cur = pref;
    while (cur != nullptr) {
        std::cout << cur->id_ + cur->size_ - 1  << ' ' << cur->id_ << "   ";
        cur = cur->pref_;
    }
    std::cout << "\n-------------------------------\n";
}

class Heap {
public:
    std::vector <Node*> data_;
    int64_t size_ = 0;
    explicit Heap(int64_t sz) {
        data_.resize(sz, nullptr);
    }

    void addNode(Node* node) {
        data_[++size_] = node;
        siftUp(size_);
    }

    void remove(Node* node) {
        int id = -1;
        for (int i = 1; i <= size_; ++i) {
            if (data_[i]->size_ == node->size_ && data_[i]->id_ == node->id_) {
                id = i;
                break;
            }
        }
        if (id == -1)
            return;
        if (id == size_) {
            data_[size_--] = nullptr;
            return;
        }
        data_[id] = data_[size_--];
        data_[size_ + 1] = nullptr;
        siftDown(id);
        siftUp(id);
    }

    Node* extractMax() {
        if (size_ == 0)
            return nullptr;
        Node* res = data_[1];
        data_[1] = data_[size_--];
        if (size_ == 0) {
            data_[1] = nullptr;
            return res;
        }
        siftDown(1);
        return res;
    }

private:
    void siftUp(int64_t child) {
        int64_t par = child / 2;
        if (par == 0 || data_[par] == nullptr)
            return;
        if (data_[par]->size_ < data_[child]->size_ ||
        (data_[par]->size_ == data_[child]->size_ && data_[par]->id_ > data_[child]->id_)) {
            std::swap(data_[par], data_[child]);
            siftUp(par);
        }
    }

    void siftDown(int64_t par) {
        int64_t child = par * 2;
        if (child > size_)
            return;
        if (child + 1 <= size_ && ((data_[child+1]->size_ > data_[child]->size_) ||
        (data_[child+1]->size_ == data_[child]->size_ &&
        data_[child+1]->id_ < data_[child]->id_))) {
            ++child;
        }
        if (data_[par]->size_ < data_[child]->size_ ||
        (data_[par]->size_ == data_[child]->size_ && data_[par]->id_ > data_[child]->id_)) {
            std::swap(data_[par], data_[child]);
            siftDown(child);
        }
    }
};


signed main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Heap heap(200000);
    std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    int64_t nn, mm;
    std::cin >> nn >> mm;
//        nn = rnd() % 10000000;
//        mm = rnd() % 100000;
//        std::cout << nn << ' ' << mm << "++++++++++++++++++++++\n";
    auto node = new Node(nn, 1, true);
    heap.addNode(node);
    List list(node);
    std::vector <Node*> op(mm, nullptr);
    for (int64_t ii = 0; ii < mm; ++ii) {
        int64_t num;
        std::cin >> num;
//            num = rnd() % 100000 + 1;
//            num = -num * (rnd() % 2) + num * (!(rnd() % 2));
//            while (num == 0) {
//                num = rnd() % 100000 + 1;
//                num = -num * (rnd() % 2) + num * (!(rnd() % 2));
//            }
//            std::cout << num << "------\n";
        if (num > 0) {
            Node* cur = heap.extractMax();
            if (cur == nullptr) {
                std::cout << "-1\n";
                continue;
            }
            if (cur->size_ < num) {
                std::cout << "-1\n";
                heap.addNode(cur);
                continue;
            }
            int64_t sz = cur->size_;
            Node* left = new Node(num, cur->id_, false);
            Node* right = new Node(sz - num, cur->id_ + num, true);
            list.insAfter(cur, left);
            if (right->size_ != 0) {
                list.insAfter(left, right);
                heap.addNode(right);
            } else {
                list.delNode(right);
            }
            list.delNode(cur);
            op[ii] = left;
            std::cout << left->id_ << '\n';
        } else { // -------------------------------------------
            num = -num;
            --num;
            if (op[num] == nullptr) {
                continue;
            }
            Node* cur = op[num];
            op[num] = nullptr;
            Node* left = cur->pref_;
            Node* right = cur->next_;
            if (left == nullptr && right == nullptr) {
                cur->is_free_ = true;
                heap.addNode(cur);
                continue;
            }
            if (left == nullptr) {
                if (right->is_free_) {
                    Node* sum = new Node(cur->size_ + right->size_, cur->id_, true);
                    list.insAfter(right, sum);
                    heap.remove(right);
                    heap.addNode(sum);
                    list.delNode(cur);
                    list.delNode(right);

                } else {
                    cur->is_free_ = true;
                    heap.addNode(cur);
                }
                continue;
            }
            if (right == nullptr) {
                if (left->is_free_) {
                    Node* sum = new Node(left->size_ + cur->size_, left->id_, true);
                    list.insAfter(cur, sum);
                    heap.remove(left);
                    heap.addNode(sum);
                    list.delNode(left);
                    list.delNode(cur);
                } else {
                    cur->is_free_ = true;
                    heap.addNode(cur);
                }
                continue;
            }
            if (left->is_free_ && right->is_free_) {
                Node* sum = new Node(left->size_ + cur->size_ + right->size_, left->id_, true);
                list.insAfter(right, sum);
                heap.remove(left);
                heap.remove(right);
                list.delNode(left);
                list.delNode(cur);
                list.delNode(right);
                heap.addNode(sum);
                continue;
            }
            if (left->is_free_) {
                Node* sum = new Node(left->size_ + cur->size_, left->id_, true);
                list.insAfter(cur, sum);
                heap.remove(left);
                heap.addNode(sum);
                list.delNode(left);
                list.delNode(cur);
                continue;
            }
            if (right->is_free_) {
                Node* sum = new Node(cur->size_ + right->size_, cur->id_, true);
                list.insAfter(right, sum);
                heap.remove(right);
                heap.addNode(sum);
                list.delNode(cur);
                list.delNode(right);
                continue;
            }
            cur->is_free_ = true;
            heap.addNode(cur);
        }
    }
}
