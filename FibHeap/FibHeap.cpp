#include "FibHeap.h"
#include <vector>

FibHeap::FibHeap()
    : size_(0)
    , min_(nullptr)
{

}

void FibHeap::add(Node *x)
{
    x->parent = nullptr;
    x->mark = false;

    if (!min_) {
        min_ = x;
        min_->left = min_;
        min_->right = min_;
    }
    else {
        x->right = min_->right;
        min_->right->left = x;
        min_->right = x;
        x->left = min_;
        min_ = x->key < min_->key ? x : min_;
    }

    size_++;
}

void FibHeap::unionList(Node *f, Node *s)
{
    auto L = f->left;
    auto R = s->right;
    s->right = f;
    f->left = s;
    L->right = R;
    R->left = L;
}

void FibHeap::merge(FibHeap *h)
{
    if (!h->min_)
        return;
    if (!min_) {
        min_ = h->min_;
        size_ = h->size_;
        return;
    }

    unionList(min_, h->min_);

    size_ += h->size_;

    if (h->min_->key < min_->key)
        min_ = h->min_;
}

void FibHeap::cut(Node *x)
{
    auto p = x->parent;

    p->degree--;

    if (p->child == x)
        if (p->child == p->child->right)
            p->child = nullptr;
        else
            p->child = p->child->right;

    x->right->left = x->left;
    x->left->right = x->right;

    add(x);
    size_--;
}

void FibHeap::cascadingCut(Node *x)
{
    while (x->parent and x->mark) {
        auto p = x->parent;
        cut(x);
        x = p;
    }

    x->mark = true;
}

void FibHeap::decreaseKey(Node *x, int k)
{
    if (k > x->key)
        return;

    if (!x->parent) {
        x->key = k;
        min_ = k < min_->key ? x : min_;
        return;
    }

    x->key = k;
    auto p = x->parent;

    cut(x);
    cascadingCut(p);
}

void FibHeap::addChild(Node *p, Node *c)
{
    c->right->left = c->left;
    c->left->right = c->right;

    if (p->child) {
        c->right = p->child->right;
        p->child->right->left = c;
        c->left = p->child;
        c->parent = p;
        p->child->right = c;
    }
    else {
        p->child = c;
        p->child->left = c;
        p->child->right = c;
        c->parent = p;
    }
}

void FibHeap::consolidate()
{
    int max_degree = log(size_) / log(1.618) + 1;

    std::vector<Node *> degree_arr(max_degree, nullptr);
    std::vector<Node *> elem;

    auto z = min_;
    do {
        elem.push_back(z);
        z = z->right;
    } while (z != min_);

    for (auto x : elem) {
        while (degree_arr[x->degree] != nullptr) {
            auto y = degree_arr[x->degree];
            degree_arr[x->degree] = nullptr;
            auto add_to = (x->key < y->key) ? x : y;
            auto adding = (add_to == x) ? y : x;
            addChild(add_to, adding);
            adding->parent = add_to;
            add_to->degree++;
            x = add_to;
        }

        degree_arr[x->degree] = x;
    }

    min_ = nullptr;
    for (auto i : degree_arr) {
        if (i) {
            if (!min_) {
                min_ = i;
                min_->left = i;
                min_->right = i;
            }
            else {
                add(i);
                size_--;
            }
        }
    }
}

int FibHeap::extractMin()
{
    auto z = min_;

    if (z) {
        if (z->child) {
            auto f = z->child;
            do {
                auto b = f->right;
                add(f);
                size_--;
                f = b;
            } while (f != z->child);
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            min_ = nullptr;
        }
        else {
            min_ = z->right;
            consolidate();
        }

        size_--;
    }

    int r = z->key;
    delete z;

    return r;
}

Node *FibHeap::getMin()
{
    return min_;
}

Node::Node(int n)
    : key(n)
    , degree(0)
    , parent(nullptr)
    , child(nullptr)
    , left(nullptr)
    , right(nullptr)
    , mark(nullptr)
{

}