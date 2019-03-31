#include <algorithm>
#include <iostream>
#include <cassert>
#include <random>
#include <string>
#include <vector>
#include <set>

using namespace std;

class BinarySearchTree {
 public:

  BinarySearchTree();
  ~BinarySearchTree();

  bool Contains(int value) const;
  void Add(int value);
  void Erase(int value);

  int Size() const;
  vector<int> ToSortedArray() const;

 protected:

  struct Node {

    int value_;

    Node* parent;
    Node* left;
    Node* right;

    Node(int value, Node* parent) : value_(value),
                                    parent(parent),
                                    left(nullptr),
                                    right(nullptr) {}
    ~Node() {
      delete left;
      delete right;
    }
  };

  Node* root_;
  int size_;

  Node* Find(int value) const;
  void Detach(Node* target_node);

  Node* GetLeftmostNode(Node* node) const;
  Node* GetRightmostNode(Node* node) const;

  void AppendToSortedArray(Node* node, std::vector<int>* res) const;

  void NullPoint(Node* target_node_);

  Node* Son(Node* ptr);
};

BinarySearchTree::BinarySearchTree() : size_(0),
                                       root_(nullptr) {}

BinarySearchTree::~BinarySearchTree() {
  delete root_;
}

bool BinarySearchTree::Contains(int value) const {
  Node* ptr = root_;
  while (ptr != nullptr) {
    if (value > ptr->value_) {
      ptr = ptr->right;
      continue;
    }
    if (value < ptr->value_) {
      ptr = ptr->left;
      continue;
    }
    if (value == ptr->value_)
      return true;
  }
  return false;
}

void BinarySearchTree::Add(int value) {
  size_++;
  if (root_ == nullptr) {
    root_ = new Node(value, nullptr);
    return;
  }
  Node* ptr = root_;
  Node* parent = root_;
  while (ptr != nullptr) {
    parent = ptr;
    if (value >= ptr->value_) {
      ptr = ptr->right;
    } else {
      if (value < ptr->value_) {
        ptr = ptr->left;
      }
    }
  }
  ptr = new Node(value, parent);
  if (value >= parent->value_) {
    parent->right = ptr;
  } else {
    parent->left = ptr;
  }
}

void BinarySearchTree::Erase(int value) {
  Node* ptr = Find(value);
  if (ptr == nullptr)
    return;
  Detach(ptr);
  delete ptr;
  size_--;
  return;
}

int BinarySearchTree::Size() const {
  return size_;
}

vector<int> BinarySearchTree::ToSortedArray() const {
  vector<int> vect;
  vect.reserve(size_);
  AppendToSortedArray(root_, &vect);
  return vect;
}
BinarySearchTree::Node* BinarySearchTree::Find(int value) const {
  Node* ptr = root_;
  while (ptr != nullptr) {
    if (value == ptr->value_)
      break;
    if (value >= ptr->value_) {
      ptr = ptr->right;
    } else {
      if (value < ptr->value_) {
        ptr = ptr->left;
      }
    }
  }
  return ptr;
}
void BinarySearchTree::Detach(BinarySearchTree::Node* target_node) {
  Node* temp_node;
  if (target_node->left == nullptr && target_node->right == nullptr) {
    if (target_node->parent == nullptr) {
      root_ = nullptr;
      return;
    }
    if (target_node->value_ >= target_node->parent->value_) {
      target_node->parent->right = nullptr;
    } else {
      target_node->parent->left = nullptr;
    }
    return;
  }

  Node* first_child = nullptr, * second_child = nullptr, * ptr = nullptr;
  if (target_node->right != nullptr) {
    ptr = GetLeftmostNode(target_node->right);
    ptr->left = target_node->left;
    first_child = target_node->left;
    second_child = target_node->right;
  } else if (target_node->left != nullptr) {
    ptr = GetRightmostNode(target_node->left);
    ptr->right = target_node->right;
    first_child = target_node->right;
    second_child = target_node->left;
  }
  if (first_child != nullptr) {
    first_child->parent = ptr;
  }
  if (second_child->parent->parent != nullptr) {
    if (second_child->parent->value_ >= second_child->parent->parent->value_) {
      second_child->parent->parent->right = second_child;
    } else {
      second_child->parent->parent->left = second_child;
    }
  } else {
    root_ = second_child;
  }
  second_child->parent = target_node->parent;
  NullPoint(target_node);
}

BinarySearchTree::Node* BinarySearchTree::
GetLeftmostNode(BinarySearchTree::Node* node) const {
  if (node->left != nullptr)
    return GetLeftmostNode(node->left);
  return node;
}
BinarySearchTree::Node* BinarySearchTree::
GetRightmostNode(BinarySearchTree::Node* node) const {
  if (node->right != nullptr)
    return GetRightmostNode(node->right);
  return node;
}
void BinarySearchTree::AppendToSortedArray(BinarySearchTree::Node* node,
                                           vector<int>* res) const {
  if (node == nullptr)
    return;
  AppendToSortedArray(node->left, res);
  res->push_back(node->value_);
  AppendToSortedArray(node->right, res);
}
void BinarySearchTree::NullPoint(Node* target_node) {
  target_node->parent = nullptr;
  target_node->left = nullptr;
  target_node->right = nullptr;
}
BinarySearchTree::Node* BinarySearchTree::Son(Node* ptr) {
  if (ptr->left != nullptr) {
    return ptr->left;
  } else {
    return ptr->right;
  }
}

int main() {
  const int kQueriesCount = 50'000;
  const std::vector<int>
      kElementsMaxValues({1, 2, 5, 13, 42, 1024, 1'000'000'000});

  mt19937_64 random_generator(2018);
  for (const int max_element_value : kElementsMaxValues) {
    vector<int> elements;
    BinarySearchTree tree;

    for (int i = 0; i < kQueriesCount; ++i) {
      int query_type = random_generator() % 3;

      switch (query_type) {
        case 0: {
          int value;
          if (elements.empty() || random_generator() % 100 < 75) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          elements.push_back(value);
          sort(elements.begin(), elements.end());  // Never do like this :)
          tree.Add(value);
          break;
        }
        case 1: {
          int value;
          if (elements.empty() || random_generator() % 100 < 40) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          bool result = tree.Contains(value);
          bool expected_result =
              std::find(elements.begin(), elements.end(), value)
                  != elements.end();
          assert(result == expected_result);
          break;
        }
        case 2: {
          int value;
          if (elements.empty() || random_generator() % 100 < 25) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          auto element_it = std::find(elements.begin(), elements.end(), value);
          if (element_it != elements.end()) {
            elements.erase(element_it);
          }
          tree.Erase(value);
          break;
        }
        default: {
          break;  // IMPOSSIBLE
        }
      }
      assert(elements.size() == tree.Size());
      assert(elements == tree.ToSortedArray());
    }

    cout << "Passed: max_element_value = " << max_element_value << endl;
  }

  return 0;
}
