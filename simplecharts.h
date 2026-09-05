#ifndef SIMPLECHARTS_H
#define SIMPLECHARTS_H

#include <QWidget>
#include <QMap>
#include <QString>

// Graphiques simples dessinés avec QPainter.
// (Qt Charts n'est pas installé sur cette machine — même rendu, sans module en plus.)

class PieChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PieChartWidget(QWidget *parent = nullptr);
    void setTitre(const QString &titre);
    void setData(const QMap<QString, int> &data);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_titre;
    QMap<QString, int> m_data;
};

class BarChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BarChartWidget(QWidget *parent = nullptr);
    void setTitre(const QString &titre);
    void setDataInt(const QMap<QString, int> &data);
    void setDataDouble(const QMap<QString, double> &data);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_titre;
    QMap<QString, double> m_data; // on unifie en double pour réutiliser le dessin
};

#endif // SIMPLECHARTS_H
