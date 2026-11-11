#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//Konstante per numrin  max te perdoruesve, nr max te kurseve, sa lende duhet te kete nje kurs, dhe mbaj gjithashtu te fiksuar username-in dhe fjalekalimin e administratorit
#define perdoruesit_maksimumi 100      // Numri maksimal i perdoruesve
#define kurse_maksimumi 50           // Numri maksimal i kurseve
#define lende_per_kurs 10           // Numri maksimal i lendeve per kurs
#define admin_emri "admin"     // Emri i administratorit 
#define admin_fjalekalimiiiiiiii "admin123"  // Fjalekalimi i administratorit 

//Struktura per te dhenat e perdoruesit
typedef struct {
    char emer[50];            // Emri i perdoruesit
    char fjalekalim[50];      // Fjalekalimi i perdoruesit
    char rol[20];             // Roli: "admin", "mesues" ose "student"
    int id;                   // ID e vecante e perdoruesit
} Perdorues;

//Struktura per te dhenat e lendes
typedef struct {
    char titull[100];         // Titulli i lendes
    int id;                   // ID e vecante e lendes
    char pershkrim[200];      // Pershkrimi i lendes
} Lende;

//Struktura per te dhenat e kursit
typedef struct {
    char titull[100];         // Titulli i kursit
    int id;                   // ID e vecante e kursit
    char pershkrim[200];      // Pershkrimi i kursit
    Lende lende[lende_per_kurs];   // Vektori i lendeve ne kurs
    int numriLendeve;         // Numri aktual i lendeve ne kurs
} Kurs;

//Struktura per te dhenat e progresit
typedef struct {
    int idPerdorues;          // ID e perdoruesit
    int idKurs;               // ID e kursit
    int progres;              // Progresi ne perqindje (0-100%)
} Progres;

//Variablat globale te cilat do te therriten ne funksionet e meposhtem
Perdorues listaUser[perdoruesit_maksimumi];  // Vektori i perdoruesve
Kurs grupet[kurse_maksimumi];                 // Vektori i kurseve
Progres rritjet[perdoruesit_maksimumi * kurse_maksimumi];  // Vektori i progreseve
int nrUser = 0;              // Numri aktual i perdoruesve
int nrKurse = 0;                  // Numri aktual i kurseve
int nrRritje = 0;               // Numri aktual i progreseve
Perdorues *useri = NULL;     // Pointer per perdoruesin aktual

//unksioni shkruan te dhenat e perdoruesve ne skedar binar listaUser.bin,  nese skedari nuk mund te hapet, skedar do te jete NULL
void ruajPerdoruesit() {
    FILE *skedar = fopen("listaUser.bin", "wb");
    if (skedar) {
        fwrite(&nrUser, sizeof(int), 1, skedar); // Shkruan numrin e perdoruesve (nrUser) ne skedar
        // sizeof(int) - madhesia e nje integer ne bytes
        // 1 - numri i elementeve per te shkruar
        // skedar - pointeri i skedarit ku do te shkruhet
        fwrite(listaUser, sizeof(Perdorues), nrUser, skedar);// Shkruaj te gjithe perdoruesit nga vektori listaUser ne skedar
        fclose(skedar);
    }
}
// // funksioni ngarkon te dhenat e perdoruesve nga skedar binar listaUser.bin, ai hap skedarin per te lexuar
void ngarkoPerdoruesit() {
    FILE *skedar = fopen("listaUser.bin", "rb");
    if (skedar) {
        fread(&nrUser, sizeof(int), 1, skedar); // Lexon numrin e perdoruesve (nrUser) nga skedari
        // sizeof(int) - madhesia e nje integer ne bytes
        // 1 - numri i elementeve per te lexuar
        // skedar - pointeri i skedarit ku lexohet
        fread(listaUser, sizeof(Perdorues), nrUser, skedar);//Lexon te gjithe perdoruesit nga skedari ne vektori listaUser
        fclose(skedar);//mbyll skedarin pasi te dhenat jane lexuar
    } else {
        // Krijon administratorin  nese skedari nuk ekziston ose nuk mund te hapet

        strcpy(listaUser[0].emer, admin_emri);
        strcpy(listaUser[0].fjalekalim, admin_fjalekalimiiiiiiii);
        strcpy(listaUser[0].rol, "admin");
        listaUser[0].id = 1;// vendos ID 1 per administratorin
        nrUser = 1;// vendos nr e perdoruesve ne 1 per cdo rol ne kete rat per administratorin
        ruajPerdoruesit();// ruan administratorin ne skedar
    }
}
// // funksioni ruan te dhenat e kurseve ne skedar, funksioni hap skedarin grupet.bin per te shkruar nr e kurseve(nrKurse) ne skedar ku shenon adresen e ndryshores qe do te shkruhet, nr e elementeve qe duhen shkruar dhe pointer  
void ruajKurset() {
    FILE *skedar = fopen("grupet.bin", "wb");
    if (skedar) {
        fwrite(&nrKurse, sizeof(int), 1, skedar);
        fwrite(grupet, sizeof(Kurs), nrKurse, skedar); // Shkruan të gjitha kurset nga vargu 'grupet' në skedar
        // - grupet: vargu i strukturave Kurs që do të ruhen
        // - sizeof(Kurs): madhësia e një strukture Kurs
        // - nrKurse: numri i kurseve për të shkruar
        fclose(skedar);// mbyll skedarin 
    }
    //nese skedari nuk hapet, funksioni perfundon pa bere gje
}
// // funksioni ngarkon te dhenat e kurseve nga skedari grupet.bin per  te lexuar te dhenat e te gjithave kurseve ne vektorin,
void ngarkoKurset() {
    FILE *skedar = fopen("grupet.bin", "rb");
    if (skedar) {
        fread(&nrKurse, sizeof(int), 1, skedar);
        fread(grupet, sizeof(Kurs), nrKurse, skedar);//lexon vektoret ku do te ruhen kurset e lexuara, madhesine e struktures se kursit, nr e kurseve qe do lexohen
        fclose(skedar);//mbyll skedarin ne perfundim
    }
    //nese skedari nuk ekziston, funksioni perfundon pa bere gje
}
// // funksioni shkruan te dhenat e progresit ne skedarin progres.bin; nese skedari eshte ekzistues atehere do te mbishkruhet dhe nese nuk ekziston atehere do te krijohet
void ruajProgreset() {
    FILE *skedar = fopen("progres.bin", "wb");
    if (skedar) {
        fwrite(&nrRritje, sizeof(int), 1, skedar);//adresa e variablës që do të shkruhet;madhësia në bytes e të dhënave;numri i elementeve për të shkruar
        fwrite(rritjet, sizeof(Progres), nrRritje, skedar);//Shkruaj të gjitha progreset nga vargu 'rritjet' në skedar
        // rritjet: vektori i strukturave Progres që do të ruhen
        //nrRritje: numri i progreseve për të shkruar
        fclose(skedar);//mbyll skedarin
    }
    //nese skedari (skedar==NULL), funksioni perfundon pa bere asgje 
}
//funksioni ngarkon te dhenat e progresin nga skedar 
void ngarkoProgreset() {
	// Hap skedarin për lexim
    FILE *skedar = fopen("progres.bin", "rb");
    if (skedar) {
        fread(&nrRritje, sizeof(int), 1, skedar);// lexon numrin e progreseve
        fread(rritjet, sizeof(Progres), nrRritje, skedar);// Lexo të gjitha progreset
        fclose(skedar);
    }
     // Nëse skedari nuk hapet, nuk bëhet asgjë
}
//funksioni fillon te dhenat e programit duke ngarkuar nga skedaret
void fillo() {
    ngarkoPerdoruesit();// funksion qe ngarkon perdoruesit nga skedari
    ngarkoKurset();// funksion qe ngarkon kurset nga skedari
    ngarkoProgreset();// funksion qe ngarkon progreset nga skedari
}
// per te gjetur  nje perdorues ne baze te emrit
Perdorues* gjejPerdorues(char *emer) {  // Kontrollon cdo përdorues në listë
    for (int i = 0; i < nrUser; i++) { // Krahason emrin e përdoruesit me emrin e kërkuar
        if (strcmp(listaUser[i].emer, emer) == 0) {  // Nëse gjendet, kthe pointerin e atij përdoruesi
            return &listaUser[i];
        }
    }
    return NULL;// Nëse nuk gjendet, kthe NULL
}
//per te kerkuar nje kurs ne baze te id
Kurs* gjejKurs(int id) {// Kontrollon çdo kurs në listë
    for (int i = 0; i < nrKurse; i++) {// Krahaso ID-në e kursit me ID-në e kërkuar
        if (grupet[i].id == id) {
            return &grupet[i];// Nëse gjen përputhje, kthen pointerin e kursit
        }
    }
    return NULL; // Nëse nuk gjendet, kthen NULL
}
// per te kerkuar progresin e nje perdoruesi per nje kurs
Progres* gjejProgres(int idPerdorues, int idKurs) {     // Kontrollon çdo regjistrim progresi
    for (int i = 0; i < nrRritje; i++) {// Kontrollon përputhjen e ID-së së studentit dhe ID-së së kursit
        if (rritjet[i].idPerdorues == idPerdorues && rritjet[i].idKurs == idKurs) {
            return &rritjet[i]; // Kthen progresin nëse gjendet
        }
    }
    return NULL; // Kthen NULL nëse nuk ekziston progresi
}
//prototipet e funksioneve ne kete kod 
// Funksione për menaxhimin e skedarëve
void ruajPerdoruesit();
void ngarkoPerdoruesit();
void ruajKurset();
void ngarkoKurset();
void ruajProgreset();
void ngarkoProgreset();
void fillo();

// Funksione për kërkim
Perdorues* gjejPerdorues(char *emer);
Kurs* gjejKurs(int id);
Progres* gjejProgres(int idPerdorues, int idKurs);

// Funksione për autentifikim
void regjistroPerdorues();
void logIn();
void dil();

// Funksione për menaxhimin e përdoruesve
void shtoPerdorues();
void modifikoPerdorues();
void fshiPerdorues();
void listoPerdoruesit();

// Funksione për menaxhimin e kurseve
void shtoKurs();
void modifikoKurs();
void fshiKurs();
void listoKurset();

// Funksione për menaxhimin e lëndëve
void shtoLendeNeKurs(Kurs *kurs);
void modifikoLendeNeKurs(Kurs *kurs);
void fshiLendeNgaKurs(Kurs *kurs);
void menaxhoLendet();

// Funksione për regjistrim dhe progres
void regjistroNeKurs();
void kompletoLende();
void shfaqProgresin();

// Funksione për menutë
void menuAdmin();
void menuMesues();
void menuStudent();
void menuKryesore();
// funksioni regjistron nje perdorues te ri ne program
void regjistroPerdorues() {
    if (nrUser >= perdoruesit_maksimumi) {// Kontrollon nëse ka arritur ose jo numri max i perdoruesve ne sistem
        printf("Numri maksimal i perdoruesve eshte arritur!\n");
        return;
    }
// Krijon një përdorues të ri
    Perdorues perdoruesIRi;
    printf("Emri: ");
    scanf("%s", perdoruesIRi.emer);
     // Kontrollon nëse ekziston përdoruesi me të njëjtin emër
    if (gjejPerdorues(perdoruesIRi.emer)) {
        printf("Ky emer ekziston!\n");
        return;
    }
// Merr të dhënat nga përdoruesi si emri, fjalekalimi dhe roli
    printf("Fjalekalimi: ");
    scanf("%s", perdoruesIRi.fjalekalim);
    
    printf("Roli (mesues/student): ");
    scanf("%s", perdoruesIRi.rol);
   
    // Verifikon që roli të jetë valid
    if (strcmp(perdoruesIRi.rol, "mesues") != 0 && strcmp(perdoruesIRi.rol, "student") != 0) {
        printf("Roli duhet te jete mesues ose student!\n");
        return;
    }
 // Cakton ID te vecante dhe shton përdoruesin në listë
    perdoruesIRi.id = nrUser + 1;
    listaUser[nrUser++] = perdoruesIRi;
    ruajPerdoruesit();// Ruaj ndryshimet në skedar
    printf("Regjistrimi u krye me sukses!\n");

}
//funksioni ben log in e nje perdoruesi ekzistues ne program
void logIn() {
    char emer[50], fjalekalim[50];
    printf("Emri: ");
    scanf("%s", emer);
    printf("Fjalekalimi: ");
    scanf("%s", fjalekalim);

    Perdorues *perdorues = gjejPerdorues(emer); // Kërkon përdoruesin në bazë të emrit
    // Verifikon te dhenat
    if (perdorues && strcmp(perdorues->fjalekalim, fjalekalim) == 0) {
        useri = perdorues;// Vendos përdoruesin aktual
        printf("Miresevini, %s!\n", perdorues->emer);
    } else {  // nqs te dhenat jane të pasakta
        printf("Emer ose fjalekalim i gabuar!\n");
    }
}
//funksioni ben log out nga llogaria aktuale
void dil() {
    useri = NULL;// Reset pointerin e përdoruesit momental
    printf("Ju keni dale nga llogaria juaj.\n");
}
//funksioni ben shtimin e nje perdoruesi te ri (vet admin)
void shtoPerdorues() {
    if (!useri || strcmp(useri->rol, "admin") != 0) {// Kontrollon nëse përdoruesi aktual është administrator
        printf("Vetem administratori mund te shtoje perdorues!\n");
        return;
    }
    regjistroPerdorues(); // therret funksionin e regjistrimit të përdoruesve
}
//funksioni ben modifikimin e te dhenat e nje perdoruesi (vet admin)
void modifikoPerdorues() {
    if (!useri || strcmp(useri->rol, "admin") != 0) {// Kontrollon nese useri eshte admin
        printf("Vetem administratori mund te modifikoje perdorues!\n");
        return;
    }
 // Merr emrin e përdoruesit për modifikim
    char emer[50];
    printf("Shkruani emrin e listaUser qe doni te modifikoni: ");
    scanf("%s", emer);
   // Gjen përdoruesin
    Perdorues *perdorues = gjejPerdorues(emer);
    if (!perdorues || strcmp(perdorues->rol, "admin") == 0) {// Kontrollon nee eshte ne rolin e admin te mos e modifikoje
        printf("Perdoruesi nuk ekziston ose nuk mund te modifikohet!\n");
        return;
    }
 // Merr të dhënat e reja
    printf("Fjalekalim i ri: ");
    scanf("%s", perdorues->fjalekalim);
    printf("Roli i ri (mesues/student): ");
    scanf("%s", perdorues->rol);

    ruajPerdoruesit();//ruan ndryshimet
    printf("Perdoruesi u modifikua me sukses!\n");
}
//funksioni ben thirjen e nje perdoruesi (vet admin)
void fshiPerdorues() {
	// Kontrollon nëse përdoruesi aktual është administrator
    if (!useri || strcmp(useri->rol, "admin") != 0) {
        printf("Vetem administratori mund te fshije perdorues!\n");
        return;
    }
 // Merr emrin e përdoruesit që do të fshihet
    char emer[50];
    printf("Shkruani emrin e listaUser qe doni te fshini: ");
    scanf("%s", emer);
 // Kërkon përdoruesin në listë, dhe kontrollon nëse përputhet emri dhe nuk është administrator
    for (int i = 0; i < nrUser; i++) {
        if (strcmp(listaUser[i].emer, emer) == 0 && strcmp(listaUser[i].rol, "admin") != 0) {
            for (int j = i; j < nrUser - 1; j++) {// Zhvendos të gjithë përdoruesit pasues për një pozicion mbrapa
                listaUser[j] = listaUser[j + 1];
            }
            nrUser--;// Zvogëlon numrin e përdoruesve
            ruajPerdoruesit();
            printf("Perdoruesi u fshi me sukses!\n");
            return;
        }
    }
    printf("Perdoruesi nuk ekziston ose nuk mund te fshihet!\n");
}
//funksioni ben listimin te gjithe perdoruesit (vet admin) nga skedari i perdoruesve listUser.bin
void listoPerdoruesit() {
    if (!useri || strcmp(useri->rol, "admin") != 0) {// Kontrollon nese perdoruesi aktual eshte administrator
        printf("Vetem administratori mund te liste listaUser!\n");
        return;
    }
   // Shfaq titullin e listes
    printf("\nLista e perdoruesve:\n");
    for (int i = 0; i < nrUser; i++) {  // kalon neper te gjithe perdoruesit
        printf("ID: %d, Emri: %s, Roli: %s\n", // afishon detajet e perdoruesit
               listaUser[i].id, listaUser[i].emer, listaUser[i].rol);
    }
}
//funksioni ben shtimin e nje kurs te ri (admin ose mesuesi)
void shtoKurs() {
    if (!useri || (strcmp(useri->rol, "admin") != 0 && strcmp(useri->rol, "mesues") != 0)) {//kontrollon nese eshte admin ose mesus
        printf("Vetem administratori ose mesuesi mund te shtoje kurs!\n");
        return;
    }
//kontrollon nese eshte kaluar ose jo numri max i kurseve
    if (nrKurse >= kurse_maksimumi) {
        printf("Numri maksimal i kurseve eshte arritur!\n");
        return;
    }

    Kurs kursIRi;//krijon nje kurs te ri ku merr nga perdoruesi titullin dhe pershkrimin
    printf("Titulli i kursit: ");
    scanf(" %[^\n]", kursIRi.titull);
    printf("Pershkrimi: ");
    scanf(" %[^\n]", kursIRi.pershkrim);
    
    kursIRi.id = nrKurse + 1;//id rritet
    kursIRi.numriLendeve = 0;//fillon kursin pa lende ne te
    
    grupet[nrKurse++] = kursIRi;//shton kursin ne liste
    ruajKurset();//ruan ndryshimet
    printf("Kursi u shtua me sukses!\n");
}
//funksioni ben modifikimin e  te dhenat si titulli apo pershkrimin  e nje kursi (admin ose mesuesi)
void modifikoKurs() {
    if (!useri || (strcmp(useri->rol, "admin") != 0 && strcmp(useri->rol, "mesues") != 0)) {//kontrollon nese eshte ne rolin e adminit ose mesuesit
        printf("Vetem administratori ose mesuesi mund te modifikoje kurs!\n");
        return;
    }
//kerkon iD e kursit qe do i behet modifikimi
    int id;
    printf("Shkruani ID e kursit qe doni te modifikoni: ");
    scanf("%d", &id);
//kerkon kursin ne baze te id
    Kurs *kurs = gjejKurs(id);
    if (!kurs) {
        printf("Kursi nuk ekziston!\n");//kur kursi nuk eshte jep mesazhin qe nuk ekziston
        return;
    }
//merr te dhenat e reja si titulli apopershkrimi
    printf("Titulli i ri (%s): ", kurs->titull);
    scanf(" %[^\n]", kurs->titull);
    printf("Pershkrimi i ri (%s): ", kurs->pershkrim);
    scanf(" %[^\n]", kurs->pershkrim);

    ruajKurset();//ruan ndryshimet
    printf("Kursi u modifikua me sukses!\n");
}
//funksioni ben fshirjen e nje kursi (admin ose mesuesi)
void fshiKurs() {
    if (!useri || (strcmp(useri->rol, "admin") != 0 && strcmp(useri->rol, "mesues") != 0)) {
        printf("Vetem administratori ose mesuesi mund te fshije kurs!\n");
        return;
    }
 // Merr ID e kursit për të fshirë
    int id;
    printf("Shkruani ID e kursit qe doni te fshini: ");
    scanf("%d", &id);
//kerkon kursin ne listen e kurseve ne skedarin
    for (int i = 0; i < nrKurse; i++) {
        if (grupet[i].id == id) {
            for (int j = i; j < nrKurse - 1; j++) { // Zhvendos kurset pasuese për të mbyllur boshllëkun
                grupet[j] = grupet[j + 1];
            }
            nrKurse--;//zvogelon nr e kurseve
            ruajKurset();//ruan ndryshimet ne skedarin perkates
            printf("Kursi u fshi me sukses!\n");
            return;
        }
    }
    printf("Kursi nuk ekziston!\n");
}
//funksioni ben listimin pra leximin nga skedari i te gjitha kurseve
void listoKurset() {
    printf("\nLista e kurseve:\n"); // Iteron neper te gjitha kurset
    for (int i = 0; i < nrKurse; i++) {// Shfaq informacionin baze te kursit
        printf("%d. %s - %s\n", grupet[i].id, grupet[i].titull, grupet[i].pershkrim);
        printf("Lendet:\n");// Shfaq lendet e kursit
        for (int j = 0; j < grupet[i].numriLendeve; j++) {
            printf("  %d. %s - %s\n", grupet[i].lende[j].id, grupet[i].lende[j].titull, grupet[i].lende[j].pershkrim);
        }
    }
}
//funksioni ben shtimin e nje lende te re ne kurs
void shtoLendeNeKurs(Kurs *kurs) {
    if (kurs->numriLendeve >= lende_per_kurs) {   // Kontrollon nëse kursi ka hapësirë për lëndë të reja
        printf("Kursi ka arritur numrin maksimal te lendeve!\n");
        return;
    }
//krijon nje lende te re
    Lende lendeERe;//merr te dhenat si titulli, pershkrimin per lenden e re qe do te shtohet
    printf("Titulli i lendes: ");
    scanf(" %[^\n]", lendeERe.titull);
    printf("Pershkrimi: ");
    scanf(" %[^\n]", lendeERe.pershkrim);
     // Cakton ID  për lëndën dhe e shton në kurs
    lendeERe.id = kurs->numriLendeve + 1;
    kurs->lende[kurs->numriLendeve++] = lendeERe;
    printf("Lenda u shtua me sukses!\n");
}
//funksioni ben modifikimin e nje lende ekzistuese ne kurs
void modifikoLendeNeKurs(Kurs *kurs) {
    if (kurs->numriLendeve == 0) {// kontrollon nese kursi ka lende ose jo
        printf("Nuk ka lende ne kete kurs!\n");
        return;
    }
//shfaq listen me te gjithe lendet e kursit
    printf("Lendet e kursit %s:\n", kurs->titull);
    for (int i = 0; i < kurs->numriLendeve; i++) {
        printf("%d. %s - %s\n", kurs->lende[i].id, 
               kurs->lende[i].titull, kurs->lende[i].pershkrim);
    }
 // Merr ID e lëndës për modifikim
    int idLende;
    printf("Shkruani ID e lendes qe doni te modifikoni: ");
    scanf("%d", &idLende);
// Kontrollon vlefshmërinë e ID-së
    if (idLende < 1 || idLende > kurs->numriLendeve) {
        printf("ID e pavlefshme!\n");
        return;
    }
// Gjen dhe modifikon lëndën duke ndryshuar titullin dhe pershkrimin e ri
    Lende *lende = &kurs->lende[idLende-1];
    printf("Titulli i ri (%s): ", lende->titull);
    scanf(" %[^\n]", lende->titull);
    printf("Pershkrimi i ri (%s): ", lende->pershkrim);
    scanf(" %[^\n]", lende->pershkrim);

    printf("Lenda u modifikua me sukses!\n");
}
//funksioni ben fshirjen e nje teme ekzistuese nga nje kurs
void fshiLendeNgaKurs(Kurs *kurs) {
    if (kurs->numriLendeve == 0) {//kontrollon nese kursi ka lende ne te ose jo
        printf("Nuk ka lende ne kete kurs!\n");
        return;
    }
//shfaq listen me lendet e kursit 
    printf("Lendet e kursit %s:\n", kurs->titull);
    for (int i = 0; i < kurs->numriLendeve; i++) {
        printf("%d. %s - %s\n", kurs->lende[i].id, 
               kurs->lende[i].titull, kurs->lende[i].pershkrim);
    }
//keerkon id e lendes qe deshirojme te fshijme
    int idLende;
    printf("Shkruani ID e lendes qe doni te fshini: ");
    scanf("%d", &idLende);
//kontrollon nese id eshte ne skedar ose jo
    if (idLende < 1 || idLende > kurs->numriLendeve) {
        printf("ID e pavlefshme!\n");
        return;
    }
//fshin lendet duke zhvendosur lendet pasardhese
    for (int i = idLende-1; i < kurs->numriLendeve-1; i++) {
        kurs->lende[i] = kurs->lende[i+1];
        kurs->lende[i].id = i+1;
    }
    kurs->numriLendeve--;//zvogelon nr e lendeve te kursit
    printf("Lenda u fshi me sukses!\n");
}
//funksioni sjell menune e menaxhmit te lendeve e nje kursi (admin ose mesuesi)
void menaxhoLendet() {
    if (!useri || (strcmp(useri->rol, "admin") != 0 && 
                        strcmp(useri->rol, "mesues") != 0)) {//kontrollon nese useri eshte admin apo mesues
        printf("Vetem administratori ose mesuesi mund te menaxhojne lende!\n");
        return;
    }
//shfaq listen me kurset ekzistuese dhe kerkon id e kursit qe duam ti menanaxhojme lendet
    listoKurset();
    int idKurs;
    printf("Shkruani ID e kursit qe doni te menaxhoni lendet: ");
    scanf("%d", &idKurs);
//kerkon kursin ne skedar
    Kurs *kurs = gjejKurs(idKurs);
    if (!kurs) {
        printf("Kursi nuk ekziston!\n");//kur kur nuk eshte ose nuk gjendet
        return;
    }
//menu e menaxhmit te lendeve
    int zgjedhje;
    do {
        printf("\n Menu Per Menaxhimin E Lendeve\n");
        printf("1. Shto lende te re\n");
        printf("2. Modifiko lende ekzistuese\n");
        printf("3. Fshi lende\n");
        printf("4. Listo te gjitha lendet\n");
        printf("5. Kthehu ne menu kryesore\n");
        printf("Zgjedhja juaj: ");
        scanf("%d", &zgjedhje);

        switch (zgjedhje) {
            case 1: shtoLendeNeKurs(kurs); ruajKurset(); break;
            case 2: modifikoLendeNeKurs(kurs); ruajKurset(); break;
            case 3: fshiLendeNgaKurs(kurs); ruajKurset(); break;
            case 4: //shfaq lenden 
                printf("\nLendet e kursit %s:\n", kurs->titull);
                for (int i = 0; i < kurs->numriLendeve; i++) {
                    printf("%d. %s - %s\n", kurs->lende[i].id, 
                           kurs->lende[i].titull, kurs->lende[i].pershkrim);
                }
                break;
            case 5: break;//dalim nga menuja
            default: printf("Zgjedhje e pavlefshme!\n");
        }
    } while (zgjedhje != 5);//perserit derisa useri te zgjedhe daljen
}
//funksioni ben regjistrimin e studentin ne nje kurs
void regjistroNeKurs() {
    if (!useri || strcmp(useri->rol, "student") != 0) {
        printf("Vetem studentet mund te regjistrohen ne kurs!\n");
        return;//kontrollon nese useri eshte student ose jo 
    }
//liston kurset ekzistuese ne skedar
    listoKurset();
    int id;//kerkon id e kursit ku do rregjistrohet
    printf("Shkruani ID e kursit qe doni te regjistroheni: ");
    scanf("%d", &id);
//kontrollon nese id e kursit eshte ne skedar pra ekziston
    if (!gjejKurs(id)) {
        printf("Kursi nuk ekziston!\n");
        return;
    }
//kontrollon nese studenti esht i rregjistruar ne kurs ose jjo
    if (gjejProgres(useri->id, id)) {
        printf("Ju jeni regjistruar ne kete kurs tashme!\n");
        return;
    }
//kontrollon nese kalon apo jo numri i pjesetareve ne kurs
    if (nrRritje >= perdoruesit_maksimumi * kurse_maksimumi) {
        printf("Numri maksimal i regjistrimeve eshte arritur!\n");
        return;
    }
//rregjistron studentin me id, id e kursit dhe progres fillestar 0%
    rritjet[nrRritje].idPerdorues = useri->id;
    rritjet[nrRritje].idKurs = id;
    rritjet[nrRritje].progres = 0;
    nrRritje++;//rritet nr i rregjistrimeve
    ruajProgreset();
    printf("Ju u regjistruat ne kurs me sukses!\n");
}
//funksioni ben perfundmin e nje lende dhe rrit progresin
void kompletoLende() {
    if (!useri || strcmp(useri->rol, "student") != 0) {
        printf("Vetem studentet mund te kompletojne lende!\n");
        return;
    }

    listoKurset();
    int idKurs;
    printf("Shkruani ID e kursit: ");
    scanf("%d", &idKurs);
//shfaq listen e kurseve ku studenti eshte rregjistrruar
    Progres *progres = gjejProgres(useri->id, idKurs);
    if (!progres) {
        printf("Ju nuk jeni regjistruar ne kete kurs!\n");
        return;
    }
//kontron a eshte si kurs ekzistues ose jo
    Kurs *kurs = gjejKurs(idKurs);
    if (!kurs) {
        printf("Kursi nuk ekziston!\n");
        return;
    }
//shfaq lendet  e kursit
    printf("Lendet e kursit %s:\n", kurs->titull);
    for (int i = 0; i < kurs->numriLendeve; i++) {
        printf("%d. %s\n", kurs->lende[i].id, kurs->lende[i].titull);
    }
//kerkon id  e lendes qe duam te perfundojme
    int idLende;
    printf("Shkruani ID e lendes qe keni perfunduar: ");
    scanf("%d", &idLende);
//kontrollon nese id e lendes eshte ekzistuese ose jo
    if (idLende < 1 || idLende > kurs->numriLendeve) {
        printf("Lenda nuk ekziston!\n");
        return;
    }
//rrit progresin me 10%
    progres->progres += 10;
    ruajProgreset();//ruan ndryshimet
    printf("Ju keni perfunduar lenden %s. Progresi juaj tani eshte %d%%\n", 
           kurs->lende[idLende-1].titull, progres->progres);
}
//funksioni shfaq progresin e studentit ne kurset
void shfaqProgresin() {
    if (!useri || strcmp(useri->rol, "student") != 0) {
        printf("Vetem studentet mund te shikojne progresin!\n");
        return;
    }
    printf("\nProgresi juaj:\n");
    //shikon te gjitha regjistrimet e progresit
	for (int i = 0; i < nrRritje; i++) {
        if (rritjet[i].idPerdorues == useri->id) {//kerkon per regjistrimet qe i perkasin studentit aktual
            Kurs *kurs = gjejKurs(rritjet[i].idKurs);
            if (kurs) {//nese kusri eshte, atehere shfaq progresin
                printf("Kursi: %s - Progresi: %d%%\n", kurs->titull, rritjet[i].progres);
            }
        }
    }
}
//Menu per administratorin
void menuAdmin() {
    int zgjedhje;
    do {
        printf("\n Menuja e administratorit\n");
        printf("1. Shto perdorues\n");
        printf("2. Modifiko perdorues\n");
        printf("3. Fshi perdorues\n");
        printf("4. Listo te gjithe perdoruesit\n");
        printf("5. Shto kurs\n");
        printf("6. Modifiko kurs\n");
        printf("7. Fshi kurs\n");
        printf("8. Menaxho lendet\n");
        printf("9. Listo kurset\n");
        printf("10. Dil\n");
        printf("Zgjedhja juaj: ");
        scanf("%d", &zgjedhje);
        switch (zgjedhje) {
            case 1: shtoPerdorues(); break;
            case 2: modifikoPerdorues(); break;
            case 3: fshiPerdorues(); break;
            case 4: listoPerdoruesit(); break;
            case 5: shtoKurs(); break;
            case 6: modifikoKurs(); break;
            case 7: fshiKurs(); break;
            case 8: menaxhoLendet(); break;
            case 9: listoKurset(); break;
            case 10: break;
            default: printf("Zgjedhje e pavlefshme!\n");
        }
    } while (zgjedhje != 10);
}
//Menu per mesuesin
void menuMesues() {
    int zgjedhje;
    do {
        printf("\n Menuja e mesuesit \n");
        printf("1. Shto kurs\n");
        printf("2. Modifiko kurs\n");
        printf("3. Fshi kurs\n");
        printf("4. Menaxho lendet\n");
        printf("5. Listo kurset\n");
        printf("6. Dil\n");
        printf("Zgjedhja juaj: ");
        scanf("%d", &zgjedhje);
        switch (zgjedhje) {
            case 1: shtoKurs(); break;
            case 2: modifikoKurs(); break;
            case 3: fshiKurs(); break;
            case 4: menaxhoLendet(); break;
            case 5: listoKurset(); break;
            case 6: break;
            default: printf("Zgjedhje e pavlefshme!\n");
        }
    } while (zgjedhje != 6);
}
//Menu per studentin
void menuStudent() {
    int zgjedhje;
    do {
        printf("\n Menuja e studentit\n");
        printf("1. Regjistrohu ne kurs\n");
        printf("2. Kompleto lende\n");
        printf("3. Shiko progresin\n");
        printf("4. Listo kurset\n");
        printf("5. Dil\n");
        printf("Zgjedhja juaj: ");
        scanf("%d", &zgjedhje);
        switch (zgjedhje) {
            case 1: regjistroNeKurs(); break;
            case 2: kompletoLende(); break;
            case 3: shfaqProgresin(); break;
            case 4: listoKurset(); break;
            case 5: break;
            default: printf("Zgjedhje e pavlefshme!\n");
        }
    } while (zgjedhje != 5);
}
// Menu kryesore e programit
void menuKryesore() {
    int zgjedhje;
    do {
        printf("\n Menuja kryesore\n");
        printf("1. Log in\n");
        printf("2. Regjistrohu\n");
        printf("3. Dil\n");
        printf("Zgjedhja juaj: ");
        scanf("%d", &zgjedhje);
        switch (zgjedhje) {
            case 1: 
                logIn(); 
                if (useri) {
                    if (strcmp(useri->rol, "admin") == 0) {
                        menuAdmin();
                    } else if (strcmp(useri->rol, "mesues") == 0) {
                        menuMesues();
                    } else if (strcmp(useri->rol, "student") == 0) {
                        menuStudent();
                    }
                    dil();
                }
                break;
            case 2: 
                regjistroPerdorues(); 
                if (useri) {
                    if (strcmp(useri->rol, "mesues") == 0) {
                        menuMesues();
                    } else if (strcmp(useri->rol, "student") == 0) {
                        menuStudent();
                    }
                    dil();
                }
                break;
            case 3: 
                return;
            default: 
                printf("Zgjedhje e pavlefshme!\n");
        }
    } while (zgjedhje != 3);
}
// // Funksioni kryesor i programit
int main() {
    fillo();
    menuKryesore();
    return 0;
}
