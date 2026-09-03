#include "coursdao.h"
#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

static const char *CONN_COURS = "formation_connexion";

Cours CoursDAO::lireLigne(QSqlQuery &query) const
{
    Cours c;
    c.idCours = query.value(0).toInt();
    c.titre = query.value(1).toString();
    c.description = query.value(2).toString();
    c.disciplineArtistique = query.value(3).toString();
    c.niveau = query.value(4).toString();
    c.dureeHeures = query.value(5).toDouble();
    c.dateDebut = query.value(6).toDate();
    c.dateFin = query.value(7).toDate();
    c.tarif = query.value(8).toDouble();
    c.capaciteMax = query.value(9).toInt();
    c.salle = query.value(10).toString();
    c.statut = query.value(11).toString();
    c.idFormateur = query.value(12).toInt();
    // prenom + nom du formateur (colonnes 13 et 14 du JOIN)
    c.nomFormateur = query.value(13).toString() + QStringLiteral(" ") + query.value(14).toString();
    return c;
}

void CoursDAO::setErreur(const QString &msg)
{
    m_derniereErreur = msg;
}

QList<Cours> CoursDAO::listerTous()
{
    QList<Cours> liste;
    if (!Database::estOuverte())
        return liste;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    // JOIN pour afficher le nom du formateur, pas seulement l'id
    query.prepare(
        "SELECT c.id_cours, c.titre, c.description, c.discipline_artistique, c.niveau, "
        "c.duree_heures, c.date_debut, c.date_fin, c.tarif, c.capacite_max, c.salle, c.statut, "
        "c.id_formateur, f.prenom, f.nom "
        "FROM COURS c "
        "JOIN FORMATEUR f ON c.id_formateur = f.id_formateur "
        "ORDER BY c.id_cours");

    if (!query.exec()) {
        setErreur(query.lastError().text());
        return liste;
    }
    while (query.next())
        liste.append(lireLigne(query));
    return liste;
}

bool CoursDAO::ajouter(const Cours &c, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare(
        "INSERT INTO COURS (id_cours, titre, description, discipline_artistique, niveau, "
        "duree_heures, date_debut, date_fin, tarif, capacite_max, salle, statut, id_formateur) "
        "VALUES (seq_cours.NEXTVAL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(c.titre);
    query.addBindValue(c.description);
    query.addBindValue(c.disciplineArtistique);
    query.addBindValue(c.niveau);
    query.addBindValue(c.dureeHeures);
    query.addBindValue(c.dateDebut);
    query.addBindValue(c.dateFin);
    query.addBindValue(c.tarif);
    query.addBindValue(c.capaciteMax);
    query.addBindValue(c.salle);
    query.addBindValue(c.statut);
    query.addBindValue(c.idFormateur);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}

bool CoursDAO::modifier(const Cours &c, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare(
        "UPDATE COURS SET titre=?, description=?, discipline_artistique=?, niveau=?, "
        "duree_heures=?, date_debut=?, date_fin=?, tarif=?, capacite_max=?, salle=?, "
        "statut=?, id_formateur=? WHERE id_cours=?");
    query.addBindValue(c.titre);
    query.addBindValue(c.description);
    query.addBindValue(c.disciplineArtistique);
    query.addBindValue(c.niveau);
    query.addBindValue(c.dureeHeures);
    query.addBindValue(c.dateDebut);
    query.addBindValue(c.dateFin);
    query.addBindValue(c.tarif);
    query.addBindValue(c.capaciteMax);
    query.addBindValue(c.salle);
    query.addBindValue(c.statut);
    query.addBindValue(c.idFormateur);
    query.addBindValue(c.idCours);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}

bool CoursDAO::supprimer(int idCours, QString *messageErreur)
{
    if (!Database::estOuverte())
        return false;

    QSqlQuery query(QSqlDatabase::database(CONN_COURS));
    query.prepare("DELETE FROM COURS WHERE id_cours=?");
    query.addBindValue(idCours);

    if (!query.exec()) {
        setErreur(query.lastError().text());
        if (messageErreur)
            *messageErreur = m_derniereErreur;
        return false;
    }
    return true;
}
