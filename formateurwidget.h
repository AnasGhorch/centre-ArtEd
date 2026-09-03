#ifndef FORMATEURWIDGET_H
#define FORMATEURWIDGET_H

#include "formateurdao.h"

#include <QWidget>

class QTableWidget;

// Onglet "Formateurs" : tableau + boutons CRUD
class FormateurWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FormateurWidget(QWidget *parent = nullptr);

    void rafraichir();

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();

private:
    void remplirTableau(const QList<Formateur> &liste);
    Formateur formateurSelectionne() const;

    FormateurDAO m_dao;
    QTableWidget *m_table = nullptr;
};

#endif // FORMATEURWIDGET_H
