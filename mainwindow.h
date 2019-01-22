#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciscintillabase.h>
#include <Qsci/qscilexercpp.h>

#include <search.h>

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
    void loadFile(const QString &fileName);
    void fitMarginLines();
    void call_do_search_and_replace();

private:
    Ui::MainWindow *ui;
    const bool p_mydebug = true;
};

#endif // MAINWINDOW_H
