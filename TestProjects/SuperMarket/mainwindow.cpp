#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  _countTicketOffices_i = 0;
  _useCustomVector_b = false;
  _startTimer_b = false;
  _minutesCustomersCustom_po = nullptr;
  _currentTime_i = 0;
  _nextCustomerIndex_ui = 0;
  _simulationFinished_b = false;

  ui->setupUi(this);
  connect(ui->Accept_btn, &QPushButton::clicked, this, &MainWindow::Accept_v);
  _BaseTimer_po = new QTimer(this);
  connect(_BaseTimer_po, &QTimer::timeout, this, &MainWindow::TickProcess_v);
  _BaseTimer_po->start(1000); // 1 second interval

}

MainWindow::~MainWindow()
{
  Reset_v();
  delete ui;
}

void MainWindow::Accept_v(bool flag_b)
{
  Q_UNUSED(flag_b);
  // Обнуляем все значения
  Reset_v();
  // Переводим QTextEdit c количеством касс в переменнеую
  _countTicketOffices_i = (ui->TicketOffices->toPlainText()).toUInt();

  //Переводим QTextEdit с количеством минут для каждого покупателя в вид вектора
  QString minutesInText_str = ui->MinutesArray->toPlainText();
  QStringList minites_lst = minutesInText_str.split('\n',
                                                    Qt::SkipEmptyParts); // Пропуск путсых строк


  // Если используем вектор со своим алокатаром
  if (ui->checkBox->isChecked()) {
    _useCustomVector_b = true;
    FixedSizeAllocator<int> alloc_o(minites_lst.size());
    _minutesCustomersCustom_po = new std::vector<int, FixedSizeAllocator<int>>(alloc_o);
    _minutesCustomersCustom_po->reserve(minites_lst.size());

    for (QString &minute_str : minites_lst) {
      bool check_b = false;
      int value_i = minute_str.trimmed().toInt(&check_b);
      if (check_b) {
        _minutesCustomersCustom_po->push_back(value_i);
      }
    }
  }

  // Если используем вектор обычный
  else {
    _useCustomVector_b = false;
    for (QString &minute_str : minites_lst) {
      bool check_b = false;
      int value_i = minute_str.trimmed().toInt(&check_b);
      if (check_b) {
        _minutesCustomers_vec.push_back(value_i);
      }
    }
  }

  // Инициализация симуляции
  _cashiers_vec = std::vector<int>(_countTicketOffices_i, 0);
  _currentTime_i = 0;
  _simulationFinished_b = false;

  // Добавляем первых покупателей к кассе
  for (int i = 0; i < _countTicketOffices_i; i++) {
    if (_nextCustomerIndex_ui < getTotalCustomers()) {
      _cashiers_vec[i] = getCustomerTime(_nextCustomerIndex_ui);
      _nextCustomerIndex_ui++;
    }
  }

  _startTimer_b = true;
  UpdateUI_v();
}

int MainWindow::getCustomerTime(int index)
{
  if (_useCustomVector_b) {
    if (_minutesCustomersCustom_po && index < _minutesCustomersCustom_po->size())
      return (*_minutesCustomersCustom_po)[index];
    return 0;
  } else {
    if (index < _minutesCustomers_vec.size())
      return _minutesCustomers_vec[index];
    return 0;
  }
}

int MainWindow::getAllCustomerTimeInQueue()
{
  int sum_i = 0;
  if (_useCustomVector_b) {
    sum_i = std::accumulate(_minutesCustomersCustom_po->begin() + _nextCustomerIndex_ui,
                            _minutesCustomersCustom_po->end(),
                            0);
    return sum_i;
  }
  sum_i = std::accumulate(_minutesCustomers_vec.begin() + _nextCustomerIndex_ui, _minutesCustomers_vec.end(), 0);
  return sum_i;
}


int MainWindow::getTotalCustomers()
{
  if (_useCustomVector_b) {
    if (_minutesCustomersCustom_po)
      return _minutesCustomersCustom_po->size();
    return 0;
  } else {
    return _minutesCustomers_vec.size();
  }
}

int MainWindow::queueTime_i(int countOffices, std::vector<int> minutesCustomers)
{
  return 0;
}

int MainWindow::queueTimeCustom_i(int countOffices,
                                  std::vector<int, FixedSizeAllocator<int>> minutesCustomersCustom)
{
  return 0;
}

void MainWindow::TickProcess_v()
{
  if (_startTimer_b && !_simulationFinished_b) {
    // Decrease time for all busy cashiers
    for (int i = 0; i < _cashiers_vec.size(); i++) {
      if (_cashiers_vec[i] > 0) {
        _cashiers_vec[i]--;
      }
    }

    // Запускаем нового покупателя к кассе
    for (int i = 0; i < _cashiers_vec.size(); i++) {
      if (_cashiers_vec[i] == 0 && _nextCustomerIndex_ui < getTotalCustomers()) {
        _cashiers_vec[i] = getCustomerTime(_nextCustomerIndex_ui);
        _nextCustomerIndex_ui++;
      }
    }

    _currentTime_i++;

    // Проверяем если симуляция закончилась
    _simulationFinished_b = true;
    if (_nextCustomerIndex_ui < getTotalCustomers()) {
      _simulationFinished_b = false;
    } else {
      for (int time : _cashiers_vec) {
        if (time > 0) {
          _simulationFinished_b = false;
          break;
        }
      }
    }

    if (_simulationFinished_b) {
      ui->plainTextEdit->appendPlainText(QString("\nTotal time: %1 seconds").arg(_currentTime_i));
    }

    UpdateUI_v();
  }
}

void MainWindow::UpdateUI_v()
{
  QString status;
  status += QString("Current time: %1 seconds\n").arg(_currentTime_i);
  status += "Cashiers status:\n";

  int sumTime_i = getAllCustomerTimeInQueue();
  status += QString("Current count time in queue %1\n").arg(sumTime_i);


  for (int i = 0; i < _cashiers_vec.size(); i++) {
    status += QString("  Cashier %1: ").arg(i+1);
    if (_cashiers_vec[i] > 0) {
      status += QString("busy, %1 seconds left\n").arg(_cashiers_vec[i]);
    } else {
      status += "free\n";
    }
  }

  status += "\nQueue status:\n";
  int remaining = getTotalCustomers() - _nextCustomerIndex_ui;
  status += QString("  Remaining customers: %1\n").arg(remaining);

  ui->plainTextEdit->setPlainText(status);
}

void MainWindow::Reset_v()
{
  _countTicketOffices_i = 0;
  _minutesCustomers_vec.clear();
  _startTimer_b = false;
  _useCustomVector_b = false;
  _currentTime_i = 0;
  _nextCustomerIndex_ui = 0;
  _simulationFinished_b = false;
  _cashiers_vec.clear();

  if (_minutesCustomersCustom_po != nullptr) {
    delete _minutesCustomersCustom_po;
    _minutesCustomersCustom_po = nullptr;
  }
}
