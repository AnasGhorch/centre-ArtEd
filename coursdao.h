#ifndef COURSDAO_H
#define COURSDAO_H

#include "cours.h"

#include <QDate>
#include <QList>
#include <QMap>
#include <QString>

// Toutes les requêtes SQL sur la table COURS
class CoursDAO
{
public:
    QList<Cours> listerTous();
    bool ajouter(const Cours &c, QString *messageErreur = nullptr);
    bool modifier(const Cours &c, QString *messageErreur = nullptr);
    bool supprimer(int idCours, QString *messageErreur = nullptr);

    // Filtre combiné : valeurs vides / dates invalides / tarif < 0 = critère ignoré
    QList<Cours> filtrer(const QString &discipline,
                         const QString &niveau,
                         const QString &statut,
                         const QDate &dateMin,
                         const QDate &dateMax,
                         double tarifMin,
                         double tarifMax);

    // Stats
    QMap<QString, int> compterParDiscipline();
    QMap<QString, double> revenuParDiscipline(); // SUM(tarif) par discipline

    QString derniereErreur() const { return m_derniereErreur; }

private:
    Cours lireLigne(class QSqlQuery &query) const;
    void setErreur(const QString &msg);

    QString m_derniereErreur;
};

#endif // COURSDAO_H
