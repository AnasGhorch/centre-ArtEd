#ifndef STATISTIQUESWIDGET_H
#define STATISTIQUESWIDGET_H

#include <QWidget>

class PieChartWidget;
class BarChartWidget;

// Onglet Statistiques : 3 graphiques (rafraîchis depuis MainWindow)
class StatistiquesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatistiquesWidget(QWidget *parent = nullptr);

public slots:
    void rafraichir();

private:
    PieChartWidget *m_pieFormateurs = nullptr;
    BarChartWidget *m_barCours = nullptr;
    BarChartWidget *m_barRevenu = nullptr;
};

#endif // STATISTIQUESWIDGET_H
