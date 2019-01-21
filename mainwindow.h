#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_find_textChanged(const QString &arg1);
    void on_btn_previous_clicked();
    void on_btn_next_clicked();
    void on_btn_replace_clicked();
    void on_btn_replace_all_clicked();
    void do_search_and_replace(QString action_str);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
