#include <cstdlib>
#include <memory>
#include <vector>

template <typename T>
class FixedSizeAllocator {
  public:
  using value_type = T;

  explicit FixedSizeAllocator(size_t capacity)
    : _capacity_u(capacity), _buffer_po(nullptr), _allocated_u(0) {}

  template <typename U>
  FixedSizeAllocator(const FixedSizeAllocator<U>& other)
    : _capacity_u(other._capacity_u), _buffer_po(nullptr), _allocated_u(0) {}

  T* allocate(size_t n) {
    // Создаём вектор
    if (_buffer_po == nullptr) {
      // Если количество элементов больше чем максимальное количество контейнера выкидываем bad_alloc
      if (n > _capacity_u)
        throw std::bad_alloc();
      // Выделяем блок памяти размером _capacity_u * sizeof(T)
      _buffer_po = static_cast<T*>(std::malloc(_capacity_u * sizeof(T)));
      if (!_buffer_po)
        throw std::bad_alloc();
      // Количество нужных элементов
      _allocated_u = n;
      return _buffer_po;
    }
    // Выделяем память из уже созданног блока (если осталось место)
    if (_allocated_u + n > _capacity_u)
      throw std::bad_alloc();

    // Возвращаем указатель на следующую свободную позицию
    T* ptr = _buffer_po + _allocated_u;
    _allocated_u += n;
    return ptr;
  }

  void deallocate(T* p, size_t n) noexcept {
    // Освобождаем только при разрушении аллокатора
  }

  size_t capacity() const { return _capacity_u; }

  ~FixedSizeAllocator() {
    std::free(_buffer_po);
  }

  private:
  size_t _capacity_u;     ///< @brief Максимальное количество элементов
  T* _buffer_po;          ///< @brief Указатель на выделенный блок памяти
  size_t _allocated_u;    ///< @brief Количество уже выделенных элементов (счётчик)
};

// Для совместимости с STL
template <typename T, typename U>
bool operator==(const FixedSizeAllocator<T>&, const FixedSizeAllocator<U>&) {
  return true;
}

template <typename T, typename U>
bool operator!=(const FixedSizeAllocator<T>& a, const FixedSizeAllocator<U>& b) {
  return !(a == b);
}
