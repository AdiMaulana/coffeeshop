#include <stdio.h>
 
#include "products.cpp"
#include "transaction.cpp"
#include "application.cpp"

int main() {
	
	initProducts();
	initTransactions();
	
	int option;
	do {
        displayMenu();
           
        scanf("%d", &option);
        switch (option) {
            case 1:
                addProducts();
                break;
                
            case 2:
            	updateProducts();
                break;
                
            case 3:
            	deleteProducts();
                break;
                
            case 4:
                displayProducts(1);
                break;
                
            case 5:
                recordTransactionSales();
                break;
                
            case 6: 
            	displayTransactionSales();
                break;
            
            case 7: 
            	gotoxy(x+50, y+5); printf("Fitur dalam tahap pengembangan");
                break;
                
            case 8:
                gotoxy(x+5, y+19); printf("Terima kasih! Program selesai.");
                break;
                
            default:
                printf("Opsi tidak valid. Silakan pilih opsi 1-7.\n");
                getch();
                break;
        }
    } while (option != 8);
    
	return 0;
}

