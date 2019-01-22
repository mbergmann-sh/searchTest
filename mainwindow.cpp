#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // give a file to load and load it into ui->textEdit:
    QString fileName = "moc_mainwindow.cpp";
    QFontMetrics fontmetrics = QFontMetrics(ui->textEdit->font());

    // set a readable default font for Linux and Windows:
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        QFont font("Courier New", 10);
        #elif defined(__APPLE__)
        QFont font("SF Mono Regular", 11);
        if(p_mydebug)
            qDebug() << "Running on Mac. Font is SF Mono Regular now!";
    #elif defined(__unix__)
        QFont font("Source Code Pro", 9);
        if(p_mydebug)
            qDebug() << "Linux detected. Setting font to Source Code Pro";
    #endif

    // set margins, so line numbering will fit:
    QFont myfont = font;
    myfont.setFixedPitch(true);
    ui->textEdit->setFont(myfont);
    ui->textEdit->setMarginsFont(ui->textEdit->font());
    ui->textEdit->setMarginWidth(0, fontmetrics.width(QString::number(ui->textEdit->lines())) + 10);
    ui->textEdit->setMarginLineNumbers(0, true);

    // show line numbers
    ui->textEdit->setMarginLineNumbers(0, true);
    ui->textEdit->setMarginWidth(0, fontmetrics.width(QString::number(ui->textEdit->lines())) + 10);

    // connect search string issues
    connect(ui->lineEdit_find, SIGNAL(returnPressed()), this, SLOT(call_do_search_and_replace())); // Helper slot for compatibel call!
    connect(ui->lineEdit_find, SIGNAL(textChanged(const QString &)), this, SLOT(reset_searchResult())); // Reset marked text items

    // resize line numbers margin if needed!
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(fitMarginLines()));
    loadFile(fileName);
    ui->lineEdit_find->setFocus();

    // init Lexer for C++
    QsciLexerCPP *lexer = new QsciLexerCPP();
    ui->textEdit->setLexer(lexer);
    ui->textEdit->SendScintilla(ui->textEdit->QsciScintilla::SCI_STYLESETCHARACTERSET, 1, QsciScintilla::SC_CHARSET_8859_15);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_find_textChanged(const QString &arg1)
{
    reset_searchResult();
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
// search and replace:
// Helper slot for compatible call of do_search_and_replace()
// from editingFinished()
// void call_do_search_and_replace()
//
void MainWindow::call_do_search_and_replace()
{
    do_search_and_replace("0");
}

//
// search & replace:
// do_search_and_replace() - search for matching word
//
void MainWindow::do_search_and_replace(QString action_str)
{
    int line, index;
    qDebug() <<  "do_search_and_replace()";
    // just to be sure...
    if(action_str.isEmpty())
        action_str == "0";

    int action_nr = action_str.toInt();    // convert argument to int, so we can switch() on it...
    text = ui->lineEdit_find->text();
    docText = ui->textEdit->text();
    qDebug() <<  "action_nr: " << action_nr;

    //
    // first part: Mark all occurances of search term
    //
    if (!( text.isEmpty() ))
    {
        qDebug() << text;
        ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, 0, QsciScintilla::INDIC_FULLBOX);
        ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETFORE,0, QColor(Qt::darkBlue));


        int end = docText.lastIndexOf(text);
        int cur = -1;

        if(end != -1)
        {
            ui->textEdit->getCursorPosition(&line, &index);
            qDebug() << "line: " << line << " index: " << index;
            while(cur != end)
            {
                cur = docText.indexOf(text,cur+1);
                ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICATORFILLRANGE,cur,
                    text.length());
            }
        }
    } // END text.isEmpty(), END mark ALL

    //
    // second part: Find firs occurance of search term
    //
    bool use_regular_expression, is_case_sensitive, match_whole_word_only, use_wrap, search_forward;
    use_regular_expression = false;
    is_case_sensitive = ui->checkBox_CaseSensitive->isChecked();
    match_whole_word_only = ui->checkBox_WholeWords->isChecked();
    use_wrap = true;
    search_forward = ui->radioButton_Forward->isChecked();

    ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, 0, QsciScintilla::INDIC_FULLBOX);
    //ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETFORE,0, QColor(Qt::darkBlue));

    bool found = ui->textEdit->findFirst(text, use_regular_expression, is_case_sensitive, match_whole_word_only, use_wrap, search_forward);
    qDebug() <<  "START: found = " << found;
    while(found)
    {
        ui->textEdit->getCursorPosition(&line, &index);

        qDebug() << "line: " << line << " index: " << index;
        qDebug() << text;

        // pattern: found = findFirst(pattern, use_regular_expression, is_case_sensitive, match_whole_word_only, use_wrap, search_forward)
        //found = ui->textEdit->findFirst(text, use_regular_expression, is_case_sensitive, match_whole_word_only, use_wrap, search_forward);

        if(found)
        {
            ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICATORFILLRANGE, line, text.length());
            int start = ui->textEdit->positionFromLineIndex(line, index);
            int end = ui->textEdit->positionFromLineIndex(line, index + text.length());
            qDebug() << "line: " << line << " start: " << start << " end: " << end;

//            found = ui->textEdit->findNext();
//            ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICATORFILLRANGE, line, text.length());

        }

        found = false;
    }
}

void MainWindow::loadFile(const QString &fileName)
{

    qDebug() << "loadFile() called with parameter: " << fileName;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, tr("searchTest"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::fitMarginLines()
{
    QFontMetrics fontmetrics = ui->textEdit->fontMetrics();
    ui->textEdit->setMarginWidth(0, fontmetrics.width(QString::number(ui->textEdit->lines())) + 10);
}

void MainWindow::reset_searchResult()
{
    int line, index;
    qDebug() <<  "in: reset_searchResult()";

    //QString text = ui->lineEdit_find->text();
    text.clear();
    docText.clear();

    //
    // first part: Mark all occurances of search term
    //
    if (( text.isEmpty() ))
    {
        qDebug() << text;
        ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, 0, QsciScintilla::INDIC_PLAIN);
        ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICSETFORE,0, QColor(Qt::white));

        //docText = text;
        docText = ui->lineEdit_find->text();
        int end = docText.lastIndexOf(text);
        int cur = -1;

        if(end != -1)
        {
            ui->textEdit->getCursorPosition(&line, &index);
            qDebug() << "line: " << line << " index: " << index;
            while(cur != end)
            {
                cur = docText.indexOf(text,cur+1);
                ui->textEdit->SendScintilla(QsciScintillaBase::SCI_INDICATORFILLRANGE,cur,
                    text.length());
            }
        }
    } // END text.isEmpty(), END mark ALL
}
