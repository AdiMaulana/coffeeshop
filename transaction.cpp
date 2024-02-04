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

struct Filter {
	Date startDate;
	Date endDate;
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
    fflush(stdin);  
    fgets(t.customerName, sizeof(t.customerName), stdin); 
    t.customerName[strcspn(t.customerName, "\n")] = '\0';
    
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

void clearTableScreen2() {
	for (int i=1; i<height-4; i++) {
		for (int j=1; j<width-50; j++) {
			gotoxy(x+j+50, y+i+6); printf(" ");		
		}
	}	
}

void viewProducts() {
	
	gotoxy(x+50, y+5); printf("Jenis Produk");
	gotoxy(x+50, y+6); drawline(60);
    
    clearTableScreen2();
	
	// Open the file in read mode
    listProducts = fopen(fileListProductsName, "r");
    if (fileProducts == NULL) {
        perror("Error opening file products");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
    int rowCount = 0;   
    while (fgets(row, sizeof(row), listProducts) != NULL) {
    	    	
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
    
    
	if (rowCount == 0) {
		gotoxy(x+50, y+7); printf("Produk Masih Kosong");		
		
	} else {
	    gotoxy(x+50, y+rowCount+9); drawline(60);
	    
	    for (int i = 0; i<count+3; i++) {
	    	gotoxy(x+110, y+i+6); printf("%c", separator);
		}
		
		int cord_x[5] = {50, 57, 85, 95, 110};
		for (int i=0; i<4;i+=2) {
		    for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
		        gotoxy(x+ cord_x[j], y+i+6); printf("+");
		    }
		}
		 
		for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
		    gotoxy(x+ cord_x[j], y+count+9); printf("+");
		}
	}  
	
    // Close the file
    fclose(listProducts); 
}

void recordTransactionSales() {
	
	gotoxy(x+5, y+18); printf("Penjualan");
	gotoxy(x+5, y+19); drawline(35);
	
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
			
	gotoxy(x+48, y+5); printf("Lihat Penjualan");
	gotoxy(x+45, y+7); drawline(67);
	
	if (countTrx == 0) {
		gotoxy(x+45, y+8); printf("Transaksi Masih Kosong");		
	} 
	
	// Open the file in read mode
    fileTransactions = fopen(fileTransactionsName, "r");
    if (fileTransactions == NULL) {
        gotoxy(x+45, y+9); perror("Error opening file transactions");
    }

    char row[MAX_ROW_LENGTH];
    int isFirstRow = 1;
    
	// Read and loop through rows of data    
	int rowCount = 0;
    while (fgets(row, sizeof(row), fileTransactions) != NULL) {
 
 		// Skip the first row (header)
        if (isFirstRow) {
        	gotoxy(x+45,  y+8); printf("%c Tanggal", separator);
			gotoxy(x+58,  y+8); printf("%c Nama Pemesan", separator);
			gotoxy(x+75,  y+8); printf("%c Produk", separator);
			gotoxy(x+92,  y+8); printf("%c Jumlah", separator);
			gotoxy(x+102, y+8); printf("%c Nominal", separator);
			gotoxy(x+45, y+9); drawline(67);
			
            isFirstRow = 0;
            continue;
        }
        
        if (rowCount == 15) {
        	break;
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
 		
 		gotoxy(x+45,  y+rowCount+10); printf("%c %s", separator, trxDate);
		gotoxy(x+58,  y+rowCount+10); printf("%c %s", separator, trx.customerName);
		gotoxy(x+75,  y+rowCount+10); printf("%c %s", separator, trx.productName);
		gotoxy(x+92,  y+rowCount+10); printf("%c %d", separator, trx.amount);
		gotoxy(x+102, y+rowCount+10); printf("%c %d", separator, trx.total);
	
		rowCount++;		
    }

	gotoxy(x+46, y+rowCount+10); drawline(67);
    
    for (int i = 0; i<rowCount+4; i++) {
    	gotoxy(x+112, y+i+7); printf("%c", separator);
	}
	
	int cord_x[6] = {45, 58, 75, 92, 102, 112};
	for (int i=0; i<4;i+=2) {
	    for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
	        gotoxy(x+ cord_x[j], y+i+7); printf("+");
	    }
	}
	 
	for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
	    gotoxy(x+ cord_x[j], y+rowCount+10); printf("+");
	}
	
    // Close the file
    fclose(fileTransactions);
    
    gotoxy(x+74, y+5); printf("Tanggal Awal (dd-mm-yyyy) : ");
	gotoxy(x+74, y+6); printf("Tanggal Akhir(dd-mm-yyyy) : ");
	
    Filter f;
    gotoxy(x+102, y+5); scanf("%d-%d-%d", &f.startDate.dd, &f.startDate.mm, &f.startDate.yyyy);
    gotoxy(x+102, y+6); scanf("%d-%d-%d", &f.endDate.dd, &f.endDate.mm, &f.endDate.yyyy);
    
    gotoxy(x+45, y+rowCount+11); printf("Tekan ENTER untuk lanjut ");
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
