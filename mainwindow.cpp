#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEdit_find, SIGNAL(textChanged(const QString &)), this, SLOT(do_search_and_replace(QString)));
    ui->lineEdit_find->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_find_textChanged(const QString &arg1)
{

}

void MainWindow::on_btn_previous_clicked()
{

}

void MainWindow::on_btn_next_clicked()
{

}

void MainWindow::on_btn_replace_clicked()
{

}

void MainWindow::on_btn_replace_all_clicked()
{

}

//
// search & replace:
// do_search_and_replace() - search for matching word
//
void MainWindow::do_search_and_replace(QString action_str)
{
    qDebug() <<  "do_search_and_replace()";
    // just to be sure...
    if(action_str.isEmpty())
        action_str == "0";

    int action_nr = action_str.toInt();    // convert argument to int, so we can switch() on it...
    QString text = ui->lineEdit_find->text();
    qDebug() <<  "action_nr: " << action_nr;

    if (!( text.isEmpty() ))
    {
        qDebug() << text;
        ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, 0, QsciScintilla::INDIC_FULLBOX);
        ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETFORE,0, QColor(Qt::darkBlue));

        QString docText = ui->textEdit->text();
        int end = docText.lastIndexOf(text);
        int cur = -1;

        if(end != -1)
        {
            while(cur != end)
            {
                cur = docText.indexOf(text,cur+1);
                ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICATORFILLRANGE,cur,
                    text.length());
            }
        }
    }
}
