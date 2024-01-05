#include <18F4550.h> // dinh kem file dinh nghia PIC su dung
#FUSES hs // su dung giao dong thach anh ngoài 
#FUSES PLL1,NOUSBDIV,NOWDT,NOIESO,MCLR,CPUDIV1

#use delay(clock=12000000) // khai bao tan so thach anh cho hàm delay                            
#use i2c(sda = pin_c6,scl = pin_c7) // su dung giao tiep i2c cho DS1307 
#define LCD_RS PIN_D2
#include <LCD 4 bit.c> // Dinh kem file giao tiep LCD che do 4 BIT   
                                                                       
#define k1 pin_B0 // nut nhan set thoi gian                      
#define k2 pin_B1 // nut nhan tang                                                                                                                                                                                                            
#define k3 pin_B2 // nut nhan giam                                                                    
                                                                              
                                                                                                  
#define led pin_a0                             
                                     
#define ONE_WIRE_PIN PIN_D3
#include<1wire.c> //truy xuat vao file 1wire                 
#include<ds1820.c> //truy xuat vao file ds1820.c 
                                                                                                         
int8 c_g,dv_g,c_p,dv_p,c_h,dv_h,thu,c_d,dv_d,c_t,dv_t,c_n,dv_n; // khai bao ca bien chua gia tri thap phan cua cac don vi thoi gian
int8 mang_tg_rtc[7]; // mang chua giu lieu thoi gian cua rtc
              
float nhiet_do; //khai bao bien nhiet do so thuc 32 bit 


void convert_bcd(); // chuen doi so 2 bcd thanh 2 so thap phan rieng le bao gom chuc va don vi rieng cua cac gia tri thoi gian  
void ReadRTC(int8 * buff); // doc cac gia tri thoi gian tu RTC vao mang
void WriteRTC(int8 * buff); // ghi mang chua gia tri thoi gian vao RTC   
void rtc_init(void);
void chinh_gio(); // khai bao ham chinh thoi gian
void chinh_bt(); // khai bao ham chinh thoi gian bao thuc       
void tinh_thu();
//========================================                                                 
void hienthi1(int8 m)//hien thi chinh cai dat thoi gian vao 1307
{          
   LCD_putcmd(0x80); // lenh dua con tro den hang 1 cot 1 LCD  
    LCD_putchar(' ');
   if(m==1)                         
   {                     
      printf(LCD_putchar,"__"); // hien thi dau __ thay vì gia tri de bien minh dang chon gia tri can chinh nao  
   }
   else 
   {
      LCD_putchar(c_h + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_h + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }                           
   printf(LCD_putchar,":");
   if(m==2)
   {
      printf(LCD_putchar,"__");
   } 
   else
   {  
      LCD_putchar(c_p + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD 
      LCD_putchar(dv_p + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   } 
   printf(LCD_putchar,":"); 
   if(m==3)
   {
      printf(LCD_putchar,"__");
   }      
   else 
   {          
      LCD_putchar(c_g + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD 
      LCD_putchar(dv_g + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }                           
   printf(LCD_putchar," ");
   printf(LCD_putchar," "); 
   if(thu!=8)
   {                             
      printf(LCD_putchar,"T"); 
      printf(LCD_putchar,"H");
      printf(LCD_putchar,"U");     
      printf(LCD_putchar," "); 
      LCD_putchar(thu + 0x30);
   }                                           
   else
   {                         
       printf(LCD_putchar,"C"); 
      printf(LCD_putchar,"H");
      printf(LCD_putchar,"U");     
      printf(LCD_putchar," ");
       printf(LCD_putchar,"N");
   }  
   
   
    
   //printf(LCD_putchar,"Nhiet: %03.1f ",nhiet_do); //xuat len lcd nhiet do do duoc     
   LCD_putcmd(0xC0); // lenh dua con tro den hang 2 cot 1 LCD  
   if(m==4) 
   {
     printf(LCD_putchar,"__");
   } 
   else 
   {
      LCD_putchar(c_d + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD 
      LCD_putchar(dv_d + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar,"/"); 
   if(m==5)
   {                                                                                                  
      printf(LCD_putchar,"__");
   }                                                                                                        
   else 
   {
      LCD_putchar(c_t + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
      LCD_putchar(dv_t + 0x30);  // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }
   printf(LCD_putchar,"/");  
   printf(LCD_putchar,"20");
   if(m==6)                                                                                 
   {
      printf(LCD_putchar,"__"); 
   } 
   else
   {
      LCD_putchar(c_n + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD 
      LCD_putchar(dv_n + 0x30); // doi ma thap phan sang ASCII va xuat hien thi len LCD
   }                         
   printf(LCD_putchar," "); 
   printf(LCD_putchar,"%03.1f",nhiet_do); //xuat len lcd nhiet do do duoc
   LCD_putchar(223); //xuat len lcd ki tu do 
   //LCD_putchar("C    "); //xuat len ldc ki tu C     
}
//======================================== 
void Cai_dat_thoi_gian(unsigned char giay,unsigned char phut,unsigned char gio,unsigned char thu_1,unsigned char ngay,unsigned char thang,unsigned char nam)
{
   mang_tg_rtc[0] = giay; // chuyen 2 so thap phan cua giay thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[1] = phut; // chuyen 2 so thap phan cua phut thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[2] = gio; // chuyen 2 so thap phan cua gio thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[3] = thu_1;                     
   mang_tg_rtc[4] = ngay; // chuyen 2 so thap phan cua ngay thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[5] = thang; // chuyen 2 so thap phan cua thang thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[6] = nam; // chuyen 2 so thap phan cua nam thanh BCD luu vao mang thoi gian 
   WriteRTC(&mang_tg_rtc[0]); // ghi mang thoi gian vao DS1307     
}
//============================      
void main()         
{
   output_high(led);       
   lcd_init(); // khoi tao LCD             
   rtc_init();          
   LCD_putcmd(0x80); // lenh dua con tro den dong 1 cot 1 
   printf(LCD_putchar,"KHOA DIEN TU");  // xuat hien thi chuoi len LCD 
   LCD_putcmd(0xC0); // lenh dua co tro den dong 2 cot 1 
   printf(LCD_putchar,"DO AN CS NHOM 10 "); // xuat hien thi chuoi len LCD  
   DELAY_MS(3000); // tao thoi gian tre  
   //Ghi vao DS1307
   //Cai_dat_thoi_gian(9,9,0,6,8,12,23);
   //DELAY_MS(300); 
   ReadRTC(&mang_tg_rtc[0]); // do thoi gian tu RTC   
      convert_bcd(); // chuyen doi thoi gian trong RTC sang so thap phan  
      tinh_thu();  
      hienthi1(0); // xuat hien thi len LCD   
     DELAY_MS(3000);   
   //======================================== 
   while(true)                                   
   { 
      if(ds1820_read()==1)
      {
         nhiet_do=gia_tri_ds;
         nhiet_do/=10; 
      }               
      ReadRTC(&mang_tg_rtc[0]); // do thoi gian tu RTC   
      convert_bcd(); // chuyen doi thoi gian trong RTC sang so thap phan  
      tinh_thu();  
      hienthi1(0); // xuat hien thi len LCD   
      if(input(k1)==0) // khi nhan phim chinh thoi gian
      { 
         while(input(k1)==0); // cho cho toi khi nut nhan duoc nha 
         chinh_gio(); // thuc hien ham chinh thoi gian                    
      }
      
   }  
 }
//======================================== 
void convert_bcd() // BCD sang so thap phan
{
   c_g=mang_tg_rtc[0]>>4; // dich phai 4 bit de lay hang chuc  
   dv_g=mang_tg_rtc[0]&15; // xoa 4 vit cao de lay hang don vi 
   c_p=mang_tg_rtc[1]>>4; // dich phai 4 bit de lay hang chuc 
   dv_p=mang_tg_rtc[1]&0x0f; // xoa 4 vit cao de lay hang don vi 
   c_h=mang_tg_rtc[2]>>4; // dich phai 4 bit de lay hang chuc 
   dv_h=mang_tg_rtc[2]&0x0f; // xoa 4 vit cao de lay hang don vi 
   thu = mang_tg_rtc[3]; 
   c_d=mang_tg_rtc[4]>>4; // dich phai 4 bit de lay hang chuc 
   dv_d=mang_tg_rtc[4]&0x0f; // xoa 4 vit cao    de lay hang don vi                          
   c_t=mang_tg_rtc[5]>>4; // dich phai 4 bit de lay hang chuc 
   dv_t=mang_tg_rtc[5]&0x0f; // xoa 4 vit cao de lay hang don vi                       
   c_n=mang_tg_rtc[6]>>4; // dich phai 4 bit de lay hang chuc 
   dv_n=mang_tg_rtc[6]&0x0f; // xoa 4 vit cao de lay hang don vi 
}
//======================================== 
void chinh_gio() // ham chinh thoi gian thuc
{
   int8 mode=1,i=0,t;     
   ReadRTC(&mang_tg_rtc[0]);
   convert_bcd(); // chuyen doi thoi gian trong rtc ra so thap phan
   while(true) // vong lap vo han. chi thoat neu co lenh break cua ham
    {
         tinh_thu();  
         if(i<5) // khi bien lap i tu 0-4 thi hien thi gia tri can chinh 
            hienthi1(0); // hien thi nhu binh thuong cac gia tri thoi gian
         else // khi bien lap i trong lan quet 5-9 thi hien thi dau '?' cho bien minh dang chon gia tri chinh nao 
            hienthi1(mode); // hien thi dau '?' tai gia tri duoc chon 
         i++; if(i>=10) i = 0; // tang bien i sau moi lan quet (gioi han 0-9) 
         if(input(k2)==0) // nut tang duoc nhan
         {  
            while(input(k2)==0); // cho nut tang duoc nha
            i=0; // cho i = 0 de tiep tuc hien thi gia tri duoc chon
            switch(mode) // lua chon tang gia tri tuy thuoc vao bien mode      
            {
               case 1: // mode = 1 // tang gio gio 
                  t = c_h*10+dv_h; // ghep 2 so thap phan rieng le vao mot bien tam 
                  if(t<23) // neu gio <23 thi tang gio 
                     t++;
                  else 
                     t = 0; // neu gio >23 thi nap lai gio =0 
                  c_h = t/10; // tach roi hang chuc cua bien tam gio
                  dv_h = t%10; // tach toi hang don vi cua bien tam gio
                  break; // thoat khoi ham chon
               case 2: // mode = 2 // tang phut 
                  t = c_p*10+dv_p; // tuong tu nhu voi gio  
                  if(t<59) 
                     t++;
                  else                                                                               
                     t = 0; 
                  c_p = t/10; 
                  dv_p = t%10;
                  break;
               case 3: 
                  t = c_g*10+dv_g; 
                  if(t<59) 
                     t++;
                  else 
                     t = 0;
                  c_g = t/10; 
                  dv_g = t%10;
                  break;
               case 4: 
                  t = c_d*10+dv_d; 
                  if(t<31) 
                     t++;
                  else 
                     t = 1;
                  c_d = t/10; 
                  dv_d = t%10;
                  break;
               case 5: 
                  t = c_t*10+dv_t; 
                  if(t<12) 
                     t++;
                  else 
                     t = 1;
                  c_t = t/10; 
                  dv_t = t%10;
                  break;
               case 6: 
                  t = c_n*10+dv_n; 
                  if(t<99) 
                     t++;
                  else 
                     t = 0; 
                  c_n = t/10; 
                  dv_n = t%10;
                  break;
            }
         }     
         //---------------------------------------- 
         if(input(k3)==0) // khi nhan nut giam
         {  
            while(input(k3)==0); // cho nha nut giam
            i=0;        
            switch(mode)
            {
               case 1: 
                  t = c_h*10+dv_h; 
                  if(t>0) 
                     t--;
                  else 
                     t = 23; 
                  c_h = t/10; 
                  dv_h = t%10; //chuc gio + dvi gio nho hon 0 thi nhay ve 23
                  break;
               case 2: 
                  t = c_p*10+dv_p; 
                  if(t>0) 
                     t--; 
                  else 
                     t = 59; 
                  c_p = t/10; 
                  dv_p = t%10; //chuc phuc + dvi phut gio nho hon 0 thi nhay ve 59
                  break;
               case 3: 
                  t = c_g*10+dv_g; 
                  if(t>0) 
                     t--; 
                  else 
                     t = 59; 
                  c_g = t/10; 
                  dv_g = t%10;//chuc giay + dvi giay gio nho hon 0 thi nhay ve 59
                  break;
               case 4: 
                  t = c_d*10+dv_d; 
                  if(t>1) 
                     t--; 
                  else 
                     t = 31; 
                  c_d = t/10; 
                  dv_d = t%10;//chuc ngay + dvi ngay nho hon 1 thi nhay ve 31
                  break;
               case 5: 
                  t = 
                  c_t*10+dv_t; 
                  if(t>1) 
                     t--; 
                  else 
                     t = 12; 
                  c_t = t/10; 
                  dv_t = t%10;//chuc thang + dvi thango nho hon 1 thi nhay ve 12
                  break;
               case 6: 
                  t = c_n*10+dv_n; 
                  if(t>0) 
                     t--; 
                  else 
                     t = 99; 
                  c_n = t/10; 
                  dv_n = t%10;//chuc nam + dvi nam nho hon 0 thi nhay ve 99
                  break;
            }        
         }
         if(input(k1)==0) // khi nhan nut set 
         { 
            while(input(k1)==0); // cho nha nut set 
            i=5;   
            mode++; // tang bien chon gia tri chinh 
            if(mode>6)  
               break; // khi nhan chinh het 6 gia tri thi thoat khoi ham chinh thoi gian  
         }                                                           
    }
   //======================================== 
   mang_tg_rtc[0] = (c_g<<4) | dv_g; // chuyen 2 so thap phan cua giay thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[1] = (c_p<<4) | dv_p; // chuyen 2 so thap phan cua phut thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[2] = (c_h<<4) | dv_h; // chuyen 2 so thap phan cua gio thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[3] = thu;                     
   mang_tg_rtc[4] = (c_d<<4) | dv_d; // chuyen 2 so thap phan cua ngay thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[5] = (c_t<<4) | dv_t; // chuyen 2 so thap phan cua thang thanh BCD luu vao mang thoi gian 
   mang_tg_rtc[6] = (c_n<<4) | dv_n; // chuyen 2 so thap phan cua nam thanh BCD luu vao mang thoi gian 
   WriteRTC(&mang_tg_rtc[0]); // ghi mang thoi gian vao DS1307     
} 
//======================================== 
const int8 d_thang[] = {0,0,3,3,6,1,4,6,2,5,0,3,5};
void tinh_thu() 
{                                                                
   int8 a;
   int8 b;
   b=c_n*10+dv_n+5;//31/12/1999: THU 6
   a=c_n*10+dv_n&0xFC;
   a=a>>2;
   b=a+b;
   if(c_n*10+dv_n%4==0) // nam nhuan
   {
      if(c_t*10+dv_t>=3)
      {
         b++;
      }
      b--;
   }
   b=d_thang[c_t*10+dv_t]+b;
   a=c_d*10+dv_d+b;
   a=a%7;
   thu=a+2;
}
//======================================== 
void ReadRTC(int8 * buff) // doc du lieu tu 1307
{
   i2c_Start(); // Lenh bat dau i2c  
   i2c_Write(0xD0); // ghi ma ghi vào DS1307
   i2c_Write(0x00); // Ghi dia chi 0 vao DS
   i2c_Start(); // Lenh restart i2c
   i2c_Write(0xD1); // chi ma doc vao DS
   *(buff+0)=i2c_Read(1); // Doc giay voi bit thua nhan (ACK) de dia chi tang tu dong
   *(buff+1)=i2c_Read(1); // doc phut
   *(buff+2)=i2c_Read(1); // doc gio                                       
   *(buff+3)=i2c_Read(1); // doc thu
   *(buff+4)=i2c_Read(1); // doc ngay
   *(buff+5)=i2c_Read(1); // doc thang
   *(buff+6)=i2c_Read(0); // doc nam khong bit thua nhan de ket thuc viec doc
   i2c_Stop(); // Lenh dung i2c
}
//======================================== 
void WriteRTC(int8 *buff) // ghi du lieu vao 1307
{
   i2c_Start(); // lenh bat dau i2c
   i2c_Write(0xD0); // ghi ma ghi vao DS
   i2c_Write(0x00); // nap dia chi ban dau vao DS
   i2c_Write(*(buff+0)); // ghi giay
   i2c_Write(*(buff+1)); // ghi phut
   i2c_Write(*(buff+2)); // ghi gio
   i2c_Write(*(buff+3)); // ghi thu
   i2c_Write(*(buff+4)); // ghi ngay
   i2c_Write(*(buff+5)); // ghi thang
   i2c_Write(*(buff+6)); // ghi nam
   i2c_Stop(); // lenh dung i2c
} 
//========================================
void rtc_init(void) // khoi tao cho ds1307 
{  
   int8 a; // byte luu tam gia tri o nho 0 cua ds
   i2c_start();
   i2c_write(0xD0); // ghi vao ds1307 chieu ghi
   i2c_write(0); // ghi dia chi can doc
   i2c_start(); // start bit i2c 
   i2c_write(0xD1); // ghi vao ds1307 chieu doc
   a = i2c_read(0);
   i2c_stop();
   if ((a&0x80)!=0)                                  
   {
      a&=0x7f;
      i2c_start();         
      i2c_write(0); // ghi vao ds1307 chieu ghi
      i2c_write(0); // ghi dia chi can doc
      i2c_write(a); // 
      i2c_stop();   
   }  
}

