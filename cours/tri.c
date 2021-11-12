#include <stdio.h>
#include <stdlib.h>
int* tri_tab(int* tab, int n)
{
	int* tab_trie = malloc(n * sizeof(int));	
	for(int i = 0; i < n; i++) // On recopie le tableau
		tab_trie[i] = tab[i];
		
	for(int i = 0; i < n; i++)
	{
		for(int j = 1; j < n; j++) // On reparcourt le tableau
		{
			// Si la case d'avant est plus grande que la case actuelle, on échange les positions
			if(tab_trie[j - 1] > tab_trie[j])
			{
				int temp = tab_trie[j];
				tab_trie[j] = tab_trie[j - 1];
				tab_trie[j - 1] = temp;
			}
		}
		
	}
	return tab_trie;
}

int main()
{
	int tab[10] = {9,8,7,6,5,4,3,2,1};
	int* tab_t = tri_tab(tab, 10);
	
	for(int i = 0; i < 10; i++)
		printf("%d ", tab_t[i]);
		
	printf("\n");
		
	free(tab_t); // On libère la mémoire
}
