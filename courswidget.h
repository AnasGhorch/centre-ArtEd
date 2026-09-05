#ifndef COURSWIDGET_H
#define COURSWIDGET_H

#include "coursdao.h"

#include <QWidget>

class QTableWidget;
class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QFrame;

// Onglet "Cours" : filtres + tableau (tri) + CRUD + PDF
class CoursWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CoursWidget(QWidget *parent = nullptr);

    void rafraichir();

signals:
    void donneesModifiees();

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();
    void onRechercher();
    void onResetFiltres();
    void onGenererPdf();
    void basculerPanneauFiltres();

private:
    void construireFiltres();
    void remplirTableau(const QList<Cours> &liste);
    Cours coursSelectionne() const;

    CoursDAO m_dao;
    QTableWidget *m_table = nullptr;

    QFrame *m_panneauFiltres = nullptr;
    QComboBox *m_filtreDiscipline = nullptr;
    QComboBox *m_filtreNiveau = nullptr;
    QComboBox *m_filtreStatut = nullptr;
    QDateEdit *m_filtreDateMin = nullptr;
    QDateEdit *m_filtreDateMax = nullptr;
    QDoubleSpinBox *m_filtreTarifMin = nullptr;
    QDoubleSpinBox *m_filtreTarifMax = nullptr;
    class QCheckBox *m_cbDates = nullptr;
};

#endif // COURSWIDGET_H
