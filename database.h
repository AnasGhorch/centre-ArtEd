#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

// Connexion Oracle unique pour toute l'application (QODBC).
// Ouverte une fois au démarrage dans main.cpp.
namespace Database {

bool ouvrir(QString *messageErreur = nullptr);
void fermer();
bool estOuverte();

} // namespace Database

#endif // DATABASE_H
