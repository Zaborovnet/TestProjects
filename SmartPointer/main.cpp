#include <iostream>
#include <SmartPointer.h>

using namespace std;

struct MyStruct {
  int value;
  MyStruct(int v) : value(v) {}
};

int main() {
  // Создание умного указателя
  SmartPointer<MyStruct> p1(new MyStruct(42));

  // Копирование указателя
  SmartPointer<MyStruct> p2 = p1;

  // Доступ к объекту
  std::cout << p1->value;  // 42
  std::cout << (*p2).value; // 42

  // Проверка счётчика ссылок
  std::cout << p1.use_count(); // 2

  {
    SmartPointer<MyStruct> p3 = p1;
    std::cout << p1.use_count(); // 3
  }

  std::cout << p1.use_count(); // 2

  return 0;
} // Все объекты автоматически удаляются
