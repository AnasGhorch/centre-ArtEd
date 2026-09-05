#ifndef FORMATEURWIDGET_H
#define FORMATEURWIDGET_H

#include "formateurdao.h"

#include <QWidget>

class QTableWidget;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QFrame;

// Onglet "Formateurs" : filtres + tableau (tri) + CRUD
class FormateurWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FormateurWidget(QWidget *parent = nullptr);

    void rafraichir();

signals:
    // Émis après ajout / modif / suppression → MainWindow rafraîchit les stats
    void donneesModifiees();

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();
    void onRechercher();
    void onResetFiltres();
    void basculerPanneauFiltres();

private:
    void construireFiltres();
    void remplirTableau(const QList<Formateur> &liste);
    Formateur formateurSelectionne() const;

    FormateurDAO m_dao;
    QTableWidget *m_table = nullptr;

    QFrame *m_panneauFiltres = nullptr;
    QLineEdit *m_filtreNom = nullptr;
    QComboBox *m_filtreDiscipline = nullptr;
    QComboBox *m_filtreStatut = nullptr;
    QSpinBox *m_filtreExpMin = nullptr;
    QSpinBox *m_filtreExpMax = nullptr;
};

#endif // FORMATEURWIDGET_H
