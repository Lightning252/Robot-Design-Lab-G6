#include <stdarg.h>
#include <stdint.h>
#include "printf.h"
#include "stm32f10x_conf.h"

#undef printf

printf_send_func_t printf_send_func = 0;


void printf_setSendFunction(printf_send_func_t function)
{
    printf_send_func = function;
}


/* void fillInUnsignedLongInt(unsigned long int d, unsigned char *msg, int *pos) { */
/*   unsigned long int i, j; */
  
/*   if(d == 0) { */
/*     msg[*pos] = '0'; */
/*     (*pos)++; */
/*     return; */
/*   } */
  
/*   i = 0; */
/*   j = d; */
/*   while(j) { */
/*     j /= 10; */
/*     i++; */
/*   } */
  
/*   j = i; */
/*   while(i) { */
/*     msg[*pos + i - 1] = (char) (d % 10) + '0'; */
/*     d /= 10; */
/*     i--; */
/*   } */
/*   *pos += j; */
/* } */


/* int _printf(printf_send_func_t sf, const char *format, va_list* ap) { */
/*   unsigned char msg[128]; */
/*   const char *fmt = format; */
/*   int pos = 0; */
/*   long int ds; */
/*   unsigned long int d = 0; */
/*   unsigned short int h; */
/*   short int hs;   */
/*   char c, *s; */

/*   while (*fmt) { */
  
/*     if(*fmt != '%') { */
/*       msg[pos] = *fmt; */
/*       pos++; */
/*       fmt++; */
/*       continue; */
/*     } else { */
/*       fmt++; */
/*       assert_param(*fmt); */
/*     } */

/*     switch (*fmt) { */
/*     case 's':              /\* string *\/ */
/*       fmt++; */
/*       s = va_arg(*ap, char *); */
/*       while(*s) { */
/* 	msg[pos] = *s; */
/* 	pos++; */
/* 	s++; */
/*       } */
/*       break; */

      
/*     case 'l':               /\* unsigned long int *\/ */
/*       fmt++; */
/*     case 'd':              /\* int *\/ */
/*     case 'i':              /\* int *\/ */
/*       if(*fmt == 'u') { */
/* 	d = (unsigned long int) va_arg(*ap, long int); */
/*       } else { */
/* 	if(*fmt == 'i' || *fmt == 'd') { */
/* 	    ds = va_arg(*ap, long int); */

/* 	    if(ds < 0) { */
/* 	    msg[pos] = '-'; */
/* 	    pos++; */
/* 	    //set first bit to zero, so there is no  */
/* 	    //difference singend / unsigend now */
/* 	    //and we can use generic function for unsigend int */
/* 	    ds *=-1; */
/* 	    } */
/* 	    d = ds; */
/* 	} */
/*       } */
/*       fmt++; */

/*       fillInUnsignedLongInt(d, msg, &pos); */

/*       break; */

/*     case 'h':              /\* short int *\/ */
/*       fmt++; */
/*       if(*fmt == 'u') { */
/* 	h = (unsigned short int) va_arg(*ap, int); */
/* 	d = h; */
/*       } else { */
/* 	if(*fmt == 'i') { */
/* 	    hs = (short int) va_arg(*ap, int); */
/* 	    d = hs; */
/* 	    if(hs < 0) { */
/* 	    msg[pos] = '-'; */
/* 	    pos++; */
/* 	    //set first bit to zero, so there is no  */
/* 	    //difference singend / unsigend now */
/* 	    //and we can use generic function for unsigend int */
/* 	    ds = hs; */
/* 	    ds *=-1; */
/* 	    d = ds; */
/* 	    } */
/* 	} */
/*       } */
/*       fmt++;	 */
/*       fillInUnsignedLongInt(d, msg, &pos); */
/*       break; */
/*     case 'c':              /\* char *\/ */
/*       /\* need a cast here since va_arg only */
/* 	 takes fully promoted types *\/ */
/*       c = (char) va_arg(*ap, int); */
/*       msg[pos] = c; */
/*       pos++; */
/*       break; */
/*     }     */
/*   } */
  
/*   assert_param(pos < 128); */

/*   int sent = 0; */
/*   int ret = 0; */
  
/*   while(sent < pos) */
/*   { */
/*     ret = sf(msg + sent, pos - sent); */
/*     if(ret < 0) */
/* 	return -1; */
/*     sent += ret; */
/*   } */
  
/*   return pos; */
/* } */

/* int printf(const char *format, ...)  */
/* { */
/*     if(!printf_send_func) */
/* 	return 0; */
    
/*     va_list ap; */
/*     va_start(ap, format); */
/*     return  _printf(printf_send_func, format, &ap); */
/*     va_end(ap); */
/* } */

void testprintf() {
  uint16_t unsignedShort;
  int16_t signedShort;
  uint32_t unsignedLong;
  int32_t signedLong;

  //uint16_t
  unsignedShort = 0;
  printf("uint16_t 0 is %hu \n", unsignedShort);

  unsignedShort = 12345;
  printf("uint16_t 12345 is %hu \n", unsignedShort);
  
  unsignedShort = 65535;
  printf("uint16_t 65535 is %hu\n", unsignedShort);
  
  //int16_t
  signedShort = 0;
  printf("int16_t 0 is %hi \n", signedShort);

  signedShort = 32767;
  printf("int16_t 32767 is %hi \n", signedShort);

  signedShort = -32768;
  printf("int16_t -32768 is %hi \n", signedShort);

  //uint32_t
  unsignedLong = 0;
  printf("uint32_t 0 is %lu \n", unsignedLong);

  unsignedLong = 1234567;
  printf("uint32_t 1234567 is %lu \n", unsignedLong);

  unsignedLong = 4294967295UL;
  printf("uint32_t 4294967295 is %lu \n", unsignedLong);

  //int32_t
  signedLong = 0;
  printf("int32_t 0 is %li \n", signedLong);

  signedLong = 2147483647;
  printf("int32_t 2147483647 is %li \n", signedLong);

  signedLong = -(2147483648UL);
  printf("int32_t -2147483648 is %li \n", signedLong);
}
