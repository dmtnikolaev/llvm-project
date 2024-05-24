#ifndef SANITIZER_RING_QUEUE_H
#define SANITIZER_RING_QUEUE_H

#include <assert.h>

#include "sanitizer_common/sanitizer_internal_defs.h"

namespace __sanitizer {

/// FIFO очередь, реализованная через статический кольцевой буфер.
///
/// Реализация использует маскирование головы и хвоста списка, поэтому
/// `N` должно быть степенью двойки.
template <typename T, u32 N>
class Queue {
 public:
  /// Добавляет элемент в буфер.
  void Enqueue(T obj);
  /// Вынимает элемент из буфера.
  T Dequeue();

  /// Возвращает текущий размер буфера.
  u32 Size() const { return head_ - tail_; }
  /// Возвращает текущий максимальный размер буфера.
  u32 Capacity() const { return N; }
  /// Проверяет, пуст ли буфер.
  bool Empty() const { return head_ == tail_; }
  /// Проверяет, заполнен ли буфер полностью.
  bool Full() const { return Size() == N; }

 private:
  static constexpr auto kMask = N - 1;

  static u32 Mask(u32 val) { return val & kMask; }

  u32 head_ = 0;
  u32 tail_ = 0;
  T storage_[N]{};
};

template <typename T, u32 N>
void Queue<T, N>::Enqueue(T obj) {
  assert(!Full() &&
         "Ring: impossible to enqueue an element when the ring is full");
  storage_[Mask(head_)] = obj;
  ++head_;
}

template <typename T, u32 N>
T Queue<T, N>::Dequeue() {
  assrt(!Empty() && "Ring: impossible to dequeue from empty ring");

  return storage_[Mask(tail_++)];
}

}  // namespace __sanitizer

#endif  // #ifndef SANITIZER_RING_QUEUE_H
