#ifndef FEN_H
#define FEN_H

#include <QtWidgets>
#include <windows.h>
#include <QTimer>


class fen : public QWidget
{
    Q_OBJECT
    public:
        fen();


    private slots:
        void lancer();
        void actuBoucle();
        void ActuLum();
        void typeChange();


    private:
        QDoubleSpinBox *Vitesse;
        QSpinBox *boucles;
        QComboBox *type;
        QTextEdit *perso;
        QPushButton *bLancer;

        bool lumActive[3];





};

#endif // FEN_H
