#include "fenetre.h"

fenetre::fenetre() : QWidget()
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QHBoxLayout *layoutLoad = new QHBoxLayout;
    layout->addLayout(layoutLoad);
    lLoad = new QLineEdit;
    bLoadOpen = new QPushButton("Parcourir");
    bLoad = new QPushButton("Charger");
    layoutLoad->addWidget(lLoad);
    layoutLoad->addWidget(bLoadOpen);
    layoutLoad->addWidget(bLoad);
    connect(lLoad, &QLineEdit::returnPressed, this, &fenetre::sLoad);
    connect(bLoadOpen, &QPushButton::clicked, this, &fenetre::sLoadOpen);
    connect(bLoad, &QPushButton::clicked, this, &fenetre::sLoad);

    QHBoxLayout *layoutUpload = new QHBoxLayout;
    layout->addLayout(layoutUpload);
    lUpload = new QLineEdit("./myJson.json");
    bUploadOpen = new QPushButton("Parcourir");
    bUpload = new QPushButton("Enregistrer");
    layoutUpload->addWidget(lUpload);
    layoutUpload->addWidget(bUploadOpen);
    layoutUpload->addWidget(bUpload);
    connect(lUpload, &QLineEdit::returnPressed, this, &fenetre::sUpload);
    connect(bUploadOpen, &QPushButton::clicked, this, &fenetre::sUploadOpen);
    connect(bUpload, &QPushButton::clicked, this, &fenetre::sUpload);

    pteObject = new QTextEdit("<p><font color=#D0D0D0>Aucun Object Chargé</font><br /></p>");
    pteObject->setReadOnly(true);
    pteObject->setTextInteractionFlags(pteObject->textInteractionFlags() | Qt::TextSelectableByKeyboard);
    //https://doc.qt.io/qt-5/qt.html#TextInteractionFlag-enum
    layout->addWidget(pteObject);
    pteLog = new QTextEdit("<html></html>");
    pteLog->setReadOnly(true);
    pteLog->setTextInteractionFlags(pteLog->textInteractionFlags() | Qt::TextSelectableByKeyboard);
    pteLog->setMaximumHeight(192);
    layout->addWidget(pteLog);

    QHBoxLayout *layoutCmd = new QHBoxLayout;
    layout->addLayout(layoutCmd);
    lCmd = new QLineEdit;
    bCmd = new QPushButton("Envoyer");
    layoutCmd->addWidget(lCmd);
    layoutCmd->addWidget(bCmd);
    connect(lCmd, &QLineEdit::returnPressed, this, &fenetre::sCmd);
    connect(bCmd, &QPushButton::clicked, this, &fenetre::sCmd);

    pdeb = new debug(pteLog);
    json = new JSON(pdeb);

    show();
    lLoad->setText("C:/Qt/MesProg/outils/JSON/BUILD/build-Desktop_Qt_5_13_0_MinGW_32_bit-Debug/block (ressourcepack 3D)/xray.json");

    /*pdeb->addLog("fenetre", "crit", 0);
    pdeb->addLog("fenetre", "error", 1);
    pdeb->addLog("fenetre", "warn", 2);
    pdeb->addLog("fenetre", "info", 3);
    pdeb->addLog("fenetre", "debug", 4);
    pdeb->addLog("fenetre", "autre", 5);*/
}

fenetre::~fenetre()
{
    delete lLoad; delete bLoadOpen; delete bLoad;
    delete lUpload; delete bUploadOpen; delete bUpload;
    delete pteObject; delete pteLog;
    delete lCmd; delete bCmd;
}

void fenetre::sLoadOpen()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier [JSON]", QString(), "Fichier JSON ("+json->fileFormat+")");
    if(!fichier.isEmpty())
        lLoad->setText(fichier);
}

void fenetre::sLoad()
{
    json->loadFile(lLoad->text());
}

void fenetre::sUploadOpen()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Enregistrer un fichier [JSON]", QString(), "Fichier JSON ("+json->fileFormat+")");
    if(!fichier.isEmpty())
        lUpload->setText(fichier);
}

void fenetre::sUpload()
{
    json->saveInFile(lUpload->text());
}

void fenetre::sCmd()
{
    json->execCmd(lCmd->text());
    lCmd->clear();
}
