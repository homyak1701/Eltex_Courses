//вывод - так как мало словий, то хеш-таблица не создалась;

#include <stdio.h>
 
int main()
{

    int input = 4;

    switch ( input ) {
        case 1:            /* обратите внимание на двоеточие, точки сзапятой тут нет */
            printf("«His Death Was Just The Beginning»\n");
            break;
        case 2:
            printf("«Scary Just Got Sexy»\n");
            break;
        case 3:
            printf("«Fight the dead. Fear the living»\n");
            break;
        case 4:
            printf( "Сегодня смотреть ничего не будем :(\n" );
            break;
        default:
            printf( "Неправильный ввод.\n" );
    }
    getchar();
    return 0;
}

/*
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$4, -4(%rbp)
	cmpl	$4, -4(%rbp)
	je	.L2
	cmpl	$4, -4(%rbp)
	jg	.L3
	cmpl	$3, -4(%rbp)
	je	.L4
	cmpl	$3, -4(%rbp)
	jg	.L3
	cmpl	$1, -4(%rbp)
	je	.L5
	cmpl	$2, -4(%rbp)
	je	.L6
	jmp	.L3
*/