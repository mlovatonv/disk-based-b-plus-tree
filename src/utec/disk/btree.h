#pragma once

#include "pagemanager.h"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

namespace utec {
namespace disk {

template <class T, int BTREE_ORDER>
class btree;

template <class T, int BTREE_ORDER>
class node;

template <class T, int BTREE_ORDER>
class btreeiterator {
private:
  friend class btree<T, BTREE_ORDER>;
  typedef utec::disk::node<T, BTREE_ORDER> node;
  
  std::shared_ptr<pagemanager> pm;
  int index;
  long node_id;
public:
  btreeiterator(std::shared_ptr<pagemanager> &pm, long node_id) : 
    pm(pm), node_id(node_id), index(0) {}
  
  btreeiterator(std::shared_ptr<pagemanager> &pm, long node_id, int index) : 
    pm(pm), node_id(node_id), index(index) {}
 
  btreeiterator(std::shared_ptr<pagemanager> &pm, const btreeiterator& other): 
    pm(pm), node_id(other.node_id), index(other.index) {}
  
  node read_node(long page_id) {
    node n(-1);
    if (page_id == -1) {
      n.next = -1;
    } else {
      pm->recover(page_id, n);
    }
    return n;
  }
  
  btreeiterator& operator=(btreeiterator other) { 
    this->node_id = other.node_id;
    this->index = other.index;
    return *this;
  }

  btreeiterator& operator++() {
    node n = read_node(this->node_id);
    this->index++;
    if (n.count <= index) {
      this->node_id = n.next;
      this->index = 0;
    }
    return *this;
  }

  btreeiterator& operator++(int) { 
    btreeiterator it(pm, *this);
    ++(*this);
    return it; 
  }

  bool operator==(const btreeiterator& other) { 
    return (this->node_id == other.node_id) && (this->index == other.index); 
  }

  bool operator!=(const btreeiterator& other) {
    return !((*this) == other);
  } 
  
  T operator*() { 
    node n = read_node(this->node_id);
    return n.keys[index]; 
  }

  long get_page_id() {
    node n = read_node(this->node_id);
    return n.pages[index];
  }
};

template <class T, int BTREE_ORDER>
class node { 
public:
  long page_id{-1};
  long count{0};
  
  T keys[BTREE_ORDER + 1];
  
  long pages[BTREE_ORDER + 2];
  long next{-1};

  bool is_leaf{false};
  
  node(long page_id) : page_id(page_id) {
    for (int i = 0; i < BTREE_ORDER + 2; i++) {
      pages[i] = 0;
    }
  }

  node(long page_id, bool is_leaf) : page_id(page_id), is_leaf(is_leaf) {
    for (int i = 0; i < BTREE_ORDER + 2; i++) {
      pages[i] = 0;
    }
  }

  node& operator=(node other) {
    page_id = other.page_id;
    count = other.count;
    int i = 0;
    for (; i < BTREE_ORDER + 1; ++i) {
      keys[i] = other.keys[i];
      pages[i] = other.pages[i];
    }
    pages[i] = other.pages[i];
    next = other.next;
    is_leaf = other.is_leaf;
  }

  void insert_in_node(int pos, const T &key) {
    int j = count;
    while (j > pos) {
      keys[j] = keys[j - 1];
      pages[j + 1] = pages[j];
      j--;
    }
    keys[j] = key;
    pages[j + 1] = pages[j];

    count++;
  }

  void insert_in_node(int pos, const T &key, const long &data_id) {
    int j = count;
    while (j > pos) {
      keys[j] = keys[j - 1];
      pages[j] = pages[j - 1];
      j--;
    }
    keys[j] = key;
    pages[j] = data_id;

    count++;
  }

  bool is_overflow() { return count > BTREE_ORDER; }
};


template <class T, int BTREE_ORDER>
class btree {
public:
  typedef utec::disk::node<T, BTREE_ORDER> node;
  typedef btreeiterator<T, BTREE_ORDER> iterator;

  struct Metadata {
    long root_id{1};
    long count{0};
  } header;

  enum state {
    BT_OVERFLOW,
    BT_UNDERFLOW,
    NORMAL,
  };

private:
  std::shared_ptr<pagemanager> pm;
public:
  btree(std::shared_ptr<pagemanager> &pm) : pm{pm} {
    if (pm->is_empty()) {
      node root(header.root_id, true);
      pm->save(root.page_id, root);

      header.count++;

      pm->save(0, header);
    } else {
      pm->recover(0, header);
    }
  }
  
  node new_node() {
    header.count++;
    node ret(header.count);
    pm->save(0, header);
    return ret;
  }

  node read_node(long page_id) {
    node n{-1};
    pm->recover(page_id, n);
    return n;
  }

  bool write_node(long page_id, node n) { pm->save(page_id, n); }

  void insert(const T &key, const long &data_id) {
    node root = this->read_node(header.root_id);
    int state = this->insert(root, key, data_id);

    if (state == BT_OVERFLOW) {
      this->split_root();
    }
  }

  int insert(node &ptr, const T &key, const long &data_id) {
    int pos = 0; 
    while (pos < ptr.count && ptr.keys[pos] < key) {
      pos++;
    }

    if (ptr.is_leaf) {
      ptr.insert_in_node(pos, key, data_id);
      this->write_node(ptr.page_id, ptr);
    } else {
      long page_id = ptr.pages[pos];
      node child = read_node(page_id);
      int state = insert(child, key, data_id);
      if (state == BT_OVERFLOW) {
        if (child.is_leaf) {
          this->split_leaf(ptr, pos);
        } else {
          this->split_inner(ptr, pos);
        }
      }
    }

    return ptr.is_overflow() ? BT_OVERFLOW : NORMAL;
  }

  void split_inner(node &parent, int pos) {
    node left = this->read_node(parent.pages[pos]);
    node right = this->new_node();

    int mid = BTREE_ORDER / 2;
    int mid_key = left.keys[mid];
    int iter = BTREE_ORDER / 2 + 1;
    int i;

    left.keys[mid] = 0;
    for (i = 0; iter < BTREE_ORDER + 1; i++) {
      right.pages[i] = left.pages[iter];
      right.keys[i] = left.keys[iter];
      right.count++;
      
      left.pages[iter] = left.keys[iter] = 0; 

      iter++;
    }
    right.pages[i] = left.pages[iter];
    left.pages[iter] = 0;
    left.count = mid;
    
    parent.insert_in_node(pos, mid_key);
    parent.pages[pos] = left.page_id;
    parent.pages[pos + 1] = right.page_id;

    this->write_node(parent.page_id, parent);
    this->write_node(left.page_id, left);
    this->write_node(right.page_id, right);
  }

  void split_leaf(node &parent, int pos) {
    node left = this->read_node(parent.pages[pos]);
    node right = this->new_node();
    right.is_leaf = true;

    int mid = BTREE_ORDER / 2;
    int mid_key = left.keys[mid];
    int iter = BTREE_ORDER / 2;
    int i;

    for (i = 0; iter < BTREE_ORDER + 1; i++) {
      right.pages[i] = left.pages[iter];
      right.keys[i] = left.keys[iter];
      right.count++;
      
      left.pages[iter] = left.keys[iter] = 0; 

      iter++;
    }
    left.count = mid;
    
    right.next = left.next; 
    left.next = right.page_id;
    
    parent.insert_in_node(pos, mid_key);
    parent.pages[pos] = left.page_id;
    parent.pages[pos + 1] = right.page_id;

    this->write_node(parent.page_id, parent);
    this->write_node(left.page_id, left);
    this->write_node(right.page_id, right);
  }

  void split_root() {
    node ptr = this->read_node(this->header.root_id);
    node left = this->new_node();
    node right = this->new_node();

    int i = 0;
    int iter = 0;
    int mid = ptr.count / 2;
    int mid_key = ptr.keys[mid];

    if (ptr.is_leaf) {
      ptr.is_leaf = false;
      left.is_leaf = true;
      right.is_leaf = true;
    
      for (i = 0; iter < mid; i++) {
        left.keys[i] = ptr.keys[iter];
        left.pages[i] = ptr.pages[iter];
        left.count++;

        ptr.keys[iter] = ptr.pages[iter] = 0;
        iter++;
      }

      for (i = 0; iter < BTREE_ORDER + 1; i++) {
        right.keys[i] = ptr.keys[iter];
        right.pages[i] = ptr.pages[iter];
        right.count++;

        ptr.keys[iter] = ptr.pages[iter] = 0;
        iter++;
      }

      left.next = right.page_id;
    } else {
      for (i = 0; iter < mid; i++) {
        left.keys[i] = ptr.keys[iter];
        left.pages[i] = ptr.pages[iter];
        left.count++;

        ptr.keys[iter] = ptr.pages[iter] = 0;
        iter++;
      }
      left.pages[i] = ptr.pages[iter];
      ptr.pages[iter] = 0;
      iter++;

      for (i = 0; iter < BTREE_ORDER + 1; i++) {
        right.keys[i] = ptr.keys[iter];
        right.pages[i] = ptr.pages[iter];
        right.count++;

        ptr.keys[iter] = ptr.pages[iter] = 0;
        iter++;
      }
      right.pages[i] = ptr.pages[iter];
      ptr.pages[iter] = 0;
    }
    
    ptr.pages[0] = left.page_id;
    ptr.pages[1] = right.page_id;
    ptr.keys[0] = mid_key;
    ptr.count = 1;

    this->write_node(ptr.page_id, ptr);
    this->write_node(left.page_id, left);
    this->write_node(right.page_id, right);
  } 

  iterator find(const T &key) {
    node ptr = this->read_node(header.root_id);
    int pos = 0;

    while (!ptr.is_leaf) {
      pos = 0;
      while (pos < ptr.count && ptr.keys[pos] < key) {
        pos++;
      }
      ptr = this->read_node(ptr.pages[pos]);
    }

    pos = 0;
    while (pos < ptr.count && ptr.keys[pos] < key) {
      pos++;
    }

    iterator it(this->pm, ptr.page_id, pos);
    return it;
  }

  std::pair<iterator,iterator> range_search(const T &begin, const T &end) {
    return std::make_pair(this->find(begin), this->find(end));
  }

  iterator begin() {
    return this->find(0);
  }

  iterator end() {
    iterator it(this->pm, -1);
    return it;
  }
  
  void print(std::ostream& out) {
    for (auto it = this->begin(); it != this->end(); ++it) {
      out << (*it);
    }
  }

  void print_tree() {
    node root = read_node(header.root_id);
    print_tree(root, 0);
    std::cout << "________________________\n";
  }

  void print_tree(node &ptr, int level) {
    int i = 0;
    for (i = ptr.count - 1; i >= 0; i--) {
      if (!ptr.is_leaf && ptr.pages[i + 1]) {
        node child = read_node(ptr.pages[i + 1]);
        print_tree(child, level + 1);
      }

      for (int k = 0; k < level; k++) {
        std::cout << "    ";
      }
      std::cout << ptr.keys[i] << "\n";
    }
    if (!ptr.is_leaf && ptr.pages[i + 1]) {
      node child = read_node(ptr.pages[i + 1]);
      print_tree(child, level + 1);
    }
  }
};

} // namespace disk
} // namespace utec

