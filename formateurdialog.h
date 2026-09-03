#ifndef FORMATEURDIALOG_H
#define FORMATEURDIALOG_H

#include "formateur.h"

#include <QDialog>

class QLineEdit;
class QDateEdit;
class QSpinBox;
class QComboBox;

// Fenêtre pour ajouter ou modifier un formateur
class FormateurDialog : public QDialog
{
    Q_OBJECT
public:
    // idFormateur == 0 → ajout, sinon modification
    explicit FormateurDialog(const Formateur &f = Formateur(), QWidget *parent = nullptr);

    Formateur resultat() const;

    // Vérifie les champs avant d'accepter (appelée par le bouton OK)
    static bool validerEmail(const QString &email);

private slots:
    void valider();

private:
    QLineEdit *m_nom = nullptr;
    QLineEdit *m_prenom = nullptr;
    QLineEdit *m_email = nullptr;
    QLineEdit *m_telephone = nullptr;
    QLineEdit *m_discipline = nullptr;
    QDateEdit *m_dateEmbauche = nullptr;
    QComboBox *m_statut = nullptr;
    QSpinBox *m_annees = nullptr;
    int m_idFormateur = 0;
};

#endif // FORMATEURDIALOG_H
