#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->table->setColumnCount(3);
  const QStringList headers{"Variable", "Type", "Value"};
  ui->table->setHorizontalHeaderLabels(headers);
  update_table();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_one_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("1");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_two_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("2");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_three_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("3");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_four_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("4");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_five_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("5");
  ui->display->setTextCursor(prev_cursor);


}


void MainWindow::on_six_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("6");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_seven_clicked()
{
  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("7");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_eight_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("8");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_nine_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("9");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_zero_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("0");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_decimal_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText (".");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_multiply_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("*");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_divide_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("/");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_plus_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("+");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_minus_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("-");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_power_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  ui->display->insertPlainText ("^");
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_back_clicked()
{

  QTextCursor prev_cursor = ui->display->textCursor();
  ui->display->moveCursor(QTextCursor::End);
  prev_cursor.deletePreviousChar();
  ui->display->setTextCursor(prev_cursor);

}


void MainWindow::on_clear_clicked()
{

  ui->display->selectAll();
  ui->display->textCursor().removeSelectedText();

}


void MainWindow::on_reset_clicked()
{

  ui->display->clear();
  ui->history->clear();
  interpreter.reset();

}


void MainWindow::on_evaulate_clicked()
{


  auto raw_input = ui->display->toPlainText();
  ui->display->selectAll();
  ui->display->textCursor().removeSelectedText();

  qDebug() << raw_input << "\n";

  // add semi colon at end of each string
  std::string input{raw_input.toStdString()};
  auto it = input.begin();
  while((it = std::find(it, input.end(), '\n')) != input.end()){
    it = input.insert(it, ';') +2;
  }
  if(!input.ends_with(';')){
    input.push_back(';');
  }
  qDebug() << input << "\n";

  ui->display->selectAll();
  ui->display->textCursor().insertText(QString(input.c_str()));

  std::string output{""};
  try{
    output += interpreter.evaluate(input);
  }
  catch(const std::exception &e){
    output += e.what();
  }

  ui->history->appendPlainText(QString(output.c_str()).trimmed());

  ui->display->selectAll();
  ui->display->textCursor().removeSelectedText();
  update_table();

}

void MainWindow::update_table(){
  auto symbol_table = interpreter.getSymbolTable();

  ui->table->setRowCount(0);
  ui->table->setRowCount(symbol_table.size());

  auto &tmp_ui = ui;

  int count{0};
  for(auto iter = symbol_table.begin(); iter != symbol_table.end(); ++iter){

    auto is_bool = [&tmp_ui, &iter, &count](bool val){
      QString var_name{iter->first.c_str()};
      QString bool_val;
      if(val){
        bool_val = "true";
      }
      else{
        bool_val = "false";
      }
      tmp_ui->table->setItem(count, 0, new QTableWidgetItem(var_name));
      tmp_ui->table->setItem(count, 1, new QTableWidgetItem("bool"));
      tmp_ui->table->setItem(count, 2, new QTableWidgetItem(bool_val));
    };

    auto is_double = [&tmp_ui, &iter, &count](double val){
      QString var_name{iter->first.c_str()};
      QString double_val{QString::number(val)};
      tmp_ui->table->setItem(count, 0, new QTableWidgetItem(var_name));
      tmp_ui->table->setItem(count, 1, new QTableWidgetItem("double"));
      tmp_ui->table->setItem(count, 2, new QTableWidgetItem(double_val));
    };

    std::visit(overloads{is_bool, is_double}, iter->second);

    ++count;
  }
}

