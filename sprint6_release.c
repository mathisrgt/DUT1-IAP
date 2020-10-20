/* sprint6_release.c
* Auteurs : Gregory TACQUET, Mathis SERGENT
* Date de création : 26/09/2020
*/

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes numériques ------------------------------------------------------
#define LGMOT 35
#define NBCHIFFREMAX 5 
#define MAX_SPECIALITES 10
#define MAX_TRAVAILLEURS 50
#define MAX_CLIENTS 10
#define MAX_COMMANDES 500
// Messages emis par les instructions -----------------------------------------
#define MSG_INTERRUPTION "## fin de programme\n" 
#define MSG_SPECIALITES "specialites traitees : "
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CLIENTS "le client %s a commande : "
#define MSG_SUPERVISION "etat des taches pour %s : "
#define MSG_CHARGE "charge de travail pour %s : "
#define MSG_FACTURATION "facturation %s : "
#define MSG_FACTURATIONS "facturations : "

// Lexemes -------------------------------------------------------------------- 
typedef char Mot[LGMOT + 1];

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

typedef struct {
	Mot nom;
	int cout_horaire;
} Specialite;

typedef struct {
	Specialite tab_specialites[MAX_SPECIALITES];
	unsigned int nb_specialites;
} Specialites;

typedef struct {
	Mot nom;
	Booleen tags_competences[MAX_SPECIALITES];
	int nombre_heures_a_effectuer;
} Travailleur;

typedef struct {
	Travailleur tab_travailleurs[MAX_TRAVAILLEURS];
	unsigned int nb_travailleurs;
} Travailleurs;

typedef struct {
	Mot tab_clients[MAX_CLIENTS];
	unsigned int nb_clients;
} Clients;

typedef struct {
	unsigned int nb_heures_requises;
	unsigned int nb_heures_effectuees;
	unsigned int id_travailleur;
}Tache;

typedef struct {
	Mot nom;
	unsigned int id_client;
	Tache tache_par_specialite[MAX_SPECIALITES]; // n_heure_requise==0 <=> pas de tache pour cette specialite
	unsigned int nb_taches;
	unsigned int id_derniere_tache_progression;
}Commande;

typedef struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
	unsigned int id_derniere_commande_progression;
	Booleen commandes_terminees[MAX_COMMANDES];
	Booleen commandes_affichees[MAX_COMMANDES];
}Commandes;

void get_id(Mot id);
int get_int();

void traite_developpe(Specialites* ptr_specialites);
int recherche_id_specialite(Specialites* ptr_specialites, Mot nom_specialite);
void traite_embauche(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs);
void traite_demarche(Clients* ptr_clients);
void traite_commande(Commandes* ptr_commandes, Clients* ptr_clients);
int recherche_id_commande(Commandes* ptr_commandes, Mot nom_commande);
void affiche_commandes(Commandes* ptr_commandes, int indice_client);
void traite_tache(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes);
int attribution_tache(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes, int indice_specialite, int nb_heures_a_ajouter);
void traite_progression(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes);
void traite_passe(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes);
int facturation(Specialites* ptr_specialites, Commandes* ptr_commandes, Clients* ptr_clients);
void traite_specialites(Specialites* ptr_specialites);
void traite_travailleurs(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs);
int recherche_id_travailleur(Travailleurs* ptr_travailleurs, Mot nom_travailleur);
void affiche_travailleur(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Mot nom_specialite);
void traite_client(Clients* ptr_clients, Commandes* ptr_commandes);
int recherche_id_client(Clients* ptr_clients, Mot nom_client);
void traite_supervision(Specialites* ptr_specialites, Commandes* ptr_commandes);
void traite_charge(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes);
void traite_interruption();

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
			traite_tache(&specialites, &travailleurs, &commandes);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression(&specialites, &travailleurs, &commandes);
			if (facturation(&specialites, &commandes, &clients) == 1) {
				break;
			}
			else
			{
				continue;
			}
		}
		if (strcmp(buffer, "passe") == 0) {
			traite_passe(&specialites, &travailleurs, &commandes);
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
			traite_charge(&specialites, &travailleurs, &commandes);
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

int recherche_id_specialite(Specialites* ptr_specialites, Mot nom_specialite) {
	int indice_specialite;
	for (indice_specialite = 0; indice_specialite < ptr_specialites->nb_specialites; ++indice_specialite) {
		if (strcmp(ptr_specialites->tab_specialites[indice_specialite].nom, nom_specialite) == 0) {
			break;
		}
	}
	return indice_specialite;
}

void traite_embauche(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs) {
	Mot nom_employe, nom_specialite;
	get_id(nom_employe);
	get_id(nom_specialite);
	int indice_travailleur;

	for (indice_travailleur = 0; indice_travailleur < ptr_travailleurs->nb_travailleurs; ++indice_travailleur) {
		if (strcmp(ptr_travailleurs->tab_travailleurs[indice_travailleur].nom, nom_employe) == 0) {

			for (int i = 0; i < (ptr_specialites->nb_specialites); ++i) {
				if (strcmp((ptr_specialites->tab_specialites[i]).nom, nom_specialite) == 0) {
					ptr_travailleurs->tab_travailleurs[indice_travailleur].tags_competences[i] = VRAI;
				}
			}
			break;
		}
	}

	if (indice_travailleur == ptr_travailleurs->nb_travailleurs) {
		Travailleur travailleur;
		travailleur.nombre_heures_a_effectuer = 0;
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
}

void traite_demarche(Clients* ptr_clients) {
	Mot nom_client;
	get_id(nom_client);

	strcpy(ptr_clients->tab_clients[ptr_clients->nb_clients], nom_client);
	++(ptr_clients->nb_clients);
}

void traite_commande(Commandes* ptr_commandes, Clients* ptr_clients) {
	Mot nom_commande, nom_client;
	Commande commande;
	commande.nb_taches = 0;
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

int recherche_id_commande(Commandes* ptr_commandes, Mot nom_commande) {
	int indice_commande;
	for (indice_commande = 0; indice_commande < ptr_commandes->nb_commandes; ++indice_commande) {
		if (strcmp(ptr_commandes->tab_commandes[indice_commande].nom, nom_commande) == 0) {
			break;
		}
	}
	return indice_commande;
}

void affiche_commandes(Commandes* ptr_commandes, int indice_client) {
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

void traite_tache(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes) {
	Mot nom_commande, nom_specialite;
	Tache tache;
	get_id(nom_commande);
	get_id(nom_specialite);
	tache.nb_heures_requises = get_int();
	tache.nb_heures_effectuees = 0;
	int indice_specialite, indice_commande;

	indice_specialite = recherche_id_specialite(ptr_specialites, nom_specialite);
	indice_commande = recherche_id_commande(ptr_commandes, nom_commande);

	tache.id_travailleur = attribution_tache(ptr_specialites, ptr_travailleurs, ptr_commandes, indice_specialite, tache.nb_heures_requises);

	ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite] = tache;
	++(ptr_commandes->tab_commandes[indice_commande].nb_taches);
}

int attribution_tache(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes, int indice_specialite, int nb_heures_a_ajouter) {
	int id_attribue = -1;
	int i = 0;
	int nb_heures_a_effectuer;

	for (i = 0; (id_attribue == -1) && (i < ptr_travailleurs->nb_travailleurs); ++i) {
		if (ptr_travailleurs->tab_travailleurs[i].tags_competences[indice_specialite] == VRAI) {
			id_attribue = i;
			nb_heures_a_effectuer = ptr_travailleurs->tab_travailleurs[i].nombre_heures_a_effectuer;
			break;
		}
	}

	for (i = (id_attribue + 1); i < ptr_travailleurs->nb_travailleurs; ++i) {
		if ((ptr_travailleurs->tab_travailleurs[i].tags_competences[indice_specialite] == VRAI) && (ptr_travailleurs->tab_travailleurs[i].nombre_heures_a_effectuer < nb_heures_a_effectuer)) {
			id_attribue = i;
			nb_heures_a_effectuer = ptr_travailleurs->tab_travailleurs[i].nombre_heures_a_effectuer;
		}
	}

	ptr_travailleurs->tab_travailleurs[id_attribue].nombre_heures_a_effectuer += nb_heures_a_ajouter;

	return id_attribue;
}

void traite_progression(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes) {

	Mot nom_commande, nom_specialite, passe_fonction;

	get_id(nom_commande);
	get_id(nom_specialite);
	int indice_specialite, indice_commande, nb_heures_effectuees;

	indice_specialite = recherche_id_specialite(ptr_specialites, nom_specialite);
	indice_commande = recherche_id_commande(ptr_commandes, nom_commande);
	nb_heures_effectuees = get_int();

	ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_effectuees += nb_heures_effectuees;
	ptr_travailleurs->tab_travailleurs[ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].id_travailleur].nombre_heures_a_effectuer -= nb_heures_effectuees;

	ptr_commandes->id_derniere_commande_progression = indice_commande;
	ptr_commandes->tab_commandes[indice_commande].id_derniere_tache_progression = indice_specialite;
}

void traite_passe(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes) {
	int id_commande = ptr_commandes->id_derniere_commande_progression;
	int id_specialite = ptr_commandes->tab_commandes[id_commande].id_derniere_tache_progression;
	int nb_heures_a_ajouter = ptr_commandes->tab_commandes[id_commande].tache_par_specialite[id_specialite].nb_heures_requises - ptr_commandes->tab_commandes[id_commande].tache_par_specialite[id_specialite].nb_heures_effectuees;

	ptr_travailleurs->tab_travailleurs[ptr_commandes->tab_commandes[id_commande].id_client].nombre_heures_a_effectuer -= nb_heures_a_ajouter;

	ptr_commandes->tab_commandes[id_commande].tache_par_specialite[id_specialite].id_travailleur = 0;
	--(ptr_commandes->tab_commandes[id_commande].nb_taches);

	ptr_commandes->tab_commandes[id_commande].tache_par_specialite[id_specialite].id_travailleur = attribution_tache(ptr_specialites, ptr_travailleurs, ptr_commandes, id_specialite, nb_heures_a_ajouter);
	++(ptr_commandes->tab_commandes[id_commande].nb_taches);
}

int facturation(Specialites* ptr_specialites, Commandes* ptr_commandes, Clients* ptr_clients) {
	int nb_commandes_terminees = 0;
	int nb_taches_terminees = 0;
	unsigned int cout_total = 0;
	int indice_client, indice_commande, indice_specialite;
	Booleen virgule;

	for (indice_commande = 0, nb_taches_terminees = 0; indice_commande < ptr_commandes->nb_commandes; ++indice_commande, nb_taches_terminees = 0) {
		for (indice_specialite = 0; indice_specialite < ptr_specialites->nb_specialites; ++indice_specialite) {
			if ((ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_requises == ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_effectuees) && (ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_requises != 0)) {
				++nb_taches_terminees;
			}
			else {
				if (ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_requises != ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_effectuees) {
					break;
				}
			}
		}

		if (nb_taches_terminees == ptr_commandes->tab_commandes[indice_commande].nb_taches) {
			++nb_commandes_terminees;
			ptr_commandes->commandes_terminees[indice_commande] = VRAI;
		}
		else {
			ptr_commandes->commandes_terminees[indice_commande] = FAUX;
		}
	}

	if (nb_commandes_terminees != 0) {
		virgule = FAUX;
		for (indice_commande = 0; indice_commande < ptr_commandes->nb_commandes; ++indice_commande) {
			if ((ptr_commandes->commandes_terminees[indice_commande] == VRAI) && (ptr_commandes->commandes_affichees[indice_commande] != VRAI)) {
				printf(MSG_FACTURATION, ptr_commandes->tab_commandes[indice_commande].nom);

				for (indice_specialite = 0; indice_specialite < ptr_specialites->nb_specialites; ++indice_specialite) {
					if (ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_requises != 0) {
						if (virgule == VRAI) {
							printf(", ");
						}
						printf("%s:%u", ptr_specialites->tab_specialites[indice_specialite].nom, (ptr_specialites->tab_specialites[indice_specialite].cout_horaire * ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_requises));
						virgule = VRAI;
					}
				}
				ptr_commandes->commandes_affichees[indice_commande] = VRAI;
				printf("\n");
			}
		}
	}

	if (nb_commandes_terminees == ptr_commandes->nb_commandes) { //toutes commandes sont finies
		printf(MSG_FACTURATIONS);
		virgule = FAUX;
		for (indice_client = 0; indice_client < ptr_clients->nb_clients; ++indice_client) {
			cout_total = 0;
			for (indice_commande = 0; indice_commande < ptr_commandes->nb_commandes; ++indice_commande) {
				if (ptr_commandes->tab_commandes[indice_commande].id_client == indice_client) {
					for (indice_specialite = 0; indice_specialite < ptr_specialites->nb_specialites; ++indice_specialite) {
						cout_total += ((ptr_commandes->tab_commandes[indice_commande].tache_par_specialite[indice_specialite].nb_heures_effectuees) * (ptr_specialites->tab_specialites[indice_specialite].cout_horaire));
					}
				}
			}
			if (cout_total != 0) {
				if (virgule == VRAI) {
					printf(", ");
				}
				printf("%s:%u", ptr_clients->tab_clients[indice_client], cout_total);
				virgule = VRAI;
			}
		}
		printf("\n");
		return 1;
	}

	return 0;
}

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

void traite_travailleurs(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs) {
	Mot nom_specialite;
	get_id(nom_specialite);
	int i = 0;

	if (strcmp(nom_specialite, "tous") == 0) {
		for (i = 0; i < ptr_specialites->nb_specialites; ++i) {
			strcpy(nom_specialite, ptr_specialites->tab_specialites[i].nom);
			affiche_travailleur(ptr_specialites, ptr_travailleurs, nom_specialite);
			printf("\n");
		}
	}
	else {
		affiche_travailleur(ptr_specialites, ptr_travailleurs, nom_specialite);
		printf("\n");
	}
}

int recherche_id_travailleur(Travailleurs* ptr_travailleurs, Mot nom_travailleur) {
	int indice_travailleur;
	for (indice_travailleur = 0; indice_travailleur < ptr_travailleurs->nb_travailleurs; ++indice_travailleur) {
		if (strcmp(ptr_travailleurs->tab_travailleurs[indice_travailleur].nom, nom_travailleur) == 0) {
			return indice_travailleur;
		}
	}
}

void affiche_travailleur(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Mot nom_specialite) {
	int i, j;
	Booleen virgule = FAUX;

	printf(MSG_TRAVAILLEURS, nom_specialite);
	for (i = 0; i < ptr_specialites->nb_specialites; ++i) {
		if (strcmp((ptr_specialites->tab_specialites[i]).nom, nom_specialite) == 0) {
			break;
		}
	}

	for (j = 0; j < ptr_travailleurs->nb_travailleurs; ++j) {
		if ((ptr_travailleurs->tab_travailleurs[j]).tags_competences[i] == VRAI) {
			if (virgule == VRAI) {
				printf(", ");
			}
			printf("%s", (ptr_travailleurs->tab_travailleurs[j]).nom);
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
			affiche_commandes(ptr_commandes, indice_client);
			printf("\n");
		}
	}
	else {
		indice_client = recherche_id_client(ptr_clients, nom_client);
		printf(MSG_CLIENTS, (ptr_clients->tab_clients[indice_client]));
		affiche_commandes(ptr_commandes, indice_client);
		printf("\n");
	}
}

int recherche_id_client(Clients* ptr_clients, Mot nom_client) {
	int indice_client;
	for (indice_client = 0; indice_client < ptr_clients->nb_clients; ++indice_client) {
		if (strcmp((ptr_clients->tab_clients[indice_client]), nom_client) == 0) {
			return indice_client;
		}
	}
}

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
		printf("\n");
	}
}

void traite_charge(Specialites* ptr_specialites, Travailleurs* ptr_travailleurs, Commandes* ptr_commandes) {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	int indice_travailleur, nb_heures_restantes;
	Booleen virgule;

	indice_travailleur = recherche_id_travailleur(ptr_travailleurs, nom_travailleur);

	virgule = FAUX;
	printf(MSG_CHARGE, ptr_travailleurs->tab_travailleurs[indice_travailleur].nom);

	for (int i = 0; i < ptr_commandes->nb_commandes; ++i) {
		for (int j = 0; j < MAX_SPECIALITES; j++)
		{
			nb_heures_restantes = (ptr_commandes->tab_commandes[i].tache_par_specialite[j].nb_heures_requises) - (ptr_commandes->tab_commandes[i].tache_par_specialite[j].nb_heures_effectuees);
			if ((ptr_commandes->tab_commandes[i].tache_par_specialite[j].id_travailleur == indice_travailleur) && (nb_heures_restantes != 0)) {
				if (virgule == VRAI) {
					printf(", ");
				}
				printf("%s/%s/%dheure(s)", ptr_commandes->tab_commandes[i].nom, ptr_specialites->tab_specialites[j].nom, nb_heures_restantes);
				virgule = VRAI;
			}
		}
	}
	printf("\n");
}

void traite_interruption() {
	printf(MSG_INTERRUPTION);
}
