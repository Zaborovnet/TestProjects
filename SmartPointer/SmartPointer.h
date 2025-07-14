#include <iostream>

template <typename T>
class SmartPointer {
private:
  // Вспомогательная структура для управления объектом и счётчиком ссылок
  struct ControlBlock {
    T* ptr;         // Указатель на управляемый объект
    int ref_count;  // Счётчик ссылок

    ControlBlock(T* p) : ptr(p), ref_count(1) {}

    ~ControlBlock() {
      delete ptr;  // Освобождаем память объекта при уничтожении
    }
  };

  ControlBlock* block;  // Указатель на блок управления

  // Вспомогательный метод для уменьшения счётчика и очистки
  void release() {
    if (!block) return;

    // Уменьшаем счётчик ссылок
    block->ref_count--;

    // Если ссылок больше нет - уничтожаем блок управления
    if (block->ref_count == 0) {
        delete block;
      }

    block = nullptr;  // Обнуляем указатель
  }

public:
  // Конструктор из обычного указателя
  explicit SmartPointer(T* p = nullptr) {
    if (p) {
        block = new ControlBlock(p);
      } else {
        block = nullptr;
      }
  }

  // Конструктор копирования
  SmartPointer(const SmartPointer& other) : block(other.block) {
    if (block) {
        block->ref_count++;  // Увеличиваем счётчик ссылок
      }
  }

  // Оператор присваивания
  SmartPointer& operator=(const SmartPointer& other) {
    if (this != &other) {  // Защита от самоприсваивания
        release();         // Освобождаем текущий ресурс

        block = other.block;  // Копируем указатель на блок

        if (block) {
            block->ref_count++;  // Увеличиваем счётчик
          }
      }
    return *this;
  }

  // Деструктор
  ~SmartPointer() {
    release();
  }

   // Оператор разыменования
  T& operator*() const {
    if (block && block->ptr) {
        return *(block->ptr);
      }
    throw std::runtime_error("Dereferencing null pointer");
  }

   // Оператор доступа к членам класса
  T* operator->() const {
    if (block && block->ptr) {
        return block->ptr;
      }
    throw std::runtime_error("Accessing null pointer");
  }

  // Получение количества ссылок
  int use_count() const {
    return block ? block->ref_count : 0;
  }

  // Получение сырого указателя
  T* get() const {
    return block ? block->ptr : nullptr;
  }

  // Проверка на наличие объекта
  explicit operator bool() const {
    return block && block->ptr;
  }
};
