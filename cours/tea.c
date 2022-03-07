#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// TEA Tiny Encryption Algorithm
//==============================


// vecteurs de test pour TEA
// référence : http://www.cix.co.uk/~klockstone/teavect.htm
//  1  : 41ea3a0a 94baa940
//  2  : 4e8e7829 7d8236d8
//  4  : b84e28af b6b62088
//  8  : 5ddf75d9 7a4ce68f
// 16  : 4ec5d2e2 5ada1d89
// 32  : 7d2c6c57 7a6adb4d
// 64  : 2bb0f1b3 c023ed11
//  1  : 00000000 00000000


// chiffrement
// clair[2] : clair 64 bits
// crypto[2] : cryptogramme calculé 64 bits
// k[4] : clé 128 bits
void tea_chiffre(uint32_t * clair,uint32_t * crypto, uint32_t * k)
{
    uint32_t    y=clair[0],
                z=clair[1],
                sum=0;
    int i;
    for (i=0;i<32;i++)
    {
        sum += 0x9E3779B9L;
        y += ((z << 4)+k[0]) ^ (z+sum) ^ ((z >> 5)+k[1]);
        z += ((y << 4)+k[2]) ^ (y+sum) ^ ((y >> 5)+k[3]);
    }
    crypto[0]=y; crypto[1]=z;
}

// déchiffrement
// crypto[2] : cryptogramme
// clair[2] : clair calculé
// k[4] : clé 128 bits
void tea_dechiffre(uint32_t* crypto ,uint32_t* clair, uint32_t*k)
{
    uint32_t    y=crypto[0],
                z=crypto[1],
                sum=0xC6EF3720L;
    int i;
    for (i=0;i<32;i++)
    {
        z -= ((y << 4)+k[2]) ^ (y+sum) ^ ((y >> 5)+k[3]);
        y -= ((z << 4)+k[0]) ^ (z+sum) ^ ((z >> 5)+k[1]);
        sum -= 0x9E3779B9L;
    }
    clair[0]=y; clair[1]=z;
}
int main()
{
    printf("Hello TEA!\n");

    /*// programme pour produire les suites de validation
    uint32_t pz[1024]={0,0,0,0,0,0};
    int n;
    for (n=1;n<=64;n++)
    {
        tea_chiffre(pz+n,pz+n,pz+n+2);
        if (n==(n&-n)) // si n est une puissance de 2
        {
            printf("%3d : %08x %08x\n", n, pz[n], pz[n+1]);
        }
        pz[n+6]=pz[n];
    }
    for (n=64; n>0; n--)
    {
        tea_dechiffre(pz+n, pz+n, pz+n+2);
    }
    n=1;
    printf("%3d : %08x %08x\n", n, pz[n], pz[n+1]);*/
    
    // clé : 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
    // à envoyer (BIG ENDIAN) : 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
    // à envoyer (LITTLE ENDIAN) : 10 0F 0E 0D 0C 0B 0A 09 08 07 06 05 04 03 02 01
	uint32_t cle[4] = {0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10};
	
	// COMMANDES EQUIVALENTES
	uint32_t clair[2] = {0xDAFF1012, 0x00091009}; // (BIG ENDIAN) entête : DA FF 10 12, compteur : 00 09, solde : 10 09
	//uint32_t clair[2] = {0x1210FFDA, 0x09000910}; // (LITTLE ENDIAN) entête : 12 10 FF DA, compteur : 09 00, solde : 09 10
	// FIN COMMANDES
	
	uint32_t chiffre[2], dechiffre[2];
	
	// affichage clair
	printf("%08x %08x\n", clair[0], clair[1]);
	
	tea_chiffre(clair, chiffre, cle);
	// (BIG ENDIAN) chiffré : 91 27 6A 7D 5B 4A B5 D1
	// (LITTLE ENDIAN) chiffré : E8 AF C3 01 0A 7C 02 8E -> à envoyer 8E 02 7C 0A 01 C3 AF E8
	// affichage chiffré
	printf("%08x %08x\n", chiffre[0], chiffre[1]);
	
	tea_dechiffre(chiffre, dechiffre, cle);
	// affichage déchiffré
	printf("%08x %08x\n", dechiffre[0], dechiffre[1]);
	
    return 0;
}

