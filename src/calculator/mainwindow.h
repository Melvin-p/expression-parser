#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Interpreter.hpp"

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_one_clicked();

  void on_two_clicked();

  void on_three_clicked();

  void on_four_clicked();

  void on_five_clicked();

  void on_six_clicked();

  void on_seven_clicked();

  void on_eight_clicked();

  void on_nine_clicked();

  void on_zero_clicked();

  void on_decimal_clicked();

  void on_multiply_clicked();

  void on_divide_clicked();

  void on_plus_clicked();

  void on_minus_clicked();

  void on_power_clicked();

  void on_back_clicked();

  void on_clear_clicked();

  void on_reset_clicked();

  void on_evaulate_clicked();

private:
  Ui::MainWindow *ui;
  Interpreter interpreter{};
  void update_table();
};

#endif
