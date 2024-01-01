#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void printHeaderTitle() {
	#ifdef _WIN32
	    // For Windows
	    system("cls");
	#else
	    // For Linux and macOS
	    system("clear");
	#endif
	
	printf("\n==========================================================");
    printf("\n| \t\t Program Kasir Kedai Kopi \t\t |");
    printf("\n==========================================================");
}

void displayMenu() {
	printHeaderTitle();
    printf("\n|\t   DAFTAR MENU \t\t\t\t\t |");
    printf("\n|\t -----------------------------------------  \t |");
    printf("\n|\t| 1. Tambah Produk 	\t\t | \t |");
    printf("\n|\t| 2. Update Produk 	\t\t | \t |");
    printf("\n|\t| 3. Hapus Produk 	\t\t | \t |");
    printf("\n|\t| 4. Lihat Daftar Produk \t\t | \t |");
	printf("\n|\t| 5. Penjualan \t\t\t\t | \t |");
    printf("\n|\t| 6. Penjualan per Tanggal \t\t | \t |");
    printf("\n|\t| 7. Keluar 	\t\t\t | \t |");
    printf("\n|\t -----------------------------------------  \t |");
    printf("\n==========================================================");
    printf("\n Pilih Opsi Anda (1-7) : ");
}

