#include "rtc.h"

#define TODO return -1 
#include <minix/syslib.h>
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B //registo de controlo do RTC
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7) // UIP está no bit 7
// registos no rtc tem 8 bits: o bit 7 indica se o RTC está a atualizar -- tenho que deslocar o 1 - 7 posiçoes para a esquerda
#define RTC_DM_MSK (1 << 2)

static int bcd_to_bin(uint8_t bcd) { 
  return ((bcd >> 4)* 10) + (bcd & 0x0F); // o segundo masks e pega so nos ultimos 4 digitos
 }

int rtc_read_date(rtc_date *date) { 
  
  uint32_t reg1,reg2,day,month,year; //declara los
  
  // quando rtc esta a atualizar (UIP=1) os valores da struct rtc_date podem estar meio atualizados, tenho que esperar ate UIP=0 para ter os valores estaveis e poder ler c confiança
  
  while (1){
    sys_outb(RTC_ADDR_REG,RTC_REG_A); // ler o a e coloca-lo no addr_reg
    sys_inb(RTC_ADDR_REG, &reg1);
    if (!(reg1 & RTC_UIP_MSK))break;// se estiver a 0 (falso) é seguro entao saio do loop
    tickdelay(micros_to_ticks(20));

    // opcional - pausa o programa um bcdinho assim o cpu nao fica a ler o rtc agressivamente (busy wait)
    // converte 20 microsegundos para ticks
  }

  // Ler registo B para saber se os valores estão em BCD ou binário

    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_inb(RTC_DATA_REG, &reg2);

  
    sys_outb(RTC_ADDR_REG, RTC_REG_DAY);
    sys_inb(RTC_DATA_REG, &day);

    
    sys_outb(RTC_ADDR_REG, RTC_REG_MONTH);
    sys_inb(RTC_DATA_REG, &month);

    
    sys_outb(RTC_ADDR_REG, RTC_REG_YEAR);

    sys_inb(RTC_DATA_REG, &year); //conversao
    uint8_t d = (uint8_t) day;
    uint8_t m = (uint8_t) month;
    uint8_t y = (uint8_t) year;

    if (!(reg2 & RTC_DM_MSK)) { // Se DM=0, é BCD
        d = bcd_to_bin(d);
        m = bcd_to_bin(m);
        y = bcd_to_bin(y);
    }

    // guardar na struct
    date->day = d;
    date->month = m;
    date->year = y;

    return 0;
}

/* nota:
sys_inb != util_sys_inb
o util converte automaticamente para uint8_t , se usasse sys_inb, teria de criar um unsigned long temp e depois fazer cast

*/
