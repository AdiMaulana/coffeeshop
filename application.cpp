//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#include "menu.cpp"
//
//#define MAX_LINE_LENGTH2 1024
//
//typedef struct {
//    char key[MAX_LINE_LENGTH2];
//    char value[MAX_LINE_LENGTH2];
//} KeyValue;
//
//struct Config {
//	char name[50];
//    char address[50];
//};
//
//Config getInputConfig() {
//	
//	Config c;
//	
//	gotoxy(x+5, y+20); printf("Masukkan Nama Toko: ");
//    scanf("%s", &c.name);
//    
//    gotoxy(x+5, y+21); printf("Alamat Toko: ");
//    scanf("%s", &c.address);
//    
//    return c;
//}
// 
//int parseLine(char *line, KeyValue *kv) { 
//
//    if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
//        return 0; 
//    }
// 
//    char *equalSign = strchr(line, '=');
//    if (!equalSign) {
//        return -1;
//    }
//
//    // Extract key and value
//    strncpy(kv->key, line, equalSign - line);
//    kv->key[equalSign - line] = '\0'; // Null-terminate the key
//
//    strcpy(kv->value, equalSign + 1);
//    strtok(kv->value, "\n"); // Remove newline characters from the value
//
//    return 1; // Success
//}
// 
//void updateKeyValue(KeyValue *kvs, int count, const char *targetKey, const char *newValue) {
//    for (int i = 0; i < count; ++i) {
//        if (strcmp(kvs[i].key, targetKey) == 0) {
//            strcpy(kvs[i].value, newValue);
//            return;
//        }
//    }
//}
//
//void writeToFile(const char *filename, KeyValue *kvs, int count) {
//    FILE *file = fopen(filename, "w");
//    if (!file) {
//        perror("Error opening file for writing");
//        exit(EXIT_FAILURE);
//    }
//
//    for (int i = 0; i < count; ++i) {
//        fprintf(file, "%s=%s\n", kvs[i].key, kvs[i].value);
//    }
//
//    fclose(file);
//}
//
//void updateConfigApp() {
//	
//	gotoxy(x+5, y+18); printf("Edit Profil Toko");
//	gotoxy(x+5, y+19); drawline(40);
//	
//	Config config = getInputConfig();
//	
//    FILE *file = fopen("application.properties", "r");
//    if (!file) {
//        perror("Error opening file");
//    }
//
//    char line[MAX_LINE_LENGTH];
//    KeyValue kvArray[MAX_LINE_LENGTH];
//    int kvCount = 0;
// 
//    while (fgets(line, sizeof(line), file) != NULL) {
//        int result = parseLine(line, &kvArray[kvCount]);
//
//        if (result == 1) {
//            kvCount++;
//        } else if (result == -1) {
//            fprintf(stderr, "Error parsing line: %s", line);
//        }
//    }
//
//    fclose(file);
//
//    // Update the key-value pair with a new value
//    updateKeyValue(kvArray, kvCount, "coffee-shop.name", config.name);
//    updateKeyValue(kvArray, kvCount, "address", config.address);
//
//    // Write the updated key-value pairs back to the file
//    writeToFile("application.properties", kvArray, kvCount);
//
//	gotoxy(x+5, y+25); printf("Config berhasil diupdate");
//    gotoxy(x+5, y+26); printf("Tekan ENTER untuk lanjut");
//    getch();
//}

