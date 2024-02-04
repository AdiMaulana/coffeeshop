#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#include "menu.cpp"

#define MAX_ROW_LENGTH 1024
#define MAX_FIELDS 4

struct Product {
	int id;
    char name[30];
    int quantity;
    float price;
};

int count = 0;

FILE *fileProducts;
const char fileProductsName[] = "products.csv";
	
void clearInput(int length, int x, int y) {
	for(int i=0; i<length; i++) {
		 gotoxy(x+i, y); printf(" ");
	}	
}

const char errMessageInvalidId[] = "(ID Produk Tidak Valid, Masukkan Angka Numerik)";
const char errMessageMaxName[] = "(Nama Produk Maksimal 30 Karakter)";
const char errMessageInvalidQuantity[] = "(Jumlah Produk Tidak Valid, Masukkan Angka Numerik)";
const char errMessageInvalidPrice[] = "(Harga Produk Tidak Valid, Masukkan Angka Numerik)";

Product getInputProduct(bool isNewProduct){
	Product p;
	
	char inputTemp[100];
	int length;
	
	if (isNewProduct) {
		do {
			gotoxy(x+5, y+20); printf("ID Produk (Numerik) : ");
    		
    		fgets(inputTemp, sizeof(inputTemp), stdin);
 
	        if (sscanf(inputTemp, "%d", &p.id) == 1) {
	        
				clearInput(strlen(errMessageInvalidId), x+5, y+21);	
	        	
	            break; // Exit the loop when a numeric input is provided
	            
	        } else {  
	    		inputTemp[strcspn(inputTemp, "\n")] = '\0';
	
	    		length = strlen(inputTemp);
	    		
	    		clearInput(length, x+27, y+20);		
	    
	            gotoxy(x+5, y+21); printf(errMessageInvalidId);
	        }
	        
		} while(1);	
	}
	
	do {
		gotoxy(x+5, y+21); printf("Nama Produk Baru: "); 
		fflush(stdin);  
	    
		fgets(inputTemp, sizeof(inputTemp), stdin); 
	    
	    inputTemp[strcspn(inputTemp, "\n")] = '\0';
				
		if (strlen(inputTemp) > 30) {
			
			clearInput(strlen(inputTemp), x+23, y+21);		
    
            gotoxy(x+5, y+22); printf(errMessageMaxName);
            
		} else { 
			clearInput(strlen(errMessageMaxName), x+5, y+22);	
			
			strcpy(p.name, inputTemp);
    		break;
		}
	} while (1);
    
	do {
        gotoxy(x+5, y+22); printf("Jumlah Produk: ");
        
        fgets(inputTemp, sizeof(inputTemp), stdin);
 
        if (sscanf(inputTemp, "%d", &p.quantity) == 1) {
        
			clearInput(strlen(errMessageInvalidQuantity), x+5, y+23);	
        	
            break; // Exit the loop when a numeric input is provided
            
        } else {  
    		inputTemp[strcspn(inputTemp, "\n")] = '\0';

    		length = strlen(inputTemp);
    		
    		clearInput(length, x+20, y+22);		
    
            gotoxy(x+5, y+23); printf(errMessageInvalidQuantity);
        }
    } while (1);
    
    do {
        gotoxy(x+5, y+23); printf("Harga Produk: ");
        
        fgets(inputTemp, sizeof(inputTemp), stdin);
 
        if (sscanf(inputTemp, "%f", &p.price) == 1) {
        	
        	clearInput(strlen(errMessageInvalidPrice), x+5, y+24);	
        	
            break; // Exit the loop when a numeric input is provided
            
        } else {	 
    		inputTemp[strcspn(inputTemp, "\n")] = '\0';

    		length = strlen(inputTemp);
    		
    		clearInput(length, x+19, y+23);		
    
            gotoxy(x+5, y+24); printf(errMessageInvalidPrice);
        }
    } while (1);
    
    return p;
}

void clearTableScreen() {
	for (int i=1; i<height-4; i++) {
		for (int j=1; j<width-50; j++) {
			gotoxy(x+j+50, y+i+6); printf(" ");		
		}
	}	
}

void displayProducts(bool holdScreen) {
	
	clearTableScreen();
		
	gotoxy(x+50, y+5); printf("Lihat Daftar Produk");
	gotoxy(x+50, y+6); drawline(60);
		
	if (count == 0) {
		gotoxy(x+50, y+7); printf("Produk Masih Kosong");		
	} 
	
	// Open the file in read mode
    fileProducts = fopen(fileProductsName, "r");
    if (fileProducts == NULL) {
        perror("Error opening file products");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
    int rowCount = 0;   
    while (fgets(row, sizeof(row), fileProducts) != NULL) {
    	    	
 		// Skip the first row (header)
        if (isFirstRow) {
        	gotoxy(x+50, y+7); printf("%c ID", separator);
			gotoxy(x+57, y+7); printf("%c Nama Produk", separator);
			gotoxy(x+85, y+7); printf("%c Jumlah", separator);
			gotoxy(x+95, y+7); printf("%c Harga", separator);
        	gotoxy(x+50, y+8); drawline(60);
        	
            isFirstRow = 0;
            continue;
        }
        
        // Tokenize the line using strtok based on the pipe character '|'
        char *data = strtok(row, "|");
 
        Product product;
 
        int fieldCount = 0;
        while (data != NULL) {
            switch (fieldCount) {
                case 0:
                    product.id = atoi(data);
                    break;
                case 1:
                    strncpy(product.name, data, sizeof(product.name) - 1);
                    product.name[sizeof(product.name) - 1] = '\0'; // Ensure null-termination
                    break;
                case 2:
                    product.quantity = atoi(data);
                    break;
                case 3:
                    product.price = atoi(data);
                    break;
            }

            // Get the next field
            data = strtok(NULL, "|");

            fieldCount++;
        }
 		
 		gotoxy(x+50, y+rowCount+9); printf("%c %d", separator, product.id);
		gotoxy(x+57, y+rowCount+9); printf("%c %s ", separator, product.name);
		gotoxy(x+85, y+rowCount+9); printf("%c %d ", separator, product.quantity);
		gotoxy(x+95, y+rowCount+9); printf("%c %.0f ", separator, product.price);
		
    	rowCount++;
    }
    
    gotoxy(x+50, y+rowCount+9); drawline(60);
    
    for (int i = 0; i<rowCount+3; i++) {
    	gotoxy(x+110, y+i+6); printf("%c", separator);
	}
	
	int cord_x[5] = {50, 57, 85, 95, 110};
	for (int i=0; i<4;i+=2) {
	    for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
	        gotoxy(x+ cord_x[j], y+i+6); printf("+");
	    }
	    for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
	    	gotoxy(x+ cord_x[j], y+rowCount+9); printf("+");
		}
	}
	 
    // Close the file
    fclose(fileProducts);
    
    if (holdScreen) {
    	gotoxy(x+50, y+rowCount+10); printf("Tekan ENTER untuk lanjut ");
		getch();	
	}
}

void addProducts() {
	
	displayProducts(0);
	
	gotoxy(x+5, y+18); printf("Tambah Data Produk");
	gotoxy(x+5, y+19); drawline(35);
	
	Product product = getInputProduct(true) ;
	
	// Check if the file exists
    if (access(fileProductsName, F_OK) != -1) {
    	// Open the file in append mode ("a")
    	fileProducts = fopen(fileProductsName, "a");
    	 if (fileProducts == NULL) {
	        perror("Error opening file products for writing ");
	    }
	} else {
		fileProducts = fopen(fileProductsName, "w");
	    if (fileProducts == NULL) {
	        perror("Could not open the file products");
	    }	
	}
    
    if (count == 0) {
    	fprintf(fileProducts, "Id|Name|Quantity|Price\n");
	}
	    
	// Write data to the file 
	fprintf(fileProducts, "%d|%s|%d|%.0f\n", product.id, product.name, product.quantity, product.price);
	
    // Close the file
    fclose(fileProducts);
    
    count++;
    
    displayProducts(0);
    
    gotoxy(x+5, y+25); printf("Produk berhasil ditambahkan");
    gotoxy(x+5, y+26); printf("Tekan ENTER untuk lanjut");
    getch();
}

// Function to check if a row should be deleted based on a condition
int isTargetRow(const Product *product, int targetId) {
    return product->id == targetId;
}

void updateProducts() {
	
	displayProducts(0);
	
	if (count != 0) {
		
		gotoxy(x+5, y+18); printf("Update Data Produk");
		gotoxy(x+5, y+19); drawline(35);
		 
		int targetId;
	    gotoxy(x+5, y+20); printf("Masukkan ID Produk yang akan diupdate: ");
	    scanf("%d", &targetId);
	    
	    if (targetId > 0 ) {
	    	
	        Product newProduct = getInputProduct(false);
	    	
		    FILE *tempFile; 
		    char tempFilename[] = "temp_products.csv"; 
		      
		    // Open the original file in read mode
		    fileProducts = fopen(fileProductsName, "r"); 
		    if (fileProducts == NULL) {
		        perror("Error opening file products"); 
		    }
		
		    // Open a temporary file in write mode
		    tempFile = fopen(tempFilename, "w");
		    if (tempFile == NULL) {
		        perror("Error opening temporary file");
		        fclose(fileProducts); 
		    }
		
		    // Read and process rows of data
		    char row[MAX_ROW_LENGTH];
		    int isFirstRow = 1;
		    
			// Read and loop through rows of data    
		    while (fgets(row, sizeof(row), fileProducts) != NULL) {
		    	
		 		// Skip the first row (header)
		        if (isFirstRow) {
		        	fprintf(tempFile, "Id|Name|Quantity|Price\n");
		            isFirstRow = 0;
		            continue;
				}
				 
		        char *data = strtok(row, "|");
		 
		        Product product;
		
		        // Parse and process each field
		        int fieldCount = 0;
		        while (data != NULL && fieldCount < MAX_FIELDS) {
		            switch (fieldCount) {
		                case 0:
		                    product.id = atoi(data);
		                    break;
		                case 1:
		                    strncpy(product.name, data, sizeof(product.name) - 1);
		                    product.name[sizeof(product.name) - 1] = '\0'; // Ensure null-termination
		                    break;
		                case 2:
		                    product.quantity = atoi(data);
		                    break;
		                case 3:
		                    product.price = atoi(data);
		                    break;
		            }
		
		            // Get the next token
		            data = strtok(NULL, "|");
		
		            fieldCount++;
		        }
		 
		        if (isTargetRow(&product, targetId)) { 
		            fprintf(tempFile, "%d|%s|%d|%.0f\n", targetId, newProduct.name, newProduct.quantity, newProduct.price);
		            
		        } else {
		        	fprintf(tempFile, "%d|%s|%d|%.0f\n", product.id, product.name, product.quantity, product.price);
				}
		    }
		
		    // Close both files
		    fclose(fileProducts);
		    fclose(tempFile);
		
		    // Delete the original file
		    if (remove(fileProductsName) != 0) {
		        perror("Error deleting original file"); 
		    }
		    
		    // Rename the temporary file to overwrite the original file
		    if (rename(tempFilename, fileProductsName) != 0) {
		        perror("Error renaming file"); 
		    }
		
		    gotoxy(x+5, y+25); printf("ID Produk %d berhasil diupdate", targetId);
		    
	    } else {
	        gotoxy(x+5, y+25); printf("ID Produk tidak valid.");
	    }
	}
	
	displayProducts(0);
	
	gotoxy(x+5, y+26); printf("Tekan ENTER untuk lanjut ");
    getch();
}

void deleteProducts() {
	
	displayProducts(0);
	
	if (count != 0) {
		
		gotoxy(x+5, y+18); printf("Hapus Produk");
		gotoxy(x+5, y+19); drawline(35);
			
		FILE *tempFile; 
	    char tempFilename[] = "temp_products.csv"; 
	    
	    int targetId; // ID of the row to delete
	    gotoxy(x+5, y+20); printf("Masukkan ID Produk yang akan dihapus: ");
	    scanf("%d", &targetId);
	
	    // Open the original file in read mode
	    fileProducts = fopen(fileProductsName, "r"); 
	    if (fileProducts == NULL) {
	        perror("Error opening file products"); 
	    }
	
	    // Open a temporary file in write mode
	    tempFile = fopen(tempFilename, "w");
	    if (tempFile == NULL) {
	        perror("Error opening temporary file");
	        fclose(fileProducts); 
	    }
	
	    // Read and process rows of data
	    char row[MAX_ROW_LENGTH];
	    int isFirstRow = 1;
	    
		// Read and loop through rows of data    
	    while (fgets(row, sizeof(row), fileProducts) != NULL) {
	    	
	 		// Skip the first row (header)
	        if (isFirstRow) {
	        	fprintf(tempFile, "Id|Name|Quantity|Price\n");
	            isFirstRow = 0;
	            continue;
			}
			 
	        char *data = strtok(row, "|");
	 
	        Product product;
	
	        // Parse and process each field
	        int fieldCount = 0;
	        while (data != NULL && fieldCount < MAX_FIELDS) {
	            switch (fieldCount) {
	                case 0:
	                    product.id = atoi(data);
	                    break;
	                case 1:
	                    strncpy(product.name, data, sizeof(product.name) - 1);
	                    product.name[sizeof(product.name) - 1] = '\0'; // Ensure null-termination
	                    break;
	                case 2:
	                    product.quantity = atoi(data);
	                    break;
	                case 3:
	                    product.price = atoi(data);
	                    break;
	            }
	
	            // Get the next token
	            data = strtok(NULL, "|");
	
	            fieldCount++;
	        }
	 
	        if (!isTargetRow(&product, targetId)) { 
	            fprintf(tempFile, "%d|%s|%d|%.0f\n", product.id, product.name, product.quantity, product.price);
	        } else {
	        	count--;
			}
	    }
	
	    // Close both files
	    fclose(fileProducts);
	    fclose(tempFile);
	
	    // Delete the original file
	    if (remove(fileProductsName) != 0) {
	        perror("Error deleting original file"); 
	    }
	    
	    // Rename the temporary file to overwrite the original file
	    if (rename(tempFilename, fileProductsName) != 0) {
	        perror("Error renaming file"); 
	    }
		
	    gotoxy(x+5, y+25); printf("ID Produk %d berhasil dihapus", targetId);
	}
	
	displayProducts(0);
	
    gotoxy(x+5, y+26); printf("Tekan ENTER untuk lanjut ");
    getch();
}

void initProducts() {
	// Open the file in read mode
    fileProducts = fopen(fileProductsName, "r");
    if (fileProducts == NULL) {
        perror("Error opening file products");
    }
    
    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
    while (fgets(row, sizeof(row), fileProducts) != NULL) {
 		// Skip the first row (header)
        if (isFirstRow) {
            isFirstRow = 0;
            continue;
        }
 		
 		count++;
    }
    
    // Close the file
    fclose(fileProducts); 
}

