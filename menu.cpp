#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024

struct KeyValuePair{
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
};

const int x = 2;
const int y = 0;
const int width = 115;
const int height = 24;

char separator = 179;

void gotoxy(int x, int y) {
	COORD coordinat;
	coordinat.X = x;
	coordinat.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinat);
}

FILE *fileProperties;
const char filePropertiesName[] = "application.properties";

// Function to parse a line and extract key-value pair
int parseLine(char *line, KeyValuePair *kv) {
    // Skip comments and empty lines
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
        return 0; 
    }

    // Find the position of the equal sign
    char *equalSign = strchr(line, '=');
    if (!equalSign) {
        return -1; 
    }

    strncpy(kv->key, line, equalSign - line);
    kv->key[equalSign - line] = '\0'; 

    strcpy(kv->value, equalSign + 1);
    strtok(kv->value, "\n"); 

    return 1; 
}

void printHeaderTitle() {
	#ifdef _WIN32
	    // For Windows
	    system("cls");
//	    system("color 1F");
	#else
	    // For Linux and macOS
	    system("clear");
	#endif
	
	gotoxy(x, y); printf("%c", 201);	
	gotoxy(x, y+3); printf("%c", 204);
	
	for (int i=1; i<=width; i++){
		gotoxy(x+i, y); printf("%c", 196);	
		gotoxy(x+i, y+3); printf("%c", 196);
	}
	
	gotoxy(x+width, y); printf("%c", 187);
	gotoxy(x+width, y+3); printf("%c", 185);
	
	for (int i=0; i<2; i++ ) {
		gotoxy(x, y+1+i); printf("%c", 179);
		gotoxy(x+width, y+1+i); printf("%c", 179);	
	}	
	
	FILE *file = fopen(filePropertiesName, "r");
    if (!file) {
        perror("Error opening file");
    }
	
	char line[MAX_LINE_LENGTH];
    KeyValuePair kv;
    
    // Read and parse each line
    int i = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
    	
		int result = parseLine(line, &kv);
        if (result == 1) {
    		gotoxy(x+10, y+i); printf("%s", kv.value);
    		
        } else if (result == -1) {
            fprintf(stderr, "Error parsing line: %s", line);
        }
        i++;
    }

    fclose(file);
}

void drawline(int len){
	int loop = 0;
	do {
		printf("-");
		loop++;
	} while(loop < len);
}

void displayMenu() {
	printHeaderTitle();
	
	for (int i=0; i<=height; i++ ) {
		gotoxy(x, y+4+i); printf("%c", 179);
		gotoxy(x+width, y+4+i); printf("%c", 179);	
	}
	
	gotoxy(x, y+4+height); printf("%c", 200);
	for (int i=1; i<=width; i++){
		gotoxy(x+i, y+4+height); printf("%c", 196);	
	}
	gotoxy(x+width, y+4+height); printf("%c", 188);
		
    gotoxy(x+5,  y+5); printf("DAFTAR MENU");
    gotoxy(x+5,  y+6); drawline(40);
    gotoxy(x+5,  y+7); printf("1. Tambah Data Produk");
    gotoxy(x+5,  y+8); printf("2. Update Data Produk");
    gotoxy(x+5,  y+9); printf("3. Hapus Data Produk");
    gotoxy(x+5, y+10); printf("4. Lihat Daftar Produk");
	gotoxy(x+5, y+11); printf("5. Penjualan");
    gotoxy(x+5, y+12); printf("6. Lihat Penjualan (per Tanggal)");
    gotoxy(x+5, y+13); printf("7. Edit Profil Toko");
	gotoxy(x+5, y+14); printf("8. Keluar");
    gotoxy(x+5, y+15); drawline(40);
    gotoxy(x+5, y+16); printf("Pilih Opsi Anda (1-8) : ");
	gotoxy(x+29, y+16);
}

struct AppConfig {
	char name[50];
    char address[50];
};

AppConfig getInputConfig() {
	
	AppConfig c;
	
	gotoxy(x+5, y+20); printf("Masukkan Nama Toko: ");
    scanf("%s", &c.name);
    
    gotoxy(x+5, y+21); printf("Alamat Toko: ");
    scanf("%s", &c.address);
    
    return c;
}
 
void updateKeyValue(KeyValuePair *kvs, int count, const char *targetKey, const char *newValue) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(kvs[i].key, targetKey) == 0) {
            strcpy(kvs[i].value, newValue);
            return;
        }
    }
}

void writeToFile(const char *filename, KeyValuePair *kvs, int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; ++i) {
        fprintf(file, "%s=%s\n", kvs[i].key, kvs[i].value);
    }

    fclose(file);
}

void updateConfigApp() {
	
	gotoxy(x+5, y+18); printf("Edit Profil Toko");
	gotoxy(x+5, y+19); drawline(40);
	
	AppConfig config = getInputConfig();
	
    fileProperties = fopen(filePropertiesName, "r");
    if (fileProperties == NULL) {
        perror("Error opening file properties");
    }

    char line[MAX_LINE_LENGTH];
    KeyValuePair kvArray[MAX_LINE_LENGTH];
    int kvCount = 0;
 
    while (fgets(line, sizeof(line), fileProperties) != NULL) {
        int result = parseLine(line, &kvArray[kvCount]);

        if (result == 1) {
            kvCount++;
        } else if (result == -1) {
            fprintf(stderr, "Error parsing line: %s", line);
        }
    }

    fclose(fileProperties);
    
    // Update the key-value pair with a new value
    updateKeyValue(kvArray, kvCount, "coffee-shop.name", config.name);
    updateKeyValue(kvArray, kvCount, "address", config.address);

    // Write the updated key-value pairs back to the file
    writeToFile(filePropertiesName, kvArray, kvCount);

	gotoxy(x+5, y+25); printf("Config berhasil diupdate");
    gotoxy(x+5, y+26); printf("Tekan ENTER untuk lanjut");
    getch();
}

