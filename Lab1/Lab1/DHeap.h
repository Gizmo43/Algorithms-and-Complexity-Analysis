#ifndef DHEAP_H
#define DHEAP_H

#include <vector>
#include <algorithm>

template <typename T>
class DHeap {
private:
    std::vector<int> name;    
    std::vector<T> key;       
    std::vector<int> heap_index; 
    int n;                    
    int d;                     

    int first_child(int i) const {
        int k = (i - 1) * d + 2;
        if (k > n)
            return 0;
        return k;
    }

    int last_child(int i) const {
        int kf = first_child(i);
        if (kf == 0)
            return 0;
        return std::min(kf + d - 1, n);
    }

    int father(int i) const {
        if (i == 1)
            return 1;
        return (i - 2) / d + 1;
    }

    int minchild(int i) const {
        int kf = first_child(i);
        if (kf == 0)
            return i;

        int kl = last_child(i);
        T min_key = key[kf - 1];
        int min_child = kf;

        for (int j = kf + 1; j <= kl; j++) {
            if (key[j - 1] < min_key) {
                min_key = key[j - 1];
                min_child = j;
            }
        }
        return min_child;
    }

    void swapElements(int i, int j) {

        std::swap(key[i - 1], key[j - 1]);
        std::swap(name[i - 1], name[j - 1]);

        heap_index[name[i - 1]] = i;
        heap_index[name[j - 1]] = j;
    }

public:
    DHeap(int d_val, std::vector<int>& names, std::vector<T>& keys,
        std::vector<int>& index, int n_val)
        : d(d_val), n(n_val), name(names), key(keys), heap_index(index) {
    }

    void heapDown(int i) {
        while (true) {
            int c = minchild(i);
            if (c == i || key[i - 1] <= key[c - 1]) {
                break;
            }
            swapElements(i, c);
            i = c;
        }
    }

    void heapUp(int i) {
        while (i > 1) {
            int p = father(i);
            if (key[p - 1] <= key[i - 1]) {
                break;
            }
            swapElements(i, p);
            i = p;
        }
    }

    void buildQueue() {

        for (int i = 0; i < heap_index.size(); i++) {
            heap_index[i] = i + 1; 
        }

        for (int i = n; i >= 1; i--) {
            heapDown(i);
        }
    }

    void extractMinimum(int& name_val, T& key_val) {
        if (n == 0) {
            throw "Heap is empty";
        }

        name_val = name[0];
        key_val = key[0];
        heap_index[name_val] = -1;

        if (n > 1) {
            name[0] = name[n - 1];
            key[0] = key[n - 1];
            heap_index[name[0]] = 1;

            n--;
            heapDown(1);
        }
        else {
            n--;
        }
    }

    void decreaseKey(int vertex, T new_key) {
        int pos = heap_index[vertex];
        if (pos == -1 || pos > n) {
            return; 
        }

        if (new_key >= key[pos - 1]) {
            return;
        }
        key[pos - 1] = new_key;
        heapUp(pos);
    }

    bool isEmpty() const {
        return n == 0;
    }

    int size() const {
        return n;
    }

    bool isInHeap(int vertex) const {
        int pos = heap_index[vertex];
        return pos != -1 && pos <= n;
    }
};

#endif