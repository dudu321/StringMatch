#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <sstream>
#include <QMessageBox>
#include <QElapsedTimer>
#include <map>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bt_reset_clicked()
{
    ui->t_input->setText("");
    ui->t_output->setText("");
    ui->status_1->setText("Ready.");
}

using namespace std;
vector<QString> history;
int f(string str, stringstream &sout, int width);
void MainWindow::on_bt_solve_clicked()
{
    string str = ui->t_input->toPlainText().toStdString();
    if(str.empty()) {
        QMessageBox::information(this, "infomation", "empty input: please enter an vaild string.");
        return;
    }
    for(int i = 0; i < str.size(); i++) if(str[i] < '0' || str[i] > '9') {
        QMessageBox::information(this, "infomation", "invaild string: only numbers are allowed.");
        return;
    }
    if(str.length() > 600) if(QMessageBox::question(this, "query",
             "huge input: it may takes very long time to process, continue?",
              QMessageBox::Yes|QMessageBox::No) == QMessageBox::No) return;

    QElapsedTimer timer;
    timer.start();

    stringstream sout;
    int r = f(str, sout, ui->s_width->value());

    string left, right, res;
    sout >> right >> left;
    reverse(left.begin(), left.end());
    if(r == 0) left += "1";
    res = left + "[.......input.......]" + right;

    history.push_back(QString::fromStdString(res));

    ui->t_output->setText(QString::fromStdString(res));
    ui->status_1->setText("String matched. Elapsed time: " + QString::number(timer.elapsed()) + "ms.");
    ui->hlist->addItem(ui->t_input->toPlainText());
    ui->status_2->setText(QString::number(ui->hlist->count()) + " record(s).");
}

void MainWindow::on_t_input_textChanged()
{
    auto l = ui->t_input->toPlainText().length();
    if(l == 0) ui->status_1->setText("Ready.");
    else ui->status_1->setText("Input: " + QString::number(l) + "character(s).");

}


void MainWindow::on_hlist_itemClicked(QListWidgetItem *item)
{
    int i = ui->hlist->row(ui->hlist->selectedItems().at(0));
    ui->hout->setText(history[i]);
    ui->hin->setText(ui->hlist->selectedItems().at(0)->text());
}
