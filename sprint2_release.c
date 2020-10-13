/* sprint2r.c
* Auteurs : Gregory TACQUET, Mathis SERGENT
* Date de création : 26/09/2020
*/


#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Messages emis par les instructions -----------------------------------------
#define MSG_DEVELOPPE "## nouvelle specialite \"%s \" ; cout horaire \"%d\"\n" 
#define MSG_INTERRUPTION "## fin de programme\n" 
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n"
#define MSG_DEMARCHE "## nouveau client \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d'heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PASSE "## une reallocation est requise\n"
#define MSG_SPECIALITES "specialites traitees : "
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CLIENTS "\nle client %s a commande : "
#define MSG_SUPERVISION "## consultation de l'avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"

// Lexemes -------------------------------------------------------------------- 
#define LGMOT 35
#define NBCHIFFREMAX 5 
typedef char Mot[LGMOT + 1];

// gestion programme -----------------------
typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// specialites ----------------------------
#define MAX_SPECIALITES 10

typedef struct {
	Mot nom;
	int cout_horaire;
} Specialite;

typedef struct {
	Specialite tab_specialites[MAX_SPECIALITES];
	unsigned int nb_specialites;
} Specialites;

// travailleurs --------------------------
#define MAX_TRAVAILLEURS 50

typedef struct {
	Mot nom;
	Booleen tags_competences[MAX_SPECIALITES];
} Travailleur;

typedef struct {
	Travailleur tab_travailleurs[MAX_TRAVAILLEURS];
	unsigned int nb_travailleurs;
} Travailleurs;

// client ---------------------------------
#define MAX_CLIENTS 10

typedef struct {
	Mot tab_clients[MAX_CLIENTS];
	unsigned int nb_clients;
} Clients;

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
/* 
* Créer une nouvelle spécialité
* ptr_specialites [out] le pointeur du tableau des spécialités créées
*/

void traite_developpe(Specialites* ptr_specialites) {
	Mot nom_specialite;
	Specialite specialite;
	get_id(nom_specialite);
	for (int i = 0; i < LGMOT; ++i) {
		specialite.nom[i] = nom_specialite[i];
	}
	specialite.cout_horaire = get_int();
	ptr_specialites->tab_specialites[ptr_specialites->nb_specialites] = specialite;
	++(ptr_specialites->nb_specialites);
}

// embauche ----------------------------
/* 
* Créer un nouveau travailleur 
* ptr_specilites [in] le pointeur du tableau des spécialités créées
* ptr_travailleurs [out] 
*/

void traite_embauche(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs) {
	Mot nom_employe, nom_specialite;
	Travailleur travailleur;
	get_id(nom_employe);
	get_id(nom_specialite);

	strcpy(travailleur.nom, nom_employe);

	for (int i = 0; i < (ptr_specialites->nb_specialites); ++i) {
		if (strcmp((ptr_specialites->tab_specialites[i]).nom, nom_specialite) == 0) {
			travailleur.tags_competences[i] = VRAI;
		}
		else {
			travailleur.tags_competences[i] = FAUX;
		}
	}

	ptr_travailleurs->tab_travailleurs[ptr_travailleurs->nb_travailleurs] = travailleur;
	++(ptr_travailleurs->nb_travailleurs);
}

// demarche ----------------------------
/* 
* Créer un nouveau client.
* ptr_client [out] le pointeur du tableau des clients créés
*/

void traite_demarche(Clients* ptr_clients) {
	Mot nom_client;
	get_id(nom_client);

	strcpy(ptr_clients->tab_clients[ptr_clients->nb_clients], nom_client);
	++(ptr_clients->nb_clients);
}

// commande ----------------------------
/* 
* Créer une nouvelle commande.
*/

void traite_commande() {
	Mot nom_commande, nom_client;
	get_id(nom_commande);
	get_id(nom_client);
	printf(MSG_COMMANDE, nom_commande, nom_client);
}

// tache -------------------------------
/* Créer une nouvelle tâche.
* 
*/

void traite_tache() {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nb_heures = get_int();
	printf(MSG_TACHE, nom_commande, nom_specialite, nb_heures);
}

// progression -------------------------
/* Déclare l'avancement d'une tâche.
* 
*/

void traite_progression() {

	Mot nom_commande, nom_specialite, passe_fonction;

	get_id(nom_commande);
	get_id(nom_specialite);
	int nb_heures_sup = get_int();

	printf(MSG_PROGRESSION, nom_commande, nom_specialite, nb_heures_sup);
}

// passe -------------------------------
/*
* Affiche un message de réaffectation.
*/

void traite_passe() {
	printf(MSG_PASSE);
}

// specialites -------------------------
/* La fonction traite_specialites() affiche l'entête de consultation des spécialitées crées.*/

void traite_specialites(Specialites* ptr_specialites) {
	printf(MSG_SPECIALITES);
	for (int i = 0; i < ptr_specialites->nb_specialites; ++i) {
		printf("%s/%d", (ptr_specialites->tab_specialites[i]).nom, (ptr_specialites->tab_specialites[i]).cout_horaire);
		if ((i + 1) < ptr_specialites->nb_specialites) {
			printf(", ");
		}
	}
	printf("\n");
}

// travailleurs ------------------------

void affiche_travailleurs(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Mot nom_specialite, int i) {
	int j = 0;
	int before = FAUX;

	printf(MSG_TRAVAILLEURS, nom_specialite);
	for (i = 0; i < ptr_specialites->nb_specialites; ++i) {
		if (strcmp((ptr_specialites->tab_specialites[i]).nom, nom_specialite) == 0) {
			break;
		}
	}

	for (j = (ptr_travailleurs->nb_travailleurs) - 1; j >= 0; --j) {
		if ((ptr_travailleurs->tab_travailleurs[j]).tags_competences[i] == VRAI) {
			if (before == VRAI) {
				printf(", ");
			}
			printf("%s", (ptr_travailleurs->tab_travailleurs[j]).nom);
			before = VRAI;
		}
	}

	printf("\n");
}

/* La fonction traite_travailleurs() affiche l'entête de consultation :
-> Si le mot "tous" est entré, les travailleurs de chaque spécialité vont être affichés,
-> Sinon, les travailleurs pour une certaine spécialitée.
Nécessite l'entrée par l'utilisateur du mot-clé "tous" ou du nom de la spécialité. */

void traite_travailleurs(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs) {
	Mot nom_specialite;
	get_id(nom_specialite);
	int i = 0;
	int j = 0;
	int before;

	if (strcmp(nom_specialite, "tous") == 0) {
		
		for (i = 0; i < ptr_specialites->nb_specialites; ++i) {

			strcpy(nom_specialite, ptr_specialites->tab_specialites[i].nom);

			affiche_travailleurs(ptr_specialites, ptr_travailleurs, nom_specialite, i);

		}

	}
	else {

		affiche_travailleurs(ptr_specialites, ptr_travailleurs, nom_specialite, i);

	}
}

// client ------------------------------
/* La fonction traite_clients() affiche l'entête de consultation des commandes effectuées par un certain client.
Nécessite l'entrée par l'utilisateur du nom du client. */

void traite_client(Clients* ptr_clients) {
	Mot nom_client;
	get_id(nom_client);
	int i = 0;
	
	if (strcmp(nom_client, "tous") == 0) {
		for (i = 0; i < ptr_clients->nb_clients; ++i) {
			printf(MSG_CLIENTS, (ptr_clients->tab_clients[i]));
		}
	}
	else {
		for (i = 0; i < ptr_clients->nb_clients; ++i) {
			if (strcmp((ptr_clients->tab_clients[i]), nom_client) == 0) {
				break;
			}
		}

		printf(MSG_CLIENTS, (ptr_clients->tab_clients[i]));
	}
	printf("\n");
}

// supervision -------------------------
/* La fonction traite_supervision() affiche l'entête de consultation de l'avancement des commandes.*/
void traite_supervision() {
	printf(MSG_SUPERVISION);
}

// charge ------------------------------
/* La fonction traite_charge() affiche l'entête de consultation des charges de travail pour un certain travailleur.
Nécessite l'entrée par l'utilisateur du nom du travailleur. */

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
	Specialites specialites;
	specialites.nb_specialites = 0;
	Travailleurs travailleurs;
	travailleurs.nb_travailleurs = 0;
	Clients clients;
	clients.nb_clients = 0;
	
	while (VRAI) {
		get_id(buffer);
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe(&specialites);
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche(&specialites, &travailleurs);
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche(&clients);
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
		if (strcmp(buffer, "passe") == 0) {
			traite_passe();
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites(&specialites);
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs(&specialites, &travailleurs);
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client(&clients);
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
