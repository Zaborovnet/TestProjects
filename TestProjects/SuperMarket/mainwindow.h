#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FixedSizeAllocator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  private:
  void Reset_v();                         ///@brief сброс всех параметров
  void TickProcess_v();                   ///@brief Выполняем обновление
  int queueTime_i(int countOffices, std::vector<int> minutesCustomers); ///@brief получаем количество минут для очереди покупателей
  int queueTimeCustom_i(int countOffices, std::vector<int, FixedSizeAllocator<int>> minutesCustomersCustom); ///@brief получаем количество минут для очереди покупателей
  void UpdateUI_v();
  int getCustomerTime(int index);   ///@brief получение времени обработки покупателя
  int getTotalCustomers(); ///@brief получение общего количества покупателей
  int getAllCustomerTimeInQueue(); ///@brief получение общего количества покупателей



  Ui::MainWindow *ui;                     ///@brief указатель на UI
  unsigned int _countComletePeople_ui;    ///@brief количество людей, которые прошли
  unsigned int _countAllPeople_ui;        ///@brief количество всех людей
  QTimer *_BaseTimer_po;                  ///@brief указатель на таймер
  int _countTicketOffices_i;              ///@brief количество автоматов (касс)
  std::vector<int> _minutesCustomers_vec; ///@brief вектор минут относительно каждого в очереде
  std::vector<int, FixedSizeAllocator<int>> * _minutesCustomersCustom_po; ///@brief вектор минут относительно каждого в очереде (со своим аллокатором)

  bool _useCustomVector_b; ///@brief исполоьзование _minutesCustomersCustom_po
  bool _startTimer_b; ///@brief запуск отсчёта очереди

  int _currentTime_i;
  std::vector<int> _cashiers_vec;
  unsigned int _nextCustomerIndex_ui;
  bool _simulationFinished_b;


  private slots:
  void Accept_v(bool flag_b);


};
#endif // MAINWINDOW_H
