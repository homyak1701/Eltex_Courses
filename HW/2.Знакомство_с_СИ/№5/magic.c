#include <stdio.h>
#include <string.h>

int proverka(int mas[])



s11 = mas[i] + mas[i+1] + mas[i+2];
s12 = mas[i+size] + mas[i+1+size] + mas[i+2+size];
s13 = mas[i+size*2] + mas[i+1+size*2] + mas[i+2+size*2];

s21 = mas[i] + mas[i+size] + mas[i+size*2]
s22 = mas[i+1] + mas[i+size+1] + mas[i+size*2+1]
s23 = mas[i+2] + mas[i+size+2] + mas[i+size*2+2]

s31 = mas[i] + mas[i+size+1] + mas[i+size*2+2]
s32 = mas[i+size*2] + mas[i+size+1] + mas[i+2]

int main(){

    int i1, i2, i3, i4, i5, i6, i7, i8, i9; //счетчики
    int s11, s12, s13; // сумма строк
    int s21, s22, s23; // сумма столбцов
    int s31; s32; // сумма диагоналей
    int size; // размер матрицы
    int p = 0;
    printf("Enter size matrx --- ");
	scanf("%d", &size);
	int mas[size*size];
    
    for(i1=0; i1<size*size; i1++){
			mas[i1] = p++;
	}

    // for(i1=0; i1<size*size; i1++){
    //     for(i=1; i<size*size-1; i++){
    //         for(i=2; i<size*size-2; i++){
    //             for(i=3; i<size*size-3; i++){
    //                 for(i=4; i<size*size-4; i++){
    //                     for(i=5; i<size*size-5; i++){
    //                         for(i=6; i<size*size-6; i++){
    //                             for(i=7; i<size*size-7; i++){
    //                                 for(i=8; i<size*size-8; i++){

    //                                     }
    //                                 }
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }    



    for(int i1=0; i1<size*size ; i1++){
            if(0 == (mas[i1] % size)){
                printf("\n");
            }

			if (9 > mas[i1]){
				printf("%d  ", mas[i1]);
			} else {
				printf("%d ", mas[i1]);
			}
	}

    return 0;
}