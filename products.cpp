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

int count = 0;

FILE *fileProducts;
const char fileProductsName[] = "products.csv";
	
Product getInputProduct(bool isNewProduct){
	Product p;
	
	if (isNewProduct) {
		gotoxy(x+5, y+20); printf("ID Produk: ");
    	scanf("%d", &p.id);
	}
	
    gotoxy(x+5, y+21); printf("Nama Produk Baru: ");
    scanf("%s", &p.name);
    
    gotoxy(x+5, y+22); printf("Jumlah Produk: ");
    scanf("%d", &p.quantity);
    
    gotoxy(x+5, y+23); printf("Harga Produk: ");
    scanf("%f", &p.price);
    
    return p;
}

void displayProducts(bool holdScreen) {
		
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
    int i = 1;   
    while (fgets(row, sizeof(row), fileProducts) != NULL) {
    	
 		// Skip the first row (header)
        if (isFirstRow) {
        	gotoxy(x+50, y+7); printf("ID");
			gotoxy(x+57, y+7); printf("%c Nama Produk", separator);
			gotoxy(x+85, y+7); printf("%c Jumlah", separator);
			gotoxy(x+95, y+7); printf("%c Harga", separator);
        	
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
 		
 		gotoxy(x+50, y+i+7); printf("%d", product.id);
		gotoxy(x+57, y+i+7); printf("%c %s ", separator, product.name);
		gotoxy(x+85, y+i+7); printf("%c %d ", separator, product.quantity);
		gotoxy(x+95, y+i+7); printf("%c %.0f ", separator, product.price);
		
		i++;
    }

    // Close the file
    fclose(fileProducts);
    
    if (holdScreen) {
    	gotoxy(x+50, y+i+8); printf("Tekan ENTER untuk lanjut ");
		getch();	
	}
}

void addProducts() {
	
	displayProducts(0);
	
	gotoxy(x+5, y+18); printf("Tambah Data Produk");
	gotoxy(x+5, y+19); drawline(40);
	
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
		gotoxy(x+5, y+19); drawline(40);
		 
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
		gotoxy(x+5, y+19); drawline(40);
			
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

