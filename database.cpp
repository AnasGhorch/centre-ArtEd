#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>

// Nom interne de la connexion Qt (ne pas confondre avec le DSN Windows)
static const char *NOM_CONNEXION = "formation_connexion";

// Paramètres ODBC — DSN XE_SYSTEM (Oracle XE, schéma SYSTEM)
// Modifier ici si tu changes de DSN ou d'utilisateur Oracle.
static const char *DSN      = "XE_SYSTEM";
static const char *USER     = "SYSTEM";
static const char *PASSWORD = "oracle";

bool Database::ouvrir(QString *messageErreur)
{
    if (QSqlDatabase::contains(NOM_CONNEXION)) {
        if (QSqlDatabase::database(NOM_CONNEXION).isOpen())
            return true;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QODBC"), NOM_CONNEXION);
    db.setDatabaseName(QString::fromLatin1(DSN));
    db.setUserName(QString::fromLatin1(USER));
    db.setPassword(QString::fromLatin1(PASSWORD));

    if (!db.open()) {
        if (messageErreur)
            *messageErreur = db.lastError().text();
        return false;
    }
    return true;
}

void Database::fermer()
{
    if (!QSqlDatabase::contains(NOM_CONNEXION))
        return;
    {
        QSqlDatabase db = QSqlDatabase::database(NOM_CONNEXION);
        if (db.isOpen())
            db.close();
    }
    QSqlDatabase::removeDatabase(NOM_CONNEXION);
}

bool Database::estOuverte()
{
    return QSqlDatabase::contains(NOM_CONNEXION)
           && QSqlDatabase::database(NOM_CONNEXION).isOpen();
}
