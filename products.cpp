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
    char name[50];
    int quantity;
    float price;
};

struct Product products[10];
int count = 0;

FILE *fileProducts;
const char fileProductsName[] = "products.csv";
	
Product getInputProduct(bool isNewProduct){
	Product p;
	
	if (isNewProduct) {
		printf("Masukkan ID Produk: ");
    	scanf("%d", &p.id);
	}
	
    printf("Masukkan Nama Produk Baru: ");
    scanf("%s", &p.name);
    
    printf("Masukkan Jumlah Produk: ");
    scanf("%d", &p.quantity);
    
    printf("Masukkan Harga Produk: ");
    scanf("%f", &p.price);
    
    return p;
}

void addProducts() {
	
	printf("\n \t   Tambah Produk");
	printf("\n \t -----------------------------------------\n");
	
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
    
    printf("\nProduk berhasil ditambahkan");
    printf("\nTekan ENTER untuk lanjut ");
    getch();
}

// Function to check if a row should be deleted based on a condition
int isTargetRow(const Product *product, int targetId) {
    return product->id == targetId;
}


void displayProducts(bool holdScreen) {
	
	if (!holdScreen) {
		printHeaderTitle();	
	}
	
	printf("\n \t   Lihat Daftar Produk");
	printf("\n \t -----------------------------------------\n");
	
	if (count == 0) {
		printf("Produk Masih Kosong");	
	}
	
	// Open the file in read mode
    fileProducts = fopen(fileProductsName, "r");
    if (fileProducts == NULL) {
        perror("Error opening file products");
    }
    
    printf(" ID \t | Nama \t\t | Jumlah \t | Harga \t\n");

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
    while (fgets(row, sizeof(row), fileProducts) != NULL) {
 
 		// Skip the first row (header)
        if (isFirstRow) {
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
 		
 		printf(" %d \t | %s \t\t | %d \t\t | %.0f \t\n", product.id, product.name, product.quantity, product.price);
    }


    // Close the file
    fclose(fileProducts);
    
    if (holdScreen) {
    	printf("\nTekan ENTER untuk lanjut ");
		getch();	
	}
}

void updateProducts() {
	
	displayProducts(0);
	
	printf("\n \t   Edit Produk");
	printf("\n \t -----------------------------------------\n");
	
	int targetId;
    printf("\nMasukkan ID Produk yang akan diupdate: ");
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
	
	    printf("\nID Produk %d berhasil diupdate", targetId);
	    printf("\nTekan ENTER untuk lanjut ");
    	
    } else {
        printf("\nID Produk tidak valid.");
    }
    getch();
}

void deleteProducts() {
	
	displayProducts(0);
	
	printf("\n \t   Hapus Produk");
	printf("\n \t -----------------------------------------\n");
	
	FILE *tempFile; 
    char tempFilename[] = "temp_products.csv"; 
    
    int targetId; // ID of the row to delete
    printf("\nMasukkan ID Produk yang akan dihapus: ");
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

    printf("\nID Produk %d berhasil dihapus", targetId);
    printf("\nTekan ENTER untuk lanjut ");
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
