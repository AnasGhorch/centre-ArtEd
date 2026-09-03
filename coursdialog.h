#ifndef COURSDIALOG_H
#define COURSDIALOG_H

#include "cours.h"

#include <QDialog>

class QLineEdit;
class QTextEdit;
class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QSpinBox;

// Fenêtre pour ajouter ou modifier un cours
class CoursDialog : public QDialog
{
    Q_OBJECT
public:
    // idCours == 0 → ajout, sinon modification
    explicit CoursDialog(const Cours &c = Cours(), QWidget *parent = nullptr);

    Cours resultat() const;

private slots:
    void valider();

private:
    void chargerFormateurs(); // remplit la combobox depuis FormateurDAO

    QLineEdit *m_titre = nullptr;
    QTextEdit *m_description = nullptr;
    QLineEdit *m_discipline = nullptr;
    QComboBox *m_niveau = nullptr;
    QDoubleSpinBox *m_duree = nullptr;
    QDateEdit *m_dateDebut = nullptr;
    QDateEdit *m_dateFin = nullptr;
    QDoubleSpinBox *m_tarif = nullptr;
    QSpinBox *m_capacite = nullptr;
    QLineEdit *m_salle = nullptr;
    QComboBox *m_statut = nullptr;
    QComboBox *m_formateur = nullptr; // liste déroulante des formateurs
    int m_idCours = 0;
};

#endif // COURSDIALOG_H
