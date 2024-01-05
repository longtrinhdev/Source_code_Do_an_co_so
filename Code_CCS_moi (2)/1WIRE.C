/*-------Dinh nghia one-wire-------*/ 
#ifndef ONE_WIRE_PIN
#define ONE_WIRE_PIN PIN_D3  
#endif

void onewire_reset()  // reset one wire 
{ 
 output_low(ONE_WIRE_PIN); 
 delay_us( 500 ); // keo 1-wire xuong thap de reset      
 output_float(ONE_WIRE_PIN); // tha 1-wire len muc cao
 delay_us( 250 ); // cho 1-wire san sang
}                                                                                     
//====================
//Ghi vao 1 wire
void ghi_1wire(int data) 
{ 
 int dem; // khai bao bien dem 
 for (dem=0; dem<8; ++dem) // Lap lai 8 lan
 { 
  output_low(ONE_WIRE_PIN); // dua chan 1 wire xuong thap 
  output_bit(ONE_WIRE_PIN, shift_right(&data,1,0)); // xuat tung bit du lieu tu 0 - 8 
  delay_us(60); // tao tre 60us
  output_float(ONE_WIRE_PIN); // dua 1-wire len muc cao, 
 } 
} 
//====================
//Doc 1 byte tu 1 wire
int doc_1wire() 
{ 
 int dem, dulieu; // khai bao bien dem va bien du lieu 
 for (dem=0; dem<8; ++dem) // Lap lai 8 lan 
 { 
  output_low(ONE_WIRE_PIN); // dua chan 1 wire xuong thap 
  output_float(ONE_WIRE_PIN); // tha chan 1 wire len cao
  shift_right(&dulieu,1,input(ONE_WIRE_PIN)); // nhan du lieu tu 1 wire. 
  delay_us( 60 ); // cho 1 wire san sang
 } 
 return( dulieu ); //tra ve ket qua cho chuong trinh chinh
} 
