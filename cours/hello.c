#include <io.h>
#include <inttypes.h>
#include <avr/eeprom.h>


//------------------------------------------------
// Programme "hello world" pour carte � puce
// 
//------------------------------------------------


// d�claration des fonctions d'entr�e/sortie d�finies dans "io.c"
void sendbytet0(uint8_t b);
uint8_t recbytet0(void);

// variables globales en static ram
uint8_t cla, ins, p1, p2, p3;	// ent�te de commande
uint8_t sw1, sw2;		// status word

uint8_t ee_taille EEMEM = 0;		// taille des donn�es introduites -- est initialis� � 0 avant la boucle
#define MAXI 128	// taille maxi des donn�es lues
uint8_t ee_data[MAXI] EEMEM;	// donn�es introduites

// Proc�dure qui renvoie l'ATR
void atr(uint8_t n, char* hist)
{
	sendbytet0(0x3b);	// d�finition du protocole
	sendbytet0(n);		// nombre d'octets d'historique
	while(n--)		// Boucle d'envoi des octets d'historique
	{
		sendbytet0(*hist++);
	}
}


// �mission de la version
// t est la taille de la cha�ne sv
void version(int t, char* sv)
{
	int i;
	// v�rification de la taille
	if (p3!=t)
	{
		sw1=0x6c;	// taille incorrecte
		sw2=t;		// taille attendue
		return;
	}
	sendbytet0(ins);	// acquittement
	// �mission des donn�es
	for(i=0;i<p3;i++)
	{
		sendbytet0(sv[i]);
	}
	sw1=0x90;
}


// commande de r�ception de donn�es
void intro_data()
{
	int i;
	// v�rification de la taille
	if (p3>MAXI)
	{
		sw1=0x6c;	// P3 incorrect
		sw2=MAXI;	// sw2 contient l'information de la taille correcte
		return;
	}
	sendbytet0(ins);	// acquitement

	uint8_t data[MAXI];
	for(i=0;i<p3;i++)	// boucle de r�ception du message
	{
		data[i]=recbytet0();
	}
	
	eeprom_write_block(data, ee_data, p3); // �criture du tableau dans l'EEPROM
	eeprom_write_byte(&ee_taille, p3); // m�morisation de la taille des donn�es lues
	
	sw1=0x90;
}

// commande d'�mission de donn�es
void sortir_data()
{
	int i;
	
	uint8_t taille = eeprom_read_byte(&ee_taille); // lecture de la taille dans l'EEPROM
	
	// v�rification de la taille
	if (p3!=taille)
	{
		sw1=0x6c;	// P3 incorrect
		sw2=taille;	// sw2 contient l'information de la taille correcte
		return;
    }
	sendbytet0(ins);	// acquitement

	uint8_t data[MAXI];
	eeprom_read_block(data, ee_data, p3); // lecture du tableau dans l'EEPROM
	
	for(i=0;i<p3;i++)	// boucle d'envoi du message
	{
		sendbytet0(data[i]);
	}

	sw1=0x90;
}



// Programme principal
//--------------------
int main(void)
{
  	// initialisation des ports
	ACSR=0x80;
	DDRB=0xff;
	DDRC=0xff;
	DDRD=0;
	PORTB=0xff;
	PORTC=0xff;
	PORTD=0xff;
	ASSR=1<<EXCLK;
	TCCR2A=0;
	ASSR|=1<<AS2;


	// ATR
  	atr(11,"Hello scard");

	taille=0;
	sw2=0;		// pour �viter de le r�p�ter dans toutes les commandes
  	// boucle de traitement des commandes
  	for(;;)
	{
		// lecture de l'ent�te
		cla=recbytet0();
		ins=recbytet0();
		p1=recbytet0();
		p2=recbytet0();
		p3=recbytet0();
		sw2=0;
		switch (cla)
		{
			case 0x80:
				switch(ins)
				{
					case 0:
						version(4,"1.00");
						break;
					case 1:
						intro_data();
						break;
					case 2:
						sortir_data();
						break;
					default:
						sw1=0x6d; // code erreur ins inconnu
				}
				break;
			default:
				sw1=0x6e; // code erreur classe inconnue
		}
		sendbytet0(sw1); // envoi du status word
		sendbytet0(sw2);
  	}
  	return 0;
}

