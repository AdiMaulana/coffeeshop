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
	char customerName[50];
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
	
    gotoxy(x+5, y+20); printf("Masukkan ID Produk yang Dipesan: ");
    scanf("%d", &t.productId);
    
    gotoxy(x+5, y+21); printf("Nama Pemesan : ");
    scanf("%s", &t.customerName);
    
    gotoxy(x+5, y+22); printf("Jumlah : ");
    scanf("%d", &t.amount);
    
    gotoxy(x+5, y+23); printf("Tanggal Penjualan (dd-mm-yyyy): ");
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
		
		gotoxy(x+5, y+26); printf("Stock Produk %d berhasil diupdate", targetId);	    
	} 
}

void viewProducts() {
	
	gotoxy(x+50, y+5); printf("Jenis Produk");
	gotoxy(x+50, y+6); drawline(60);
	
	int countProducts = 0;
	
	// Open the file in read mode
    listProducts = fopen(fileListProductsName, "r");
    if (listProducts == NULL) {
        perror("Error opening file products");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data
	int i = 1;
    while (fgets(row, sizeof(row), listProducts) != NULL) {
    	
 		// Skip the first row (header)
        if (isFirstRow) {	
        	gotoxy(x+50, y+7); printf("ID");
			gotoxy(x+57, y+7); printf("| Nama Produk");
			gotoxy(x+85, y+7); printf("| Jumlah");
			gotoxy(x+95, y+7); printf("| Harga");
			
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
 		
 		gotoxy(x+50, y+i+7); printf("%d", product.id);
		gotoxy(x+57, y+i+7); printf("| %s ", product.name);
		gotoxy(x+85, y+i+7); printf("| %d ", product.quantity);
		gotoxy(x+95, y+i+7); printf("| %.0f ", product.price);
		
		countProducts++;
		i++;
    }

	if (countProducts == 0) {
		gotoxy(x+50, y+7); printf("Produk Masih Kosong");		
	}  
	
    // Close the file
    fclose(listProducts);
}

void recordTransactionSales() {
	
	gotoxy(x+5, y+18); printf("Penjualan");
	gotoxy(x+5, y+19); drawline(40);
	
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
    	fprintf(fileTransactions, "Product Id|Product Name|Customer Name|Amount|Transaction Date|Total\n");
	}
	
	Products product = findProductById(trx.productId);
	product.quantity = product.quantity - trx.amount;
	
	trx.total = trx.amount * product.price;
	    
	// Write data to the file 
	fprintf(fileTransactions, "%d|%s|%s|%d|%d-%d-%d|%d\n", trx.productId, product.name, trx.customerName, trx.amount, trx.date.dd, trx.date.mm, trx.date.yyyy, trx.total);
	
    // Close the file
    fclose(fileTransactions);
    
    countTrx++;
    
    gotoxy(x+5, y+25); printf("Penjualan berhasil dicatat. Total penjualan: Rp. %d", trx.total);
    
    updateQuantityProductById(product);
    
    viewProducts();
    
    gotoxy(x+5, y+27); printf("Tekan ENTER untuk lanjut ");
    getch();
}

void displayTransactionSales() {
			
	gotoxy(x+48, y+5); printf("Lihat Penjualan (per Tanggal)");
	gotoxy(x+48, y+6); drawline(64);
	
	if (countTrx == 0) {
		gotoxy(x+48, y+7); printf("Transaksi Masih Kosong");		
	} 
	
	// Open the file in read mode
    fileTransactions = fopen(fileTransactionsName, "r");
    if (fileTransactions == NULL) {
        perror("Error opening file transactions");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
	int i = 1;
    while (fgets(row, sizeof(row), fileTransactions) != NULL) {
 
 		// Skip the first row (header)
        if (isFirstRow) {
        	gotoxy(x+48,  y+7); printf("Tanggal");
			gotoxy(x+59,  y+7); printf("%c Nama Pemesan", separator);
			gotoxy(x+78,  y+7); printf("%c Produk", separator);
			gotoxy(x+93,  y+7); printf("%c Jumlah", separator);
			gotoxy(x+102, y+7); printf("%c Nominal", separator);
			 	
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
                    strncpy(trx.customerName, data, sizeof(trx.customerName) - 1);
                    trx.customerName[sizeof(trx.customerName) - 1] = '\0'; // Ensure null-termination
                    break;
                case 3:
					trx.amount = atoi(data);
                    break;
                case 4:
                    strncpy(trxDate, data, sizeof(trxDate));
                    break;
                case 5:
					trx.total = atoi(data);
                    break;
            }

            // Get the next field
            data = strtok(NULL, "|");

            fieldCount++;
        }
 		
 		gotoxy(x+48,  y+i+7); printf("%s", trxDate);
		gotoxy(x+59,  y+i+7); printf("%c %s", separator, trx.customerName);
		gotoxy(x+78,  y+i+7); printf("%c %s", separator, trx.productName);
		gotoxy(x+93,  y+i+7); printf("%c %d", separator, trx.amount);
		gotoxy(x+102, y+i+7); printf("%c %d", separator, trx.total);
	
		i++;		
    }


    // Close the file
    fclose(fileTransactions);
    
    gotoxy(x+48, y+i+8); printf("Tekan ENTER untuk lanjut ");
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
