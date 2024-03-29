#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define MAX_ROW_LENGTH 1024
#define MAX_FIELDS 4
#define MAX_ROWS_PER_PAGE 14 // Maximum number of rows per page

struct Date {
	int dd;
	int mm;
	int yyyy;
}; 

// Function to compare two dates
// Returns:
//   -1 if date1 is earlier than date2
//    0 if date1 is equal to date2
//    1 if date1 is later than date2
int compareDates(struct Date date1, struct Date date2) {
    if (date1.yyyy < date2.yyyy) {
        return -1;
    } else if (date1.yyyy > date2.yyyy) {
        return 1;
    } else {
        if (date1.mm < date2.mm) {
            return -1;
        } else if (date1.mm > date2.mm) {
            return 1;
        } else {
            if (date1.dd < date2.dd) {
                return -1;
            } else if (date1.dd > date2.dd) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

// Function to check if a given year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to validate a date
bool isValidDate(struct Date date) {
    if (date.yyyy < 1) {
        return false;
    }

    if (date.mm < 1 || date.mm > 12) {
        return false;
    }

    // Check if the day is valid based on the month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Adjust February days for leap years
    if (date.mm == 2 && isLeapYear(date.yyyy)) {
        daysInMonth[2] = 29;
    }

    if (date.dd < 1 || date.dd > daysInMonth[date.mm]) {
        return false;
    }
    
    return true;
}

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

const char errMessageInvalidProductId[] = "(ID Produk Tidak Valid, Masukkan Angka Numerik)";	
const char errMessageEmptyCustomerName[] = "(Nama Pemesan Harus Diisi)";
const char errMessageMaxCustomerName[] = "(Nama Pemesan Maksimal 50 Karakter)";
const char errMessageInvalidAmount[] = "(Jumlah Tidak Valid, Masukkan Angka Numerik)";
const char errMessageInvalidDate[] = "(Tanggal Tidak Valid, Masukkan Tanggal Sesuai Format)";
const char errMessageInvalidFormatDate[] = "(Format Tanggal Tidak Valid)";

Transaction getInputTransaction(){
	Transaction t;
	
	char inputTemp[100];
	int length;
	
	int i = 1;   
    do {
		gotoxy(x+5, y+20); printf("Masukkan ID Produk yang Dipesan: ");
    	
    	fgets(inputTemp, sizeof(inputTemp), stdin);
 
        if (sscanf(inputTemp, "%d", &t.productId) == 1) {
	        
			clearInput(strlen(errMessageInvalidProductId), x+5, y+21);	
	        	
	        break; // Exit the loop when a numeric input is provided
	            
	    } else {  
	   		inputTemp[strcspn(inputTemp, "\n")] = '\0';
	
	  		length = strlen(inputTemp);
	    		
	   		clearInput(length, x+38, y+20);		
	   		
	   		if (i != 1) {
	   			gotoxy(x+5, y+21); printf(errMessageInvalidProductId);	
			}
	    }  
		i++;  
	} while(1);	
		
    do {
		gotoxy(x+5, y+21); printf("Nama Pemesan : ");
		
		fflush(stdin);  
	    
		fgets(inputTemp, sizeof(inputTemp), stdin); 
	    
	    inputTemp[strcspn(inputTemp, "\n")] = '\0';
				
		if (strlen(inputTemp) > 50) {
			
			clearInput(strlen(inputTemp), x+20, y+21);		
    
            gotoxy(x+5, y+22); printf(errMessageMaxCustomerName);
            
		} else if (strlen(inputTemp) < 1) {
			
			clearInput(strlen(inputTemp), x+20, y+21);		
    
            gotoxy(x+5, y+22); printf(errMessageEmptyCustomerName);
            
		} else { 
			clearInput(strlen(errMessageMaxCustomerName), x+5, y+22);	
			
			strcpy(t.customerName, inputTemp);
    		break;
		}
	} while (1);

    do {
        gotoxy(x+5, y+22); printf("Jumlah : ");
        
        fgets(inputTemp, sizeof(inputTemp), stdin);
 
        if (sscanf(inputTemp, "%d", &t.amount) == 1) {
        
			clearInput(strlen(errMessageInvalidAmount), x+5, y+23);	
        	
            break; // Exit the loop when a numeric input is provided
            
        } else {  
    		inputTemp[strcspn(inputTemp, "\n")] = '\0';

    		length = strlen(inputTemp);
    		
    		clearInput(length, x+14, y+22);		
    
            gotoxy(x+5, y+23); printf(errMessageInvalidAmount);
        }
    } while (1);
    
    do {
        gotoxy(x+5, y+23); printf("Tanggal (dd-mm-yyyy): ");
        
        fgets(inputTemp, sizeof(inputTemp), stdin);
        
 		sscanf(inputTemp, "%d-%d-%d", &t.date.dd, &t.date.mm, &t.date.yyyy);
 		
       	if (isValidDate(t.date)) {
       		clearInput(strlen(errMessageInvalidDate), x+5, y+24);	
        	
           	break; // Exit the loop when a numeric input is provided	
 
		} else {  
    		inputTemp[strcspn(inputTemp, "\n")] = '\0';

    		length = strlen(inputTemp);
    		
    		clearInput(length, x+27, y+23);		
    
            gotoxy(x+5, y+24); printf(errMessageInvalidDate);
        }
    } while (!isValidDate(t.date));
    
    return t;
}

Filter getInputFilterTransaction() {
	
	Filter f;
	
	char inputTemp[100];
	int length;
	
	int i = 1;
    do {
        gotoxy(x+74, y+5); printf("Tanggal Awal (dd-mm-yyyy) : ");
        
        fgets(inputTemp, sizeof(inputTemp), stdin);
        
 		sscanf(inputTemp, "%d-%d-%d",  &f.startDate.dd, &f.startDate.mm, &f.startDate.yyyy);
 		
       	if (isValidDate(f.startDate)) {
       		clearInput(strlen(errMessageInvalidFormatDate), x+74, y+7);	
        	
           	break; // Exit the loop when a date input is provided	
 
		} else {  
    		inputTemp[strcspn(inputTemp, "\n")] = '\0';

    		length = strlen(inputTemp);
    		
    		clearInput(length, x+102, y+5);		
    		
    		if (i != 1) {
    			gotoxy(x+74, y+7); printf(errMessageInvalidFormatDate);	
			}
        }
        i++;
    } while (!isValidDate(f.startDate));
    
    do {
        gotoxy(x+74, y+6); printf("Tanggal Akhir(dd-mm-yyyy) : ");
        
        fgets(inputTemp, sizeof(inputTemp), stdin);
        
 		sscanf(inputTemp, "%d-%d-%d", &f.endDate.dd, &f.endDate.mm, &f.endDate.yyyy);
 		
       	if (isValidDate(f.endDate)) {
       		clearInput(strlen(errMessageInvalidFormatDate), x+74, y+7);	
        	
           	break; // Exit the loop when a date input is provided	
 
		} else {  
    		inputTemp[strcspn(inputTemp, "\n")] = '\0';

    		length = strlen(inputTemp);
    		
    		clearInput(length, x+102, y+6);		
    
            gotoxy(x+74, y+7); printf(errMessageInvalidFormatDate);
        }
    } while (!isValidDate(f.endDate));
	
	return f;
}

int countTrxByFilter(Filter f) {
			
	if (countTrx == 0) {
		return 0;
		
	} else {
		
		int countFilter = 0; 
		
		// Open the file in read mode
	    fileTransactions = fopen(fileTransactionsName, "r");
	    if (fileTransactions == NULL) {
	        gotoxy(x+45, y+9); perror("Error opening file transactions");
	    }
		 
	    char row[MAX_ROW_LENGTH];
	    int lineNumber = 1; 
	    
		// Read and loop through rows of data    
	    while (fgets(row, sizeof(row), fileTransactions) != NULL) {
	 
	        if (lineNumber == 1) { // Skip the first row (header) 
	 			lineNumber++;
	            continue;
	        }
	         
	        // Tokenize the line using strtok based on the pipe character '|'
	        char *data = strtok(row, "|");
	 
	        Transaction trx;
	        
	        char trxDate[12];
	 
	        int fieldCount = 0;
	        while (data != NULL) {
	            switch (fieldCount) { 
	                case 4: // field transaction date
	                    strncpy(trxDate, data, sizeof(trxDate));
	                    break; 
	            }
	
	            // Get the next field
	            data = strtok(NULL, "|");
	
	            fieldCount++;
	        }
	        
	        sscanf(trxDate, "%d-%d-%d", &trx.date.dd, &trx.date.mm, &trx.date.yyyy);
	        
	        // compare transaction date is in range filter
	        if (compareDates(trx.date, f.startDate) >= 0 && compareDates(trx.date, f.endDate) <= 0 ) {
	        	countFilter++;		
			}
	    }
	
	    // Close the file
	    fclose(fileTransactions);
	    
	    return countFilter;
    }
    
    return 0;
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
		
//		gotoxy(x+5, y+26); printf("Stock Produk %d berhasil diupdate", targetId);	    
	} 
}

void clearTableScreen2() {
	for (int i=1; i<height-4; i++) {
		for (int j=1; j<width-45; j++) {
			gotoxy(x+j+44, y+i+9); printf(" ");		
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
    
    gotoxy(x+5, y+25); printf("Penjualan & stock berhasil diupdate.");
    gotoxy(x+5, y+26); printf("Total transaksi: Rp. %d", trx.total);
    
    updateQuantityProductById(product);
    
    viewProducts();
    
    gotoxy(x+5, y+27); printf("Tekan ENTER untuk lanjut ");
    getch();
}

void drawTableTransactions() {
	
    gotoxy(x+45, y+10); drawline(67);
    gotoxy(x+45, y+MAX_ROWS_PER_PAGE+11); drawline(67);
    
    for (int i = 0; i<MAX_ROWS_PER_PAGE+4; i++) {
    	gotoxy(x+45,  y+i+8); printf("%c", separator);
		gotoxy(x+58,  y+i+8); printf("%c", separator);
		gotoxy(x+75,  y+i+8); printf("%c", separator);
		gotoxy(x+92,  y+i+8); printf("%c", separator);
		gotoxy(x+102, y+i+8); printf("%c", separator);
		gotoxy(x+112, y+i+8); printf("%c", separator);
	}
	
	int cord_x[6] = {45, 58, 75, 92, 102, 112};
	for (int i=0; i<4;i+=2) {
	    for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
	        gotoxy(x+ cord_x[j], y+i+8); printf("+");
	    }
	}
	 
	for (int j = 0; j < sizeof(cord_x) / sizeof(cord_x[0]); j++) {
	    gotoxy(x+ cord_x[j], y+MAX_ROWS_PER_PAGE+11); printf("+");
	}
}

void displayTransactionSales() {
			
	gotoxy(x+48, y+5); printf("Lihat Penjualan"); 
	gotoxy(x+45, y+8); drawline(67);
	
	if (countTrx == 0) {
		gotoxy(x+45, y+9); printf("Transaksi Masih Kosong"); 
		
	} else {
		
		Filter f = getInputFilterTransaction();
		
		int countTrxFilter = countTrxByFilter(f); 
			    
		// Open the file in read mode
	    fileTransactions = fopen(fileTransactionsName, "r");
	    if (fileTransactions == NULL) {
	        gotoxy(x+45, y+9); perror("Error opening file transactions");
	    }
		
		drawTableTransactions();
		
	    char row[MAX_ROW_LENGTH];
	    int lineNumber = 1;
	    int page = 1;
	     
		// Read and loop through rows of data    
		int rowCounter = 0;
		int rowLine = 0;
	    while (fgets(row, sizeof(row), fileTransactions) != NULL) {
	 
	        if (lineNumber == 1) { // Skip the first row (header)
	        	gotoxy(x+47,  y+9); printf("Tanggal");
				gotoxy(x+60,  y+9); printf("Nama Pemesan");
				gotoxy(x+77,  y+9); printf("Produk");
				gotoxy(x+94,  y+9); printf("Jumlah");
				gotoxy(x+104, y+9); printf("Nominal");
				
	 			lineNumber++;
	            continue;
	        }
	        
	        if (rowCounter % MAX_ROWS_PER_PAGE == 0) {
	            gotoxy(x+48, y+7); printf("Page %d of %d : ", page++, (countTrxFilter + MAX_ROWS_PER_PAGE - 1) / MAX_ROWS_PER_PAGE);
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
	        
	        sscanf(trxDate, "%d-%d-%d", &trx.date.dd, &trx.date.mm, &trx.date.yyyy);
	        
	 		 // compare transaction date is in range filter
	        if (compareDates(trx.date, f.startDate) >= 0 && compareDates(trx.date, f.endDate) <= 0 ) {
	        	
	        	gotoxy(x+47,  y+rowLine+11); printf("%s", trxDate);
				gotoxy(x+60,  y+rowLine+11); printf("%s", trx.customerName);
				gotoxy(x+77,  y+rowLine+11); printf("%s", trx.productName);
				gotoxy(x+94,  y+rowLine+11); printf("%d", trx.amount);
				gotoxy(x+104, y+rowLine+11); printf("%d", trx.total);	
				
				rowCounter++;		
				rowLine++;	
						
				char next;
				
				// If reached the maximum rows per page, wait for user input to continue
		        if (rowCounter % MAX_ROWS_PER_PAGE == 0) {
		            gotoxy(x+45, y+MAX_ROWS_PER_PAGE+12); printf("Press Enter to continue to the next page...");
		            next = getch();  
		            
		            rowLine = 0; 
		            
		            clearTableScreen2();
		            drawTableTransactions();
		        }	
	        
	        } else {
	        	continue; 
			}
	    }
	
	    // Close the file
	    fclose(fileTransactions);
    }
     
    gotoxy(x+45, y+MAX_ROWS_PER_PAGE+12); printf("Press Enter to continue to the next page...");
    
    char next = getch();	
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
