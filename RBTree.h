#pragma once

#include <iostream>
#include <utility>

#include "s21_vector.h"

enum class color { RED, BLACK };

template <typename Type>
struct Node {
  Type val;
  color c;
  Node *parent;
  Node *left;
  Node *right;

  Node() = delete;

  Node(Type val, color c, Node *parent, Node *left, Node *right)
      : val(val), c(c), parent(parent), left(left), right(right) {}
};

template <typename Type>
class RBTree {
 public:
  class RBIterator {
   public:
    RBIterator() = delete;
    RBIterator(Node<Type> *node, bool end = false)
        : iter_(node), endFlag_(end) {}
    RBIterator(const RBIterator &iter)
        : iter_(iter.iter_), endFlag_(iter.endFlag_) {}
    RBIterator(RBIterator &&iter)
        : iter_(std::exchange(iter.iter_, nullptr)), endFlag_(iter.endFlag_) {}

   public:
    Type &operator*() noexcept {
      //            if (!iter_ || endFlag_)
      //                throw std::domain_error("nullptr");
      return iter_->val;
    }
    const Type &operator*() const noexcept {
      //            if (!iter_ || endFlag_)
      //                throw std::domain_error("nullptr");
      return iter_->val;
    }
    Node<Type> *operator->() noexcept { return iter_; }
    const Node<Type> *operator->() const noexcept { return iter_; }
    void operator=(const RBIterator &iter) const noexcept {
      iter_ = iter.iter_;
      endFlag_ = iter.endFlag_;
    }
    void operator=(RBIterator &&iter) const noexcept {
      iter_ = std::exchange(iter.iter_, nullptr);
      endFlag_ = std::exchange(iter.endFlag_, endFlag_);
    }
    bool operator==(const RBIterator &r) const noexcept {
      return r.iter_ == iter_ && r.endFlag_ == endFlag_;
    }
    bool operator!=(const RBIterator &r) const noexcept {
      return !((*this) == r);
    }
    operator bool() const noexcept { return iter_ != nullptr && !endFlag_; }

    RBIterator operator++(int) const {
      RBIterator res(*this);
      ++(*this);

      return res;
    }

    RBIterator operator++() const {
      if (iter_) iter_ = nextNode();

      return *this;
    }

    RBIterator operator--(int) const {
      RBIterator res(*this);
      --(*this);

      return res;
    }

    RBIterator operator--() const {
      if (iter_) iter_ = prevNode();

      return *this;
    }

   private:
    Node<Type> *prevNode() const {
      Node<Type> *temp = iter_;
      if (endFlag_)
        endFlag_ = false;
      else if (temp->left) {
        temp = temp->left;
        while (temp->right) temp = temp->right;
      } else {
        while (LeftSon(temp)) temp = temp->parent;
        temp = temp->parent;
      }

      return temp;
    }

    Node<Type> *nextNode() const {
      Node<Type> *temp = iter_;
      if (temp->right) {
        temp = temp->right;
        while (temp->left) temp = temp->left;
      } else {
        while (temp->parent && !LeftSon(temp)) temp = temp->parent;

        if (temp->parent)
          temp = temp->parent;
        else {
          temp = iter_;
          endFlag_ = true;
        }
      }
      return temp;
    }

    bool LeftSon(Node<Type> *node) const { return node->parent->left == node; }

    // private:
   public:
    mutable Node<Type> *iter_;
    mutable bool endFlag_;
  };

 public:
  RBTree();
  ~RBTree();
  RBTree(const RBTree &m);

 public:
  void operator=(const RBTree &m);
  void operator=(RBTree &&m);

 public:
  bool operator==(const RBTree &m);
  bool operator!=(const RBTree &m);

 public:
  template <typename... Args>
  s21::vector<std::pair<RBIterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<RBIterator, bool>> result;
    InsertManyRec(result, args...);
    return result;
  }

 private:
  template <typename T0, typename... Args>
  void InsertManyRec(s21::vector<std::pair<RBIterator, bool>> &vec,
                     const T0 &v0, Args &&...args) {
    if constexpr (sizeof...(args) != 0) {
      vec.push_back(Insert(v0));
      InsertManyRec(vec, args...);
    } else
      vec.push_back(Insert(v0));
  }

 public:
  std::pair<RBIterator, bool> Insert(const Type &val);
  void Erase(const Type &val);
  void Erase(RBIterator node);
  bool Empty() const noexcept;
  size_t Size() const noexcept;
  bool Contains(const Type &val) const noexcept;  // return Find
  size_t Count(const Type &val) const noexcept;
  RBIterator Find(const Type &val) const noexcept;
  void Clear();
  void Swap(RBTree &other);

 public:
  std::pair<RBIterator, RBIterator> equal_range(const Type &key);
  RBIterator lower_bound(const Type &key);
  RBIterator upper_bound(const Type &key);

 public:
  RBIterator begin() const;
  RBIterator end() const;

 private:
  Node<Type> *Find(Node<Type> *node, const Type &val) const;
  Node<Type> *Grandfather(Node<Type> *node);
  Node<Type> *Uncle(Node<Type> *node);
  bool HasRedSon(Node<Type> *node);
  bool LeftSon(Node<Type> *node);

 private:
  void RightRotation(Node<Type> *node);
  void LeftRotation(Node<Type> *node);

 private:
  std::pair<RBIterator, bool> Insert(Node<Type> *node, const Type &val);
  void Erase(Node<Type> *node);

 private:
  void BalanceAfterInsert(Node<Type> *node);
  void BalanceAfterErase(Node<Type> *node, bool eraseLeftSon);

 private:
  void Free(const Node<Type> *node);

 private:
  Node<Type> *root_;
};

template <typename Type>
RBTree<Type>::RBTree() : root_(nullptr) {}

template <typename Type>
RBTree<Type>::~RBTree() {
  if (root_) {
    Free(root_);
    root_ = nullptr;
  }
}

template <typename Type>
RBTree<Type>::RBTree(const RBTree &m) : root_(nullptr) {
  if ((*this) != m) {
    auto it = m.begin();
    while (it != m.end()) {
      Insert(*(it));
      ++it;
    }
  }
}

template <typename Type>
void RBTree<Type>::operator=(const RBTree &m) {
  if ((*this) != m) {
    auto it = m.begin();
    while (it != m.end()) {
      Insert(*(it));
      ++it;
    }
  }
}

template <typename Type>
void RBTree<Type>::operator=(RBTree &&m) {
  if ((*this) != m) {
    Clear();
    root_ = std::move(m.root_);
    m.root_ = nullptr;
  }
}

template <typename Type>
bool RBTree<Type>::operator==(const RBTree &m) {
  return root_ == m.root_;
}

template <typename Type>
bool RBTree<Type>::operator!=(const RBTree &m) {
  return root_ != m.root_;
}

template <typename Type>
void RBTree<Type>::Free(const Node<Type> *node) {
  if (node->left) Free(node->left);
  if (node->right) Free(node->right);
  delete node;
}

template <typename Type>
void RBTree<Type>::Clear() {
  if (root_) Free(root_);
  root_ = nullptr;
}

template <typename Type>
void RBTree<Type>::Swap(RBTree &other) {
  std::swap(root_, other.root_);
}

template <typename Type>
std::pair<typename RBTree<Type>::RBIterator, bool> RBTree<Type>::Insert(
    const Type &val) {
  std::pair<typename RBTree<Type>::RBIterator, bool> result{end(), false};
  if (!root_) {
    root_ = new Node<Type>{val, color::BLACK, nullptr, nullptr, nullptr};
    result = {root_, true};
  } else
    result = Insert(root_, val);

  while (root_->parent) root_ = root_->parent;

  return result;
}

template <typename Type>
bool RBTree<Type>::Contains(const Type &val) const noexcept {
  return Find(val) != end();
}

template <typename Type>
size_t RBTree<Type>::Count(const Type &val) const noexcept {
  int count = 0;
  RBIterator iter = Find(val);
  if (iter) {
    RBIterator next(iter);
    while (iter && *iter == val) next = iter--;
    while (next && (*next) == val) {
      ++count;
      ++next;
    }
  } else
    count = 0;
  return count;
}

template <typename Type>
typename RBTree<Type>::RBIterator RBTree<Type>::Find(
    const Type &val) const noexcept {
  Node<Type> *result = nullptr;

  if (root_) result = Find(root_, val);

  return result ? result : end();
}

template <typename Type>
void RBTree<Type>::Erase(const Type &val) {
  Node<Type> *erasedNode = Find(root_, val);

  if (erasedNode) Erase(erasedNode);

  while (root_->parent) root_ = root_->parent;
}

template <typename Type>
void RBTree<Type>::Erase(RBIterator node) {
  Node<Type> *erasedNode = node.operator->();

  if (erasedNode) Erase(erasedNode);

  while (root_ && root_->parent) root_ = root_->parent;
}

template <typename Type>
bool RBTree<Type>::Empty() const noexcept {
  return root_ == nullptr;
}

template <typename Type>
size_t RBTree<Type>::Size() const noexcept {
  size_t c = 0;
  for (auto it = begin(); it != end(); ++it) ++c;

  return c;
}

template <typename Type>
std::pair<typename RBTree<Type>::RBIterator, typename RBTree<Type>::RBIterator>
RBTree<Type>::equal_range(const Type &key) {
  std::pair<RBIterator, RBIterator> result{end(), end()};

  RBIterator iter = Find(key);
  if (iter) {
    RBIterator next(iter);
    while (iter && *iter == key) next = iter--;

    result.first = next;

    while (next && (*next) == key) ++next;
    result.second = next;
  }
  return result;
}

template <typename Type>
typename RBTree<Type>::RBIterator RBTree<Type>::lower_bound(const Type &key) {
  RBIterator result(root_);

  while (result && (*result) > key) --result;

  if (!result) return begin();

  while (result && (*result < key)) ++result;

  return result;
}

template <typename Type>
typename RBTree<Type>::RBIterator RBTree<Type>::upper_bound(const Type &key) {
  RBIterator result(root_);

  while (result && (*result) > key) --result;

  if (!result) return begin();

  while (result && (*result <= key)) ++result;

  return result;
}

template <typename Type>
typename RBTree<Type>::RBIterator RBTree<Type>::begin() const {
  Node<Type> *temp = root_;

  if (temp)
    while (temp->left) temp = temp->left;
  else
    return {temp, true};

  return temp;
}

template <typename Type>
typename RBTree<Type>::RBIterator RBTree<Type>::end() const {
  Node<Type> *temp = root_;

  if (temp)
    while (temp->right) temp = temp->right;

  return RBIterator{temp, true};
}

template <typename Type>
Node<Type> *RBTree<Type>::Find(Node<Type> *node, const Type &val) const {
  Node<Type> *result = nullptr;

  if (node->val < val) {
    if (node->right) result = Find(node->right, val);
  } else if (node->val > val) {
    if (node->left) result = Find(node->left, val);
  } else
    result = node;
  return result;
}

template <typename Type>
Node<Type> *RBTree<Type>::Grandfather(Node<Type> *node) {
  Node<Type> *result = nullptr;
  if (node->parent) result = node->parent->parent;

  return result;
}

template <typename Type>
Node<Type> *RBTree<Type>::Uncle(Node<Type> *node) {
  Node<Type> *result = nullptr;

  Node<Type> *grandfather = Grandfather(node);
  if (grandfather)
    result = grandfather->left == node->parent ? grandfather->right
                                               : grandfather->left;

  return result;
}

template <typename Type>
bool RBTree<Type>::HasRedSon(Node<Type> *node) {
  bool result = false;

  if ((node->right && node->right->c == color::RED) ||
      (node->left && node->left->c == color::RED))
    result = true;

  return result;
}

template <typename Type>
void RBTree<Type>::RightRotation(Node<Type> *node) {
  if (node->parent) {
    if (LeftSon(node))
      node->parent->left = node->left;
    else
      node->parent->right = node->left;
  }
  node->left->parent = node->parent;
  node->parent = node->left;
  node->left = node->left->right;
  node->parent->right = node;
  if (node->left) node->left->parent = node;
}

template <typename Type>
void RBTree<Type>::LeftRotation(Node<Type> *node) {
  if (node->parent) {
    if (LeftSon(node))
      node->parent->left = node->right;
    else
      node->parent->right = node->right;
  }
  node->right->parent = node->parent;
  node->parent = node->right;
  node->right = node->right->left;
  node->parent->left = node;
  if (node->right) node->right->parent = node;
}

template <typename Type>
bool RBTree<Type>::LeftSon(Node<Type> *node) {
  return node->parent->left == node;
}

template <typename Type>
std::pair<typename RBTree<Type>::RBIterator, bool> RBTree<Type>::Insert(
    Node<Type> *node, const Type &val) {
  if (node->val < val) {
    if (node->right)
      return Insert(node->right, val);
    else {
      node->right = new Node<Type>(val, color::RED, node, nullptr, nullptr);
      Node<Type> *rightNode = node->right;
      BalanceAfterInsert(node->right);
      return {rightNode, true};
    }
  } else {
    if (node->left)
      return Insert(node->left, val);
    else {
      node->left = new Node<Type>(val, color::RED, node, nullptr, nullptr);
      Node<Type> *leftNode = node->left;
      BalanceAfterInsert(node->left);
      return {leftNode, true};
    }
  }
}

template <typename Type>
void RBTree<Type>::BalanceAfterInsert(Node<Type> *node) {
  if (node->parent == nullptr)
    node->c = color::BLACK;
  else if (node->parent->c == color::RED) {
    if (Uncle(node) && Uncle(node)->c == color::RED) {
      Uncle(node)->c = color::BLACK;
      Grandfather(node)->c = color::RED;
      node->parent->c = color::BLACK;
      BalanceAfterInsert(Grandfather(node));
    } else if (LeftSon(node)) {
      if (LeftSon(node->parent)) {
        Grandfather(node)->c = color::RED;
        node->parent->c = color::BLACK;
        RightRotation(Grandfather(node));
      } else {
        RightRotation(node->parent);
        BalanceAfterInsert(node->right);
      }
    } else {
      if (!LeftSon(node->parent)) {
        Grandfather(node)->c = color::RED;
        node->parent->c = color::BLACK;
        LeftRotation(Grandfather(node));
      } else {
        LeftRotation(node->parent);
        BalanceAfterInsert(node->left);
      }
    }
  }
}

template <typename Type>
void RBTree<Type>::Erase(Node<Type> *erasedNode) {
  if (!erasedNode->left && !erasedNode->right) {
    if (!erasedNode->parent) {
      delete erasedNode;
      root_ = nullptr;
    } else if (erasedNode->c == color::RED)  // checked
    {
      if (LeftSon(erasedNode))
        erasedNode->parent->left = nullptr;
      else
        erasedNode->parent->right = nullptr;
      delete erasedNode;
    } else  // not checked
    {
      Node<Type> *erasedNodeParent = erasedNode->parent;
      bool erasedNodeLeftSon = LeftSon(erasedNode);

      if (LeftSon(erasedNode))
        erasedNode->parent->left = nullptr;
      else
        erasedNode->parent->right = nullptr;
      delete erasedNode;

      BalanceAfterErase(erasedNodeParent, erasedNodeLeftSon);
    }
  } else if (erasedNode->left && !erasedNode->right)  // checked
  {
    if (erasedNode->parent) {
      if (LeftSon(erasedNode))
        erasedNode->parent->left = erasedNode->left;
      else
        erasedNode->parent->right = erasedNode->left;
      erasedNode->left->parent = erasedNode->parent;
      erasedNode->left->c = color::BLACK;
      delete erasedNode;
    } else {
      erasedNode->left->parent = nullptr;
      erasedNode->left->c = color::BLACK;
      root_ = erasedNode->left;
      delete erasedNode;
    }
  } else if (!erasedNode->left && erasedNode->right)  // checked
  {
    if (erasedNode->parent) {
      if (LeftSon(erasedNode))
        erasedNode->parent->left = erasedNode->right;
      else
        erasedNode->parent->right = erasedNode->right;
      erasedNode->right->parent = erasedNode->parent;
      erasedNode->right->c = color::BLACK;
      delete erasedNode;
    } else {
      erasedNode->right->parent = nullptr;
      erasedNode->right->c = color::BLACK;
      root_ = erasedNode->right;
      delete erasedNode;
    }
  } else  // oba sina
  {
    Node<Type> *leftMaxNode = erasedNode->left;
    while (leftMaxNode->right) leftMaxNode = leftMaxNode->right;

    if (erasedNode->parent) {
      if (LeftSon(erasedNode))
        erasedNode->parent->left = leftMaxNode;
      else
        erasedNode->parent->right = leftMaxNode;
    }

    if (leftMaxNode == erasedNode->left)  // if no right grandsons
    {
      leftMaxNode->parent = erasedNode->parent;

      erasedNode->left = leftMaxNode->left;
      if (erasedNode->left) erasedNode->left->parent = erasedNode;  // deti?

      leftMaxNode->left = erasedNode;
      erasedNode->parent = leftMaxNode;

      leftMaxNode->right = erasedNode->right;
      leftMaxNode->right->parent = leftMaxNode;  // tochno imeetsya
      erasedNode->right = nullptr;               // no grandson
    } else                                       // not checked
    {
      std::swap(leftMaxNode->parent, erasedNode->parent);
      std::swap(leftMaxNode->left, erasedNode->left);    // deti?
      std::swap(leftMaxNode->right, erasedNode->right);  // deti?
      leftMaxNode->left->parent = leftMaxNode;           // tochno imeetsya
      leftMaxNode->right->parent = leftMaxNode;
      if (erasedNode->left) erasedNode->left->parent = erasedNode;  // deti?
      erasedNode->right = nullptr;  // no grandson
    }
    std::swap(leftMaxNode->c, erasedNode->c);
    root_ = leftMaxNode;
    Erase(erasedNode);
  }
}

template <typename Type>
void RBTree<Type>::BalanceAfterErase(Node<Type> *node, bool eraseLeftSon) {
  if (eraseLeftSon) {
    if (node->c == color::RED) {
      if (node->right->right && node->right->right->c == color::RED) {
        node->c = color::BLACK;
        node->right->c = color::RED;
        node->right->right->c = color::BLACK;
        LeftRotation(node);
      } else if (node->right->left && node->right->left->c == color::RED) {
        RightRotation(node->right);
        LeftRotation(node);
        node->c = color::BLACK;
      } else {
        node->right->c = color::RED;
        node->c = color::BLACK;
      }
    } else {
      if (node->right->c == color::RED) {
        Node<Type> *left_grandson = node->right->left;
        if (left_grandson->right && left_grandson->right->c == color::RED) {
          left_grandson->right->c = color::BLACK;
          RightRotation(node->right);
          LeftRotation(node);
        } else if (left_grandson->left &&
                   left_grandson->left->c == color::RED) {
          RightRotation(node->right);
          LeftRotation(node);
        } else {
          node->right->c = color::BLACK;
          left_grandson->c = color::RED;
          LeftRotation(node);
        }
      } else {
        if (HasRedSon(node->right)) {
          if (node->right->right && node->right->right->c == color::RED) {
            node->right->right->c = color::BLACK;
            // RightRotation(left);
            LeftRotation(node);
          } else {
            node->right->left->c = color::BLACK;
            RightRotation(node->right);
            LeftRotation(node);
          }
        } else {
          node->right->c = color::RED;
          if (node->parent) BalanceAfterErase(node->parent, LeftSon(node));
        }
      }
    }
  } else {
    if (node->c == color::RED) {
      if (node->left->left && node->left->left->c == color::RED)  // 1
      {
        node->c = color::BLACK;
        node->left->c = color::RED;
        node->left->left->c = color::BLACK;
        RightRotation(node);
      } else if (node->left->right && node->left->right->c == color::RED)  // 2
      {
        LeftRotation(node->left);
        RightRotation(node);
        node->c = color::BLACK;
      } else  // 3
      {
        node->left->c = color::RED;
        node->c = color::BLACK;
      }
    } else {
      if (node->left->c == color::RED) {
        Node<Type> *right_grandson = node->left->right;
        if (right_grandson->left &&
            right_grandson->left->c == color::RED)  // 4.1
        {
          right_grandson->left->c = color::BLACK;
          LeftRotation(node->left);
          RightRotation(node);
        } else if (right_grandson->right &&
                   right_grandson->right->c == color::RED)  // 4.2
        {
          LeftRotation(node->left);
          RightRotation(node);
        } else  // 5
        {
          node->left->c = color::BLACK;
          right_grandson->c = color::RED;
          RightRotation(node);
        }
      } else {
        if (HasRedSon(node->left)) {
          if (node->left->left && node->left->left->c == color::RED)  // 6
          {
            node->left->left->c = color::BLACK;
            // RightRotation(left);
            RightRotation(node);
          } else  // 7 2.2.2.1
          {
            node->left->right->c = color::BLACK;
            LeftRotation(node->left);
            RightRotation(node);
          }
        } else  // 8
        {
          node->left->c = color::RED;
          if (node->parent) BalanceAfterErase(node->parent, LeftSon(node));
        }
      }
    }
  }
};
