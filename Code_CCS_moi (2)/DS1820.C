int8 lan_doc_ds=0;
int16 thoi_gian_tre_ds=0;
int16 gia_tri_ds;                       
int8 ds1820_read()
{
   int8 ket_qua=0;         
   int8 temp1, temp2;  
   int16 temp3;                               
   float result; //khai bao bien tra ve ket qua sau doan chuong trinh  
   if(thoi_gian_tre_ds>0)
   {
      thoi_gian_tre_ds--;
   }
   if(thoi_gian_tre_ds==0)
   {
      if(lan_doc_ds==0)        
      {
         onewire_reset(); //lenh reset                                                                                                                          
         ghi_1wire(0xCC); //bo qua truy xuat rom                 
         ghi_1wire(0x44); //Lenh chuyen doi nhiet do  
         lan_doc_ds=1; 
         thoi_gian_tre_ds=8; 
      }
      else                 
      {
         onewire_reset();  //lenh reset  
         ghi_1wire(0xCC); //lenh bo qua truy xuat rom 
         ghi_1wire(0xBE); //Lenh doc du lieu tu ds18b20                                      
         temp1 = doc_1wire(); //doc byte 0 8 bit
         temp2 = doc_1wire(); //doc byte 1 8 bit
         temp3 = make16(temp2, temp1); //noi hai byte thanh tu 16 bit         
         result =  temp3 / 16.0;  //Chuyen doi nhiet do ds18b20 thanh dang so thuc 16 bit
         result*=10; //
         gia_tri_ds=result;
         ket_qua=1;
         lan_doc_ds=0; 
         thoi_gian_tre_ds=1; 
      }         
   }
   //delay_ms(750);
   
   return(ket_qua);  
} 
