#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define MAX_ROW_LENGTH 1024
#define MAX_FIELDS 4

struct Date {
	int dd;
	int mm;
	int yyyy;
}; 

struct Transaction {
	int productId;
	char productName[50];
	int amount;
	Date date;
    int total;
};

struct Products {
	int id;
    char name[50];
    int quantity;
    float price;
};

int countTrx = 0;

FILE *fileTransactions, *listProducts;
const char fileTransactionsName[] = "transactions.csv";
const char fileListProductsName[] = "products.csv";
	
Transaction getInputTransaction(){
	Transaction t;
	
    printf("\nMasukkan ID Produk yang Terjual: ");
    scanf("%d", &t.productId);
    
    printf("Jumlah Produk Terjual: ");
    scanf("%d", &t.amount);
    
    printf("Tanggal Penjualan (dd-mm-yyyy): ");
    scanf("%d-%d-%d", &t.date.dd, &t.date.mm, &t.date.yyyy);
    
    return t;
}

Products findProductById(int id) {
	
	// Open the file in read mode
    listProducts = fopen(fileListProductsName, "r");
    if (listProducts == NULL) {
        perror("Error opening file products");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
    Products product;
     
	// Read and loop through rows of data    
    while (fgets(row, sizeof(row), listProducts) != NULL) {
 
 		// Skip the first row (header)
        if (isFirstRow) {
            isFirstRow = 0;
            continue;
        }
        
        // Tokenize the line using strtok based on the pipe character '|'
        char *data = strtok(row, "|");
 
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
 		
 		if (product.id == id) {
 			break;
		}
    }

    // Close the file
    fclose(listProducts);
     
    return product; 
}


int isUpdateRow(const Products *product, int targetId) {
    return product->id == targetId;
}

void updateQuantityProductById(Products p) {
		
	int targetId = p.id;
	int quantity = p.quantity;
	    
	if (targetId > 0 ) { 
	
		FILE *tempFile; 
		char tempFilename[] = "temp_products.csv"; 
		      
		// Open the original file in read mode
		listProducts = fopen(fileListProductsName, "r"); 
		if (listProducts == NULL) {
			perror("Error opening file products"); 
		}
		
		// Open a temporary file in write mode
		tempFile = fopen(tempFilename, "w");
		if (tempFile == NULL) {
		    perror("Error opening temporary file");
		    fclose(listProducts); 
		}
		
		// Read and process rows of data
		char row[MAX_ROW_LENGTH];
		int isFirstRow = 1;
		    
		// Read and loop through rows of data    
		while (fgets(row, sizeof(row), listProducts) != NULL) {
		    	
		 	// Skip the first row (header)
		    if (isFirstRow) {
		        fprintf(tempFile, "Id|Name|Quantity|Price\n");
		        isFirstRow = 0;
		        continue;
			}
				 
		    char *data = strtok(row, "|");
		 
		    Products product;
		
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
		 
		    if (isUpdateRow(&product, targetId)) { 
		        fprintf(tempFile, "%d|%s|%d|%.0f\n", targetId, product.name, quantity, product.price);
		            
		    } else {
		        fprintf(tempFile, "%d|%s|%d|%.0f\n", product.id, product.name, product.quantity, product.price);
			}
		}
		
		// Close both files
		fclose(listProducts);
		fclose(tempFile);
		
		// Delete the original file
		if (remove(fileListProductsName) != 0) {
		    perror("Error deleting original file"); 
		}
		    
		// Rename the temporary file to overwrite the original file
		if (rename(tempFilename, fileListProductsName) != 0) {
		    perror("Error renaming file"); 
		}
		
		printf("\nStock Produk %d berhasil diupdate", targetId);	    
	} 
}

void viewProducts() {
	
	printf("\n \t   Jenis Produk");
	printf("\n \t -----------------------------------------\n");
	
	int countProducts = 0;
	
	// Open the file in read mode
    listProducts = fopen(fileListProductsName, "r");
    if (listProducts == NULL) {
        perror("Error opening file products");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
    while (fgets(row, sizeof(row), listProducts) != NULL) {
    	
 		// Skip the first row (header)
        if (isFirstRow) {	
        	printf(" ID \t | Nama \t\t | Jumlah \t | Harga \t\n");
            isFirstRow = 0;
            continue;
        }
        
        // Tokenize the line using strtok based on the pipe character '|'
        char *data = strtok(row, "|");
 
        Products product;
 
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
 		
 		countProducts++;
    }

	if (countProducts == 0) {
		printf("Produk Masih Kosong");		
	}  
	
    // Close the file
    fclose(listProducts);
}

void recordTransactionSales() {
	
	printf("\n \t   Penjualan");
	printf("\n \t -----------------------------------------");
	
	viewProducts();
	
	Transaction trx = getInputTransaction();
	
	// Check if the file exists
    if (access(fileTransactionsName, F_OK) != -1) {
    	// Open the file in append mode ("a")
    	fileTransactions = fopen(fileTransactionsName, "a");
    	 if (fileTransactions == NULL) {
	        perror("Error opening file transactions for writing ");
	    }
	} else {
		fileTransactions = fopen(fileTransactionsName, "w");
	    if (fileTransactions == NULL) {
	        perror("Could not open the file transactions");
	    }	
	}
    
    if (countTrx == 0) {
    	fprintf(fileTransactions, "Product Id|Product Name|Amount|Transaction Date|Total\n");
	}
	
	Products product = findProductById(trx.productId);
	product.quantity = product.quantity - trx.amount;
	
	trx.total = trx.amount * product.price;
	    
	// Write data to the file 
	fprintf(fileTransactions, "%d|%s|%d|%d-%d-%d|%d\n", trx.productId, product.name, trx.amount, trx.date.dd, trx.date.mm, trx.date.yyyy, trx.total);
	
    // Close the file
    fclose(fileTransactions);
    
    countTrx++;
    
    printf("\nPenjualan berhasil dicatat. Total penjualan: Rp. %d", trx.total);
    
    updateQuantityProductById(product);
    
    printf("\nTekan ENTER untuk lanjut ");
    getch();
}

void displayTransactionSales() {
		
	printf("\n \t   Lihat Penjualan (per Tanggal)");
	printf("\n \t -----------------------------------------\n");
	
	if (countTrx == 0) {
		printf("Transaksi Masih Kosong");		
	} 
	
	// Open the file in read mode
    fileTransactions = fopen(fileTransactionsName, "r");
    if (fileTransactions == NULL) {
        perror("Error opening file transactions");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
    while (fgets(row, sizeof(row), fileTransactions) != NULL) {
 
 		// Skip the first row (header)
        if (isFirstRow) {
        	printf("Id Produk | Nama Produk  | Jumlah \t| Tanggal \t | Total \t\n");
        	
            isFirstRow = 0;
            continue;
        }
        
        // Tokenize the line using strtok based on the pipe character '|'
        char *data = strtok(row, "|");
 
        Transaction trx;
        
        char trxDate[12];
 
        int fieldCount = 0;
        while (data != NULL) {
            switch (fieldCount) {
                case 0:
                    trx.productId = atoi(data);
                    break;
                case 1:
                    strncpy(trx.productName, data, sizeof(trx.productName) - 1);
                    trx.productName[sizeof(trx.productName) - 1] = '\0'; // Ensure null-termination
                    break;
                case 2:
					trx.amount = atoi(data);
                    break;
                case 3:
                    strncpy(trxDate, data, sizeof(trxDate));
                    break;
                case 4:
					trx.total = atoi(data);
                    break;
            }

            // Get the next field
            data = strtok(NULL, "|");

            fieldCount++;
        }
 		
 		printf(" %d  \t  | %s \t | %d \t\t| %s \t | %d \t\n", trx.productId, trx.productName, trx.amount, trxDate, trx.total);
    }


    // Close the file
    fclose(fileTransactions);
    
    printf("\nTekan ENTER untuk lanjut ");
	getch();	
}

void initTransactions() {
	// Open the file in read mode
    fileTransactions = fopen(fileTransactionsName, "r");
    if (fileTransactions == NULL) {
        perror("Error opening file transactions");
    }
    
    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
    while (fgets(row, sizeof(row), fileTransactions) != NULL) {
 		// Skip the first row (header)
        if (isFirstRow) {
            isFirstRow = 0;
            continue;
        }
 		
 		countTrx++;
    }
    
    // Close the file
    fclose(fileTransactions); 
}
