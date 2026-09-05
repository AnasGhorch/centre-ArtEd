#include "simplecharts.h"

#include <QPainter>
#include <QPaintEvent>

static QColor couleurIndex(int i)
{
    static const QColor couleurs[] = {
        QColor(255, 106, 0),   // orange ArtEd
        QColor(43, 43, 43),
        QColor(25, 118, 210),
        QColor(46, 125, 50),
        QColor(123, 31, 162),
        QColor(0, 121, 107),
        QColor(198, 40, 40)
    };
    return couleurs[i % 7];
}

PieChartWidget::PieChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(240);
    setAutoFillBackground(true);
}

void PieChartWidget::setTitre(const QString &titre)
{
    m_titre = titre;
    update();
}

void PieChartWidget::setData(const QMap<QString, int> &data)
{
    m_data = data;
    update();
}

void PieChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(255, 255, 255));
    p.setPen(QColor(20, 20, 20));
    p.drawText(10, 20, m_titre.isEmpty() ? QStringLiteral("Camembert") : m_titre);

    int total = 0;
    int n = 0;
    for (int v : m_data) {
        total += v;
        if (v > 0)
            ++n;
    }
    if (total == 0) {
        p.drawText(rect(), Qt::AlignCenter, QStringLiteral("Aucune donnée"));
        return;
    }

    const QRect zone = rect().adjusted(8, 28, -8, -8);
    const int legendW = 140;
    int pieSize = qMin(zone.width() - legendW - 12, zone.height());
    pieSize = qMin(pieSize, 170);
    const int pieX = zone.left() + (zone.width() - legendW - 12 - pieSize) / 2;
    const int pieY = zone.top() + (zone.height() - pieSize) / 2;
    QRectF pieRect(pieX, pieY, pieSize, pieSize);

    double angle = 0;
    int i = 0;
    int legendY = zone.top() + (zone.height() - n * 24) / 2;
    const int legendX = zone.right() - legendW;

    for (auto it = m_data.begin(); it != m_data.end(); ++it, ++i) {
        if (it.value() <= 0)
            continue;
        double span = 360.0 * it.value() / total;
        p.setBrush(couleurIndex(i));
        p.setPen(QPen(Qt::white, 1));
        p.drawPie(pieRect, int(angle * 16), int(span * 16));
        angle += span;

        p.fillRect(legendX, legendY, 14, 14, couleurIndex(i));
        p.setPen(QColor(20, 20, 20));
        QString label = it.key();
        if (label.size() > 14)
            label = label.left(13) + QStringLiteral("…");
        p.drawText(legendX + 20, legendY + 12,
                   QStringLiteral("%1 : %2").arg(label).arg(it.value()));
        legendY += 24;
    }
}

BarChartWidget::BarChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(240);
    setAutoFillBackground(true);
}

void BarChartWidget::setTitre(const QString &titre)
{
    m_titre = titre;
    update();
}

void BarChartWidget::setDataInt(const QMap<QString, int> &data)
{
    m_data.clear();
    for (auto it = data.begin(); it != data.end(); ++it)
        m_data[it.key()] = it.value();
    update();
}

void BarChartWidget::setDataDouble(const QMap<QString, double> &data)
{
    m_data = data;
    update();
}

void BarChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(255, 255, 255));
    p.setPen(QColor(20, 20, 20));
    p.drawText(10, 20, m_titre.isEmpty() ? QStringLiteral("Barres") : m_titre);

    if (m_data.isEmpty()) {
        p.drawText(rect(), Qt::AlignCenter, QStringLiteral("Aucune donnée"));
        return;
    }

    double maxVal = 1.0;
    int maxLabelW = 80;
    for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        maxVal = qMax(maxVal, it.value());
        maxLabelW = qMax(maxLabelW, p.fontMetrics().horizontalAdvance(it.key()) + 8);
    }
    maxLabelW = qMin(maxLabelW, width() / 2);

    const int top = 32;
    const int leftLabel = 8;
    const int leftBar = leftLabel + maxLabelW + 6;
    const int right = width() - 50;
    const int barAreaW = qMax(40, right - leftBar);
    const int rowH = qMax(16, (height() - top - 10) / qMax(1, m_data.size()));
    const int barH = qMax(10, rowH - 6);

    int i = 0;
    int y = top;
    for (auto it = m_data.begin(); it != m_data.end(); ++it, ++i) {
        QString shown = p.fontMetrics().elidedText(it.key(), Qt::ElideRight, maxLabelW);
        p.setPen(QColor(20, 20, 20));
        p.drawText(QRect(leftLabel, y, maxLabelW, barH),
                   Qt::AlignVCenter | Qt::AlignLeft, shown);

        const int w = int(barAreaW * (it.value() / maxVal));
        p.fillRect(QRect(leftBar, y, qMax(4, w), barH), couleurIndex(i));
        p.setPen(QColor(20, 20, 20));
        // Affiche entier si pas de décimale utile
        QString valText = (qAbs(it.value() - int(it.value())) < 0.01)
                              ? QString::number(int(it.value()))
                              : QString::number(it.value(), 'f', 2);
        p.drawText(leftBar + qMax(4, w) + 6, y + barH - 3, valText);
        y += rowH;
    }
}
