#ifndef FORMATEURDAO_H
#define FORMATEURDAO_H

#include "formateur.h"

#include <QList>
#include <QString>

// Toutes les requêtes SQL sur la table FORMATEUR
class FormateurDAO
{
public:
    QList<Formateur> listerTous();
    bool ajouter(const Formateur &f, QString *messageErreur = nullptr);
    bool modifier(const Formateur &f, QString *messageErreur = nullptr);
    bool supprimer(int idFormateur, QString *messageErreur = nullptr);

    QString derniereErreur() const { return m_derniereErreur; }

private:
    Formateur lireLigne(class QSqlQuery &query) const;
    void setErreur(const QString &msg);

    QString m_derniereErreur;
};

#endif // FORMATEURDAO_H
