#include <stdio.h>
 
#include "products.cpp"

int main() {
	initProducts();
	
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
                // record penjualan
                printf("Fitur belum diimplementasikan.\n"); getch();
                break;
            case 6:
                // record penjualan per tanggal 
                printf("Fitur belum diimplementasikan.\n"); getch();
                break;
            case 7:
                printf("Terima kasih! Program selesai.\n");
                break;
            default:
                printf("Opsi tidak valid. Silakan pilih opsi 1-7.\n");
                getch();
                break;
        }
    } while (option != 7);
    
	return 0;
}

