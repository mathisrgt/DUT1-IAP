#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// Messages emis par les instructions -----------------------------------------
#define MSG_DEVELOPPE "## nouvelle specialite \"%s \" ; cout horaire \"%d\"\n" 
#define MSG_INTERRUPTION "## fin de programme\n" 
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n"
#define MSG_DEMARCHE "## nouveau client \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d'heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_SPECIALITES "## consultation des specialites\n"
#define MSG_TRAVAILLEURS "## consultation des travailleurs competents pour la specialite \"%s\"\n"
#define MSG_TRAVAILLEURS_TOUS "## consultation des travailleurs competents pour chaque specialite\n"
#define MSG_CLIENTS "## consultation des commandes effectuees par \"%s\"\n"
#define MSG_SUPERVISION "## consultation de l�avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"

// Lexemes -------------------------------------------------------------------- 
#define LGMOT 35
#define NBCHIFFREMAX 5 
typedef char Mot[LGMOT + 1];
void get_id(Mot id) {
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id);
}
int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer);
	return atoi(buffer);
}

// Instructions --------------------------------------------------------------- 
// developpe ---------------------------
void traite_developpe() {
	Mot nom_specialite;
	get_id(nom_specialite);
	int cout_horaire = get_int();
	printf(MSG_DEVELOPPE, nom_specialite, cout_horaire);
}

// embauche ----------------------------
/* La fonction traite_embauche() affiche la d�claration d'un nouvel employ�. 
N�cessite l'entr�e par l'utilisateur d'un nom et de la sp�cialit� associ�s au nouvel employ�. */

void traite_embauche() {
	Mot nom_employe, nom_specialite;
	get_id(nom_employe);
	get_id(nom_specialite);
	printf(MSG_EMBAUCHE, nom_employe, nom_specialite);
}

// demarche ----------------------------
/* La fonction traite_demarche() affiche la d�claration d'un nouveau client.
N�cessite l'entr�e par l'utilisateur d'un nom pour le client. */

void traite_demarche() {
	Mot nom_client;
	get_id(nom_client);
	printf(MSG_DEMARCHE, nom_client);
}

// commande ----------------------------
/* La fonction traite_commande() affiche la d�claration d'une nouvelle commande.
N�cessite l'entr�e par l'utilisateur d'un nom pour la commande et celui du client. */

void traite_commande() {
	Mot nom_commande, nom_client;
	get_id(nom_commande);
	get_id(nom_client);
	printf(MSG_COMMANDE, nom_commande, nom_client);
}

// tache -------------------------------
/* La fonction traite_tache() affiche la d�claration d'un nouvel t�che.
N�cessite l'entr�e par l'utilisateur du nom de la commande, la sp�cialit� concern�e 
et le nombre d'heures n�cessaires pour r�aliser la t�che. */

void traite_tache() {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nb_heures = get_int();
	printf(MSG_TACHE, nom_commande, nom_specialite, nb_heures);
}

// progression -------------------------
/* La fonction traite_progression() affiche la d�claration de l'avancement d'une t�che.
N�cessite l'entr�e par l'utilisateur du nom de la commande, de la sp�cialit� et le nombre d'heures � ajouter. */

void traite_progression() {

	Mot nom_commande, nom_specialite;

	get_id(nom_commande);
	get_id(nom_specialite);
	int nb_heures_sup = get_int();
	printf(MSG_PROGRESSION, nom_commande, nom_specialite, nb_heures_sup);
}

// specialites -------------------------
/* La fonction traite_specialites() affiche l'ent�te de consultation des sp�cialit�es cr�es.*/

void traite_specialites() {
	printf(MSG_SPECIALITES);
}

// travailleurs ------------------------
/* La fonction traite_travailleurs() affiche l'ent�te de consultation :
-> Si le mot "tous" est entr�, les travailleurs de chaque sp�cialit� vont �tre affich�s,
-> Sinon, les travailleurs pour une certaine sp�cialit�e.
N�cessite l'entr�e par l'utilisateur du mot-cl� "tous" ou du nom de la sp�cialit�. */

void traite_travailleurs() {
	Mot nom_specialite;
	get_id(nom_specialite);
	printf(MSG_TRAVAILLEURS, nom_specialite);
	if (strcmp(nom_specialite, "tous") == 0) {
		printf(MSG_TRAVAILLEURS_TOUS);
	}
	else {
		printf(MSG_TRAVAILLEURS, nom_specialite);
	}
}

// client ------------------------------
/* La fonction traite_clients() affiche l'ent�te de consultation des commandes effectu�es par un certain client.
N�cessite l'entr�e par l'utilisateur du nom du client. */

void traite_client() {
	Mot nom_client;
	get_id(nom_client);
	printf(MSG_CLIENTS, nom_client);
}

// supervision -------------------------
/* La fonction traite_supervision() affiche l'ent�te de consultation de l'avancement des commandes.*/
void traite_supervision() {
	printf(MSG_SUPERVISION);
}

// charge ------------------------------
/* La fonction traite_charge() affiche l'ent�te de consultation des charges de travail pour un certain travailleur.
N�cessite l'entr�e par l'utilisateur du nom du travailleur. */

void traite_charge() {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	printf(MSG_CHARGE, nom_travailleur);
}

// interruption ------------------------ 
/* La fonction traite_interruption() affiche le message de fin de programme.*/

void traite_interruption() {
	printf(MSG_INTERRUPTION);
}

// Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	while (VRAI) {
		get_id(buffer);
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe();
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche();
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche();
			continue;
		}
		if (strcmp(buffer, "commande") == 0) {
			traite_commande();
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			traite_tache();
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression();
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites();
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs();
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client();
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision();
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge();
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
