#include <stdio.h>
#include <stdlib.h>

typedef struct noeud {
	int valeur;
	struct noeud * suivant;
} noeud_t;


/****** Méthodes primitives *********/
noeud_t * creer_noeud(int val, noeud_t* prec, noeud_t* suiv)
{
	noeud_t * actuel = malloc(sizeof(struct noeud));
	actuel->valeur = val;
	actuel->suivant = suiv;
	
	if(prec != NULL)
		prec->suivant = actuel;
		
	
	return actuel;
}

void supprimer_noeud(noeud_t * actuel, noeud_t * prec)
{
	if(prec != NULL)
		prec->suivant = actuel->suivant;
	
	if(actuel != NULL)
		free(actuel);
		
}

/****** Méthodes usuelles : elles renvoient toujours la tête de la liste (NULL si inexistante) *********/

noeud_t * ajouter_tete(int val, noeud_t * tete)
{
	return creer_noeud(val, NULL, tete);
}

// Si ajouter_queue renvoie une nouvelle tête c'est que la liste était vide
noeud_t * ajouter_queue(int val, noeud_t * tete)
{
	if(tete == NULL)
		return creer_noeud(val, NULL, NULL);
		
	noeud_t * queue_actuelle = tete;
	while(queue_actuelle->suivant != NULL)
		queue_actuelle = queue_actuelle->suivant;
	
	creer_noeud(val, queue_actuelle, NULL);
	
	return tete;
}

// Si supprimer_queue renvoie une nouvelle tête, c'est bon, s'il renvoie "NULL" c'est que la liste est vide
noeud_t * supprimer_tete(noeud_t * tete)
{
	// Si on n'a pas de liste
	if(tete == NULL)
		return NULL;
	
	noeud_t * tete_actuelle = tete->suivant;
	supprimer_noeud(tete, NULL);
	
	return tete_actuelle;
}

// Si supprimer_queue renvoie la même tête, c'est bon, s'il renvoie "NULL" c'est que la liste est vide
noeud_t * supprimer_queue(noeud_t * tete)
{
	// Si on n'a pas de liste
	if(tete == NULL)
		return NULL;
	
	// Si on n'a qu'un seul élément
	if(tete->suivant == NULL)
		return supprimer_tete(tete);
	
	// Le reste
	noeud_t * queue_actuelle = tete;
	while((queue_actuelle->suivant)->suivant != NULL)
		queue_actuelle = queue_actuelle->suivant;
		
		
	supprimer_noeud(queue_actuelle->suivant, queue_actuelle);
	return tete;
}

// Affichage de la liste
void affichage_liste(noeud_t * actuel)
{
	printf("{");
	noeud_t * ite = actuel;
	while(ite != NULL)
	{
		printf("%d", ite->valeur);
		ite = ite->suivant;
		if(ite != NULL)
			printf(",");
	}
	printf("}\n");
}

int main()
{
	noeud_t * principal = malloc(sizeof(struct noeud));
	principal->valeur = 10;
	principal->suivant = NULL;
	
	noeud_t * tete = principal;
	
	affichage_liste(tete);
	tete = ajouter_tete(100, tete);
	affichage_liste(tete);
	tete = supprimer_tete(tete);
	affichage_liste(tete);
	tete = ajouter_queue(2000, tete);
	tete = ajouter_queue(3000, tete);
	affichage_liste(tete);
	tete = supprimer_queue(tete);
	affichage_liste(tete);
	tete = supprimer_tete(tete);
	affichage_liste(tete);
	
	// On vide toute la chaîne de la mémoire
	while(tete != NULL)
		tete = supprimer_tete(tete);
	
	return 0;
}
