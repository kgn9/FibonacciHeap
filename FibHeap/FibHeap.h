#pragma once

struct Node
{
    Node *parent,
         *child,
         *left,
         *right;
    int key,
        degree;
    bool mark;

    Node(int n);
};

class FibHeap
{
public:
    FibHeap();

    void add(Node *x);
    void merge(FibHeap *h);
    void decreaseKey(Node *x, int k);
    int extractMin();
    Node *getMin();

private:
    int size_;
    Node *min_;

    void consolidate();
    void addChild(Node *p, Node *c);
    void unionList(Node *f, Node *s);
    void cut(Node *x);
    void cascadingCut(Node *x);
};

