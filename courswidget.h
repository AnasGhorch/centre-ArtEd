#ifndef COURSWIDGET_H
#define COURSWIDGET_H

#include "coursdao.h"

#include <QWidget>

class QTableWidget;

// Onglet "Cours" : tableau + boutons CRUD
class CoursWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CoursWidget(QWidget *parent = nullptr);

    void rafraichir();

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();

private:
    void remplirTableau(const QList<Cours> &liste);
    Cours coursSelectionne() const;

    CoursDAO m_dao;
    QTableWidget *m_table = nullptr;
};

#endif // COURSWIDGET_H
