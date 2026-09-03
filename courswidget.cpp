#include "courswidget.h"

#include <QLabel>
#include <QVBoxLayout>

CoursWidget::CoursWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *label = new QLabel(
        QStringLiteral("Module Cours — CRUD à venir.\n"
                       "La classe CoursDAO est prête à recevoir les requêtes SQL."));
    label->setAlignment(Qt::AlignCenter);
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
