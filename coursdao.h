#ifndef COURSDAO_H
#define COURSDAO_H

#include "cours.h"

#include <QList>
#include <QString>

// Toutes les requêtes SQL sur la table COURS
class CoursDAO
{
public:
    QList<Cours> listerTous();
    bool ajouter(const Cours &c, QString *messageErreur = nullptr);
    bool modifier(const Cours &c, QString *messageErreur = nullptr);
    bool supprimer(int idCours, QString *messageErreur = nullptr);

    QString derniereErreur() const { return m_derniereErreur; }

private:
    Cours lireLigne(class QSqlQuery &query) const;
    void setErreur(const QString &msg);

    QString m_derniereErreur;
};

#endif // COURSDAO_H
