#pragma once

#include <cassert>

template <typename T>
class ListNode {
  public:
  private:
    ListNode<T>* prev_{nullptr};
    ListNode<T>* next_{nullptr};
};
