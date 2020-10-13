/* sprint3r.c
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
#define MSG_SPECIALITES "specialites traitees : "
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CLIENTS "\nle client %s a commande : "
#define MSG_SUPERVISION "\netat des taches pour %s : "
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

// commande --------------------------------
#define MAX_COMMANDES 500

typedef struct {
	unsigned int nb_heures_requises;
	unsigned int nb_heures_effectuees;
}Tache;

typedef struct {
	Mot nom;
	unsigned int id_client;
	Tache tache_par_specialite[MAX_SPECIALITES]; // n_heure_requise==0 <=> pas de tache pour cette specialite
}Commande;

typedef struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
}Commandes;

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

void traite_commande(Commandes* ptr_commandes, Clients* ptr_clients) {
	Mot nom_commande, nom_client;
	Commande commande;
	int i;
	get_id(nom_commande);
	get_id(nom_client);

	strcpy(commande.nom, nom_commande);

	for (i = 0; i < ptr_clients->nb_clients; ++i) {
		if (strcmp(ptr_clients->tab_clients[i], nom_client) == 0) {
			commande.id_client = i;
			break;
		}
	}

	for (i = 0; i < MAX_SPECIALITES; ++i) {
		commande.tache_par_specialite[i].nb_heures_requises = 0;
		commande.tache_par_specialite[i].nb_heures_effectuees = 0;
	}

	ptr_commandes->tab_commandes[ptr_commandes->nb_commandes] = commande;
	++(ptr_commandes->nb_commandes);
}

// tache -------------------------------
/* Créer une nouvelle tâche.
* 
*/

void traite_tache(Specialites* ptr_specialites, Commandes* ptr_commandes) {
	Mot nom_commande, nom_specialite;
	Tache tache;
	get_id(nom_commande);
	get_id(nom_specialite);
	tache.nb_heures_requises = get_int();
	tache.nb_heures_effectuees = 0;
	int indice_specialite, indice_commande;
	
	for (indice_specialite = 0; indice_specialite < ptr_specialites->nb_specialites; ++indice_specialite) {
		if (strcmp(ptr_specialites->tab_specialites[indice_specialite].nom, nom_specialite) == 0) {
			break;
		}
	}

	for (indice_commande = 0; indice_commande < ptr_commandes->nb_commandes; ++indice_commande) {
		if (strcmp(ptr_commandes->tab_commandes[indice_commande].nom, nom_commande) == 0) {
			break;
		}
	}

	ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite] = tache;
}

// progression -------------------------
/* Déclare l'avancement d'une tâche.
* 
*/

void traite_progression(Specialites* ptr_specialites, Commandes* ptr_commandes) {

	Mot nom_commande, nom_specialite, passe_fonction;

	get_id(nom_commande);
	get_id(nom_specialite);
	int indice_specialite, indice_commande;

	for (indice_specialite = 0; indice_specialite < ptr_specialites->nb_specialites; ++indice_specialite) {
		if (strcmp(ptr_specialites->tab_specialites[indice_specialite].nom, nom_specialite) == 0) {
			break;
		}
	}

	for (indice_commande = 0; indice_commande < ptr_commandes->nb_commandes; ++indice_commande) {
		if (strcmp(ptr_commandes->tab_commandes[indice_commande].nom, nom_commande) == 0) {
			break;
		}
	}

	ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_effectuees += get_int();
}

// passe -------------------------------
/*
* Affiche un message de réaffectation.
*/

void traite_passe() {
	//...
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
	Booleen virgule = FAUX;

	printf(MSG_TRAVAILLEURS, nom_specialite);
	for (i = 0; i < ptr_specialites->nb_specialites; ++i) {
		if (strcmp((ptr_specialites->tab_specialites[i]).nom, nom_specialite) == 0) {
			break;
		}
	}

	for (j = (ptr_travailleurs->nb_travailleurs) - 1; j >= 0; --j) {
		if ((ptr_travailleurs->tab_travailleurs[j]).tags_competences[i] == VRAI) {
			if (virgule == VRAI) {
				printf(", ");
			}
			printf("%s", (ptr_travailleurs->tab_travailleurs[j]).nom);
			virgule = VRAI;
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

	if (strcmp(nom_specialite, "tous") == 0) {
		for (int i = 0; i < ptr_specialites->nb_specialites; ++i) {
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

void afficher_commandes(Commandes* ptr_commandes, int indice_client) {
	Booleen virgule = FAUX;
	for (int i = 0; i < ptr_commandes->nb_commandes; ++i) {
		if (ptr_commandes->tab_commandes[i].id_client == indice_client) {
			if (virgule == VRAI) {
				printf(", ");
			}
			printf("%s", ptr_commandes->tab_commandes[i].nom);
			virgule = VRAI;
		}
	}
}

void traite_client(Clients* ptr_clients, Commandes* ptr_commandes) {
	Mot nom_client;
	get_id(nom_client);
	int indice_client = 0;
	
	if (strcmp(nom_client, "tous") == 0) {
		for (indice_client = 0; indice_client < ptr_clients->nb_clients; ++indice_client) {
			printf(MSG_CLIENTS, (ptr_clients->tab_clients[indice_client])); //seulement si on a au moins 1 commande faite par le client

			afficher_commandes(ptr_commandes, indice_client);
		}
	}
	else {
		for (indice_client = 0; indice_client < ptr_clients->nb_clients; ++indice_client) {
			if (strcmp((ptr_clients->tab_clients[indice_client]), nom_client) == 0) {
				break;
			}
		}

		printf(MSG_CLIENTS, (ptr_clients->tab_clients[indice_client]));
		
		afficher_commandes(ptr_commandes, indice_client);

	}
	printf("\n");
}

// supervision -------------------------
/* La fonction traite_supervision() affiche l'entête de consultation de l'avancement des commandes.*/
void traite_supervision(Specialites* ptr_specialites, Commandes* ptr_commandes) {
	for (int i = 0; i < ptr_commandes->nb_commandes; ++i) {
		Booleen virgule = FAUX;
		printf(MSG_SUPERVISION, ptr_commandes->tab_commandes[i].nom);
		for (int j = 0; j < ptr_specialites->nb_specialites; ++j) {
			if (ptr_commandes->tab_commandes[i].tache_par_specialite[j].nb_heures_requises != 0) {
				if (virgule == VRAI) {
					printf(", ");
				}
				printf("%s:%d/%d", ptr_specialites->tab_specialites[j].nom, ptr_commandes->tab_commandes[i].tache_par_specialite[j].nb_heures_effectuees, ptr_commandes->tab_commandes[i].tache_par_specialite[j].nb_heures_requises);
				virgule = VRAI;
			}
		}
	}
	printf("\n");
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
	Commandes commandes;
	commandes.nb_commandes = 0;
	
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
			traite_commande(&commandes, &clients);
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			traite_tache(&specialites, &commandes);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression(&specialites, &commandes);
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
			traite_client(&clients, &commandes);
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision(&specialites, &commandes);
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
