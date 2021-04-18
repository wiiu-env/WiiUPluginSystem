#pragma once

template<class T>
struct Node {
    Node<T> *next;
    T data;
};

template<class T>
class LinkedList {
public:
    Node<T> *first;
    Node<T> *last;

    LinkedList<T>() {
        first = nullptr;
        last = nullptr;
    }

    ~LinkedList<T>() {
        clear();
    }

    void clear() {
        if (!first) {
            return;
        }
        auto curNode = first;

        while (curNode->next != nullptr) {
            auto tmp = curNode->next;
            delete curNode;
            curNode = tmp;
        }
    }

    void add(T data) {
        if (!first) {
            // The list is empty
            first = new Node<T>;
            first->data = data;
            first->next = nullptr;
            last = first;
        } else {
            // The list isn't empty
            if (last == first) {
                // The list has one element
                last = new Node<T>;
                last->data = data;
                last->next = nullptr;
                first->next = last;
            } else {
                // The list has more than one element
                auto *insdata = new Node<T>;
                insdata->data = data;
                insdata->next = nullptr;
                last->next = insdata;
                last = insdata;
            }
        }
    }

    T get(int index) {
        if (index == 0) {
            // Get the first element
            return this->first->data;
        } else {
            // Get the index'th element
            Node<T> *curr = this->first;
            for (int i = 0; i < index; ++i) {
                curr = curr->next;
            }
            return curr->data;
        }
    }

    T operator[](int index) {
        return get(index);
    }

    int size() {
        if (!first) {
            return 0;
        }
        int result = 1;
        auto curNode = first;
        while (curNode->next != nullptr) {
            curNode = curNode->next;
            result++;
        }
        return result;
    }
};