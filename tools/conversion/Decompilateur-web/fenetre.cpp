#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenetre)
{
    ui->setupUi(this);
    connect(ui->bImport, &QPushButton::clicked, this, &fenetre::onImportClicked);
    connect(ui->bCompiler, &QPushButton::clicked, this, &fenetre::onCompileClicked);
    connect(ui->bDecompiler, &QPushButton::clicked, this, &fenetre::onDecompileClicked);
    connect(this, &fenetre::compile, &comp, &compilateur::compiler);
    connect(this, &fenetre::decompile, &comp, &compilateur::decompiler);
    connect(&comp, &compilateur::compilationOver, this, &fenetre::onCompilationOver);
    connect(&comp, &compilateur::decompilationOver, this, &fenetre::onCompilationOver);
    connect(&comp, &compilateur::progress, this, &fenetre::onProgressChanged);
}

fenetre::~fenetre()
{
    comp.exit();
    delete ui;
}

void fenetre::onImportClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Ouvrir un fichier"),
                QDir::homePath());
    if(fileName == "")
        return;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) {
        QString raison = "";
        switch (file.error()) {
        case QFile::PermissionsError:
            raison = tr("Vous n'avez pas les permissions d'ouvrir ce fichier");
            break;
        case QFile::ReadError:
            raison = tr("Erreur dans la lecture du fichier");
            break;
        default:
            raison = tr("Le fichier n'a pas pu être chargé") + " :\n"
                    + file.errorString();
        }
        QMessageBox::critical(this, tr("Importer un fichier"), raison);
        return;
    }
    lastFileName = fileName;
    ui->ptCode->setPlainText(file.readAll());
}

void fenetre::onCompileClicked()
{
    if(!comp.isAvailable()) {
        emit compile(ui->ptCode->toPlainText(),
                     getFormat());
    }
    ui->bCompiler->setDisabled(true);
    ui->bDecompiler->setDisabled(true);
}

void fenetre::onDecompileClicked()
{
    if(!comp.isAvailable()) {
        emit decompile(ui->ptCode->toPlainText(),
                       getFormat(),
                       getIndentationStr());
    }
    ui->bCompiler->setDisabled(true);
    ui->bDecompiler->setDisabled(true);
}

void fenetre::onCompilationOver(QString code)
{
    QMessageBox::information(this, tr("Compilation terminée"), code);
    if(ui->combotOutput->currentText() == "Auto") {

    }
    ui->bCompiler->setEnabled(true);
    ui->bDecompiler->setEnabled(true);
}

void fenetre::onError(QString error)
{
    QMessageBox::critical(this, tr("Erreur de Compilation"),
                          error, QMessageBox::Ok);
}

void fenetre::onProgressChanged(double progress)
{ ui->pbConversion->setValue(progress * 1000); }

QString fenetre::getIndentationStr() const
{
    QString indentationChar = ui->indentationType->currentText();
    if(indentationChar == "Tab")
        indentationChar = " ";
    else if(indentationChar == "Espace")
        indentationChar = " ";
    QString indentation = "";
    for(int i=0; i<ui->indentationNb->value(); i++)
        indentation += indentationChar;
    return indentation;
}

compilateur::Format fenetre::getFormat() const
{
    QString selected = ui->comboFormat->currentText();
    if(selected == "Auto")
        return compilateur::Format::Auto;
    if(selected == "JavaScript (JS)")
        return compilateur::Format::JS;
    if(selected == "HTML")
        return compilateur::Format::HTML;
    if(selected == "CSS")
        return compilateur::Format::CSS;
    if(selected == "JSON")
        return compilateur::Format::JSON;
    return compilateur::Format::Auto;
}
