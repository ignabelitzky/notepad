#include "notepad.h"
#include "./ui_notepad.h"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose); // Qt takes care of deleting the window from memory
    setWindowTitle(QString("%1[*]").arg("Unnamed File"));
    currentFile.clear();
    editor = Notepad::findChild<QTextEdit *>(QStringLiteral("textEdit"));
    setCentralWidget(editor);

    connect(ui->actionNew, &QAction::triggered, this, &Notepad::new_document);
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionSaveAs, &QAction::triggered, this, &Notepad::save_as);
    connect(ui->actionExit, &QAction::triggered, this, &Notepad::exit);

    statusBar()->showMessage("Done");
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::new_document()
{
    currentFile.clear();
    editor->setText(QString(""));
}

void Notepad::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    } else {
        setWindowTitle(fileName);
        QTextStream in(&file);
        QString text = in.readAll();
        editor->clear();
        editor->append(text);
        file.close();
    }
}

void Notepad::save()
{
    QString fileName;
    if(currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
    }
    currentFile = fileName;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
    } else {
        setWindowTitle(fileName);
        QTextStream out(&file);
        QString text = editor->toPlainText();
        out << text;
        file.close();
    }
}

void Notepad::save_as()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
    } else {
        currentFile = fileName;
        setWindowTitle(fileName);
        QTextStream out(&file);
        QString text = editor->toPlainText();
        out << text;
        file.close();
    }
}

void Notepad::exit()
{
    QCoreApplication::quit();
}

