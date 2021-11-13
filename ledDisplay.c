

/*
  8 numaral� resetPin pini ikinci �ipe takild�.
  1.�ipin reseti topraga �ekildi
*/
  
  
#include <SoftwareSerial.h>


#define firstBit  2                 // 74HC154N kod ��z�c�n� 4 bit bacak balant�s� yap�ld�
#define secondBit 3   
#define thirdBit  4                    
#define fourthBit 5

SoftwareSerial bt(0, 1);            //bluetooth haberle�mesi i�in pin bacaklar�n� aktif ettik
                           

boolean shiftTyp1 = false;          //# sembolu ile aktif hale gelir.karakter olarak kayd�r�r.
boolean shiftTyp2 = false;          //@ sembol� ile aktif edilir.metin olarak kayd�r�r.
boolean shiftTyp3 = false;          //> sembolu ile aktif olur.metin sag tarafa kaymaya baslar.

boolean alfabe    = false;          //$ ile aktif edilir. harf ve rakamlar� yazdir.

byte latchPin = 12; //12  
byte clockPin = 11; //11            //  74HC595 Shiftregister pin ba�lant�lar�n� yapt�k
byte dataPin  = 13; //14                                                
byte resetPin = 8;                  //Shiftregister resetPin baca��n� baglad�k


byte firstPin ;						// 4-to-16 Decoder Pin Inputs
byte secondPin; 
byte thirdPin ; 
byte fourthPin;  

char chr[25] = "";                 //bo� karakter dizisi tan�mlad�k

int numbers[15][10];               //yazd�r�l�cak olan harflerin dizisini bo� olarak tan�mlad�k

int p;

String metin = "";                 //bo� metin tan�mlad�k







int characters[38][15] = {
  {56, 68, 130, 130, 254, 130, 130, 130, 0  },       //A 
  {248, 132, 132, 248, 132, 130, 132, 248   },       //B 
  {120, 132, 128, 128, 128, 128, 132, 120   },       //C 
  {248, 132, 132, 132, 132, 132, 132, 248   },       //D 
  {248, 128, 128, 240, 128, 128, 128, 248   },       //E
  {248, 128, 128, 240, 128, 128, 128, 128   },       //F
  {252, 132, 128, 128, 156, 132, 132, 252   },       //G
  {132, 132, 132, 252, 132, 132, 132, 132   },       //H
  {24, 0, 24, 24, 24, 24, 24, 24, 0, 0, 0   },       //�
  {0, 28, 8, 8, 136, 136, 136, 248, 0, 0    },       //J
  {68, 72, 80, 96, 80, 72, 68, 66, 0, 0     },       //K
  {64, 64, 64, 64, 64, 64, 64, 124, 0, 0    },     	 //L
  {65, 99, 85, 73, 65, 65, 65, 65, 0, 0     },       //M
  {129, 193, 161, 145, 137, 133, 131, 129, 0},       //N
  {60, 66, 129, 129, 129, 129, 66, 60       },       //O
  {62, 66, 66, 124, 64, 64, 64, 64, 0       },       //P
  {62, 66, 66, 124, 80, 72, 68, 66, 0       },       //R
  {30, 32, 32, 28, 2, 2, 2, 60, 0, 0        },       //S
  {127, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0        },       //T
  {0, 65, 65, 65, 65, 65, 65, 62, 0			},       //U
  {0, 65, 65, 65, 65, 34, 20, 8, 0			},       //V
  {0, 65, 34, 20, 8, 8, 8, 8, 0, 0			},       //Y
  {0, 126, 2, 4, 8, 16, 32, 126, 0			},       //Z
  {65, 34, 20, 8, 20, 34, 65, 0, 0			},       //X
  {62, 65, 65, 65, 73, 69, 67, 63			  },       //Q
  {0, 65, 65, 73, 73, 73, 73, 62			  },       //W

  {0, 126, 66, 66, 66, 66, 66, 126			},        //0
  {0, 8, 24, 8, 8, 8, 8, 28, 0, 0			  },        //1
  {0, 126, 2, 2, 126, 64, 64, 126			  },        //2
  {0, 126, 2, 2, 126, 2, 2, 126, 0			},        //3
  {0, 66, 66, 66, 126, 2, 2, 2				  },        //4
  {0, 126, 64, 64, 126, 2, 2, 126			  },        //5
  {0, 126, 64, 64, 126, 66, 66, 126			},        //6
  {0, 126, 2, 4, 8, 8, 8, 8, 0				  },        //7
  {0, 126, 66, 66, 126, 66, 66, 126			},        //8
  {0, 126, 66, 66, 126, 2, 2, 126			  },        //9

} ;


void setup() 
{  

	bt.begin(9600);                //Bluetooth haberle�me h�z�n� belirledik
	Serial.begin(9600);            //Arduino haberle�me h�z�n� belirledik
	Serial.setTimeout(20);         //Okunan de�erler aras� zaman a��m�n� belirledik
	
    //Bacak ba�lant�lar�m� ��k�� olarak tan�mlad�k
	pinMode(firstBit,   OUTPUT);
	pinMode(secondBit,  OUTPUT);
	pinMode(thirdBit,   OUTPUT);
	pinMode(fourthBit,  OUTPUT);
	pinMode(latchPin,   OUTPUT);
	pinMode(clockPin,   OUTPUT);
	pinMode(dataPin,    OUTPUT);
	pinMode(resetPin,   OUTPUT);
	
	digitalWrite(resetPin, HIGH);
	

  
  
 // Yazd�r�l�cak olan harf bilgilerini s�f�rlad�k 
for (byte i = 0; i <= 9; i++)
 {              
    for (byte t = 0; t <= 9; t++) 
	{
      numbers[i][t] = "";
    }
  }
}

void loop()
 {
  //Clear Function
  temizle();
  
  shiftTyp1 = false;
  shiftTyp2 = false;
  shiftTyp3 = false;
  alfabe    = false;
  
  if (Serial.available()) 
  {
    metin = Serial.readString();   // gelen kelimeleri  oku
    metin.toCharArray(chr, 25);    // okudu�un kelimeleri dizi haline getir
    p = metin.length();            // kelimenin uzunlu�unu bul
    p = p - 3;                     // i�ine yaramayan karakterleri ��kar
    delay(50);

    for (byte t = 0; t <= p; t++) 
	{
		//gelen karaktere g�re metin listesini olu�tur
      switch (chr[t]) 
	  {
        case '$':
          shiftTyp1 = false;
          shiftTyp2 = false;
          shiftTyp3 = false;
          alfabe 	= true;
          break;
        case '#':
          shiftTyp1 = true;
          shiftTyp2 = false;
          shiftTyp3 = false;
          alfabe 	= false;
          break;
        case '@':
          shiftTyp1 = false;
          shiftTyp2 = true;
          shiftTyp3 = false;
          alfabe 	= false;
          break;
        case '>':
          shiftTyp1 = false;
          shiftTyp2 = false;
          shiftTyp3 = true;
          alfabe 	= false;
          break;
        case 'a':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[0][r];
          }
          break;
        case 'b':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[1][r];
          }
          break;
        case 'c':
          for (byte r = 0; r <= 10; r++)
		   {
            numbers[t][r] = characters[2][r];
          }
          break;
        case 'd':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[3][r];
          }
          break;
        case 'e':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[4][r];
          }
          break;
        case 'f':
          for (byte r = 0; r <= 10; r++)
		   {
            numbers[t][r] = characters[5][r];
          }
          break;
        case 'g':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[6][r];
          }
          break;

        case 'h':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[7][r];
          }
          break;
        case 'i':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[8][r];
          }
          break;
        case 'j':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[9][r];
          }
          break;
        case 'k':
          for (byte r = 0; r <= 10; r++)
		   {
            numbers[t][r] = characters[10][r];
          }
          break;
        case 'l':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[11][r];
          }
          break;


        case 'm':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[12][r];
          }
          break;
        case 'n':
          for (byte r = 0; r <= 10; r++)
		   {
            numbers[t][r] = characters[13][r];
          }
          break;
        case 'o':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[14][r];
          }
          break;
        case 'p':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[15][r];
          }
          break;
        case 'r':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[16][r];
          }
          break;


        case 's':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[17][r];
          }
          break;
        case 't':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[18][r];
          }
          break;
        case 'u':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[19][r];
          }
          break;
        case 'v':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[20][r];
          }
          break;
        case 'y':
          for (byte r = 0; r <= 10; r++)
		   {
            numbers[t][r] = characters[21][r];
          }
          break;
       

 case 'z':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[22][r];
          }
          break;
        case 'x':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[23][r];
          }
          break;
        case 'q':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[24][r];
          }
          break;
        case 'w':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[25][r];
          }
          break;

        case '0':
          for (byte r = 0; r <= 10; r++)
		   {
            numbers[t][r] = characters[26][r];
          }
          break;

        case '1':
          for (byte r = 0; r <= 10; r++)
		  {
            numbers[t][r] = characters[27][r];
          }
          break;
        case '2':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[28][r];
          }
          break;
        case '3':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[29][r];
          }
          break;
        case '4':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[30][r];
          }
          break;
        case '5':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[31][r];
          }
          break;
        

case '6':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[32][r];
          }
          break;
        case '7':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[33][r];
          }
          break;
        case '8':
        	
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[34][r];
          }
          break;
        case '9':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = characters[35][r];
          }
          break;
        case ' ':
          for (byte r = 0; r <= 10; r++) 
		  {
            numbers[t][r] = 0;
          }
          break;

		//eger okunan karakter tan�ml� de�ilse hata bilgisi ver
        default:              
          Serial.print("HATA");
          break;
      }
    }
    
    delay(750);
    
    if (shiftTyp1 == false && shiftTyp2 == false && shiftTyp3 == false)
	 {
	 //Kayd�rma i�lemi
      for (int e = 0; e <= p; e++) 
	  {
	    //Bu d�ng� s�resi kadar yan�k kal
        for (int d = 0; d <= 25; d++)
		 {
		  //Sat�r aktif
          for (int c = 0; c <= 8; c++) 
		  {
            activateLineFunc1(c);
            
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin, clockPin, MSBFIRST, highByte(numbers[e][c] )  ) ;
            
            resett();
            
            shiftOut(dataPin, clockPin, MSBFIRST, lowByte( numbers[e][c] ) );
            digitalWrite(latchPin, HIGH);

          }
        }

      }
    }





    if (shiftTyp2 == true) 
	{
      for (byte t = 1; t <= p; t++)           // x degeri secme
	   {
       	 for (byte r = 0; r <= 9; r++)        //Kayd�rma
			{
          for (byte d = 0; d <= 4 ; d++)     //Bekletme
		   		{ 
            for (byte c = 0; c <= 8; c++)    //Sat�r aktif
					{
					
              activateLineFunc1(c);
              digitalWrite(latchPin, LOW);
              
              //shiftOut(dataPin, clockPin, MSBFIRST, (a[c] >> k) );//a[c] bitini k kadar saga kayd�r
              shiftOut(dataPin, clockPin, MSBFIRST, highByte( numbers[t][c] << r )  ) ;
              resett();
              
              shiftOut(dataPin, clockPin, MSBFIRST, lowByte( numbers[t][c] << r ) );
              digitalWrite(latchPin, HIGH);
              
                    }   
			   }   
			}  
		}   
	}

    if (shiftTyp3 == true) 
	{
      for (byte t = 1; t <= p; t++)          // x degeri secme
	  { 
        for (byte r = 0; r <= 9; r++)        //kayd�rma
		{
		
          for (byte d = 0; d <= 4 ; d++)      //bekletme
		  { 
            for (byte c = 0; c <= 8; c++)     //sat�r aktif
			{
				
              activateLineFunc1(c);
              digitalWrite(latchPin, LOW);
              
              shiftOut(dataPin, clockPin, MSBFIRST, highByte( numbers[t][c] >> r )  ) ;
              resett();
              
              shiftOut(dataPin, clockPin, MSBFIRST, lowByte( numbers[t][c] >> r ) );
              digitalWrite(latchPin, HIGH);
              
             }
          }      
		}   
	}

    if (shiftTyp1 == true) 
	{
      for (int y = 1; y <= p - 1; y++) 
	  {
        for (int r = 0; r <= 8; r++)         //kayd�rma
		{
          for (int d = 0; d <= 2; d++)      //bekletme
		  {
            for (int c = 0; c <= 8; c++)   //sat�r aktif
			{

              activateLineFunc1(c);
              
              digitalWrite(latchPin, LOW);
              shiftOut(dataPin, clockPin, MSBFIRST, highByte(numbers[y][c] << r )  ) ;
              
              resett();
              
              shiftOut(dataPin, clockPin, MSBFIRST, lowByte( numbers[y][c] << r ) );
              digitalWrite(latchPin, HIGH);
              
              activateLineFunc2(c);
              
              digitalWrite(latchPin, LOW);
              shiftOut(dataPin, clockPin, MSBFIRST, highByte(numbers[y + 1][c] >> 8 - r )  ) ;
              
              resett();
              
              shiftOut(dataPin, clockPin, MSBFIRST, lowByte( numbers[y + 1][c] >> 8 - r) );
              digitalWrite(latchPin, HIGH);
              
            }
          }
        }
      }
    }
  }


  if (alfabe == true)
  {
    for (int e = 0; e <= 37; e++)       //kayd�rma
	{
      for (int d = 0; d <= 20; d++)      //bekletme
	  {
        for (int c = 0; c <= 8; c++)     //sat�r aktif
		{
			
          activateLineFunc1(c);
          
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, highByte(characters[e][c] )  ) ;
          
          resett();
          
          shiftOut(dataPin, clockPin, MSBFIRST, lowByte( characters[e][c] ) );
          digitalWrite(latchPin, HIGH);

        }
      }

    }
  } 

  shiftTyp1 = false;
  shiftTyp2 = false;
  shiftTyp3 = false;
}
  for (byte i = 0; i <= 10; i++) 
  {
    for (byte t = 0; t <= 10; t++)
	{
      numbers[i][t] = "";
    }
  }
  
  
}

void resett()
{
	
  digitalWrite(resetPin, LOW);
  delay(1);
  digitalWrite(resetPin, HIGH);
  
}

void temizle()
 {
  for (byte c = 0; c <= 8; c++)   //sat�r aktif
   {
   	
    activateLineFunc1(c);
    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, highByte(  0   )) ;
    
    resett();
    
    shiftOut(dataPin, clockPin, MSBFIRST, lowByte( 0 ));
    digitalWrite(latchPin, HIGH);

  }
}











void activateLineFunc1(short x) 
{
  switch ( x ) {
    case 0 :
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 1:
      firstPin  = 1;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 2:
      firstPin  = 1;
      secondPin = 1;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 3:
      firstPin  = 0;
      secondPin = 1;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 4:
      firstPin  = 0;
      secondPin = 1;
      thirdPin  = 1;
      fourthPin = 0;
      break;
    case 5:
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 1;
      fourthPin = 0;
      break;
    case 6:
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 1;
      fourthPin = 1;
      break;
    case 7:
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 1;
      break;
    case 8:
      firstPin  = 1;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 1;
      break;
      //    case 9:
      //      firstPin  = 1;
      //      secondPin = 0;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 10:
      //      firstPin  = 0;
      //      secondPin = 1;
      //      thirdPin  = 0;
      //      fourthPin = 1;
      //      break;
      //    case 11:
      //      firstPin  = 1;
      //      secondPin = 1;
      //      thirdPin  = 0;
      //      fourthPin = 1;
      //      break;
      //    case 12:
      //      firstPin  = 0;
      //      secondPin = 0;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 13:
      //      firstPin  = 1;
      //      secondPin = 0;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 14:
      //      firstPin  = 0;
      //      secondPin = 1;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 15:
      //      firstPin  = 1;
      //      secondPin = 1;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;

  }

  digitalWrite(2, firstPin );
  digitalWrite(3, secondPin);
  digitalWrite(4, thirdPin );
  digitalWrite(5, fourthPin);
}

// Grey Coding for fast bit changing
void activateLineFunc2(short x)
 {
  
  switch ( x ) {
    case 8 :
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 0:
      firstPin  = 1;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 1:
      firstPin  = 1;
      secondPin = 1;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 2:
      firstPin  = 0;
      secondPin = 1;
      thirdPin  = 0;
      fourthPin = 0;
      break;
    case 3:
      firstPin  = 0;
      secondPin = 1;
      thirdPin  = 1;
      fourthPin = 0;
      break;
    case 4:
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 1;
      fourthPin = 0;
      break;
    case 5:
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 1;
      fourthPin = 1;
      break;
    case 6:
      firstPin  = 0;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 1;
      break;
    case 7:
      firstPin  = 1;
      secondPin = 0;
      thirdPin  = 0;
      fourthPin = 1;
      break;
      //    case 9:
      //      firstPin  = 1;
      //      secondPin = 0;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 10:
      //      firstPin  = 0;
      //      secondPin = 1;
      //      thirdPin  = 0;
      //      fourthPin = 1;
      //      break;
      //    case 11:
      //      firstPin  = 1;
      //      secondPin = 1;
      //      thirdPin  = 0;
      //      fourthPin = 1;
      //      break;
      //    case 12:
      //      firstPin  = 0;
      //      secondPin = 0;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 13:
      //      firstPin  = 1;
      //      secondPin = 0;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 14:
      //      firstPin  = 0;
      //      secondPin = 1;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;
      //    case 15:
      //      firstPin  = 1;
      //      secondPin = 1;
      //      thirdPin  = 1;
      //      fourthPin = 1;
      //      break;

  }

  digitalWrite(2, firstPin );
  digitalWrite(3, secondPin);
  digitalWrite(4, thirdPin );
  digitalWrite(5, fourthPin);  
  
   }

