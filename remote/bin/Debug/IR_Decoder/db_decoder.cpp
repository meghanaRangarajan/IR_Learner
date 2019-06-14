#include <pigpio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iterator>
#include <vector> 
#include <list> 
#include <utility> 
#include <sqlite3.h>
#include "cpp_code.h"
#include "constants.h"


uint32_t frequency;
uint32_t result;
std::list <std::pair<int,uint32_t>> frames ;
int pin_callback = 0;//return variable for pigpio callback funtion
int SENSOR_PIN = 0;
int protocol_id;

extern "C"
{

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   
   return 0;
}
static int callback1(void *data, int argc, char **argv, char **azColName){
   int i;

   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int create_insert(int key,uint32_t code)
{

std::string table_name = protocol[protocol_id];

	sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "";

   /* Open database */
   rc = sqlite3_open("remotes.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stdout, "Opened database successfully\n");
   }

   /* Create SQL statement */
   std::string create = "CREATE TABLE "+ table_name+" (ID INT NOT NULL,CODE STRING NOT NULL,P_ID INT);";
   sql = (char*) create.c_str();

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      sqlite3_free(zErrMsg);
   } 
   std::stringstream srs,srs1,srs2;
   srs<<key;
   std::string s2 = srs.str();
   srs1<<std::hex<<code;
   std::string s3 = srs1.str();
   srs2<<protocol_id;
   std::string s4 = srs2.str();
   
   std::string del = "DELETE from "+ table_name +" where ID=" + s2 +"; " +
         "SELECT * from "+ table_name +"";
         sql = (char*) del.c_str();
         

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   std::string insert = "INSERT INTO "+table_name+" (ID,CODE,P_ID) VALUES ("+ s2 + ",'0x" + s3 + "',"+s4+")";;
   sql = (char*) insert.c_str();

	
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
   /*std::string select = "SELECT * from "+table_name+"";
   sql = (char*) select.c_str();

   // Execute SQL statement 
   rc = sqlite3_exec(db, sql, callback1, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
   }*/
   
   sqlite3_close(db);
   return rc;
}
void init(int pin)
{
	pin_callback = 0;
	SENSOR_PIN = pin;
	result = 0;
	
}
    uint32_t code()
    {
        int b;
        do
        {    
        	int a = gpioInitialise();
        if(a<0)
        {
            std::cout<<"cant initialize\n";
            return 0;
        }

        int pin = 4;
        init(pin);
        enable();    
            b = analyse_ir_pulses();

        }while(b==0);
        std::cout<<"\n";
        std::cout<<"The frequency = "<<(frequency/1000.00)<<"KHz"<<std::endl;
        std::cout<<"The code = "<<std::hex<<"0x"<<result<<std::dec<<std::endl;
        std::cout<<"___________________________________________________________________________"<<std::endl;
	gpioTerminate();
	
	
        return result;
    }

	void pin_callback_entry(int gpio, int level, uint32_t tick)
     {
     
        int length = frames.size();
        if (level == 2)
        {
            if(length<5) 
            {
                std::cout<<"Disgarding\n"<<std::endl;
                gpioSetWatchdog(SENSOR_PIN, 0);
                frames.clear();
                return;
            }
            analyse_ir_pulses();
            disable();
            return;      
        }

        if (length == 0)
        {
            gpioSetWatchdog(SENSOR_PIN, 50);
        }   

        if (length == 1)
        {
            gpioSetWatchdog(SENSOR_PIN, 50);
        }   


         

        frames.push_back(std::make_pair(level,tick));     

     }

    void enable()
    {
        if (pin_callback!=0)
            return;
        frames.clear();    
        
        pin_callback = gpioSetAlertFunc(SENSOR_PIN,pin_callback_entry);

     }
 
	void shutdown()
     {
        gpioSetWatchdog(SENSOR_PIN, 0);
        if(pin_callback != 0)
        {
            pin_callback = 0;
        }
     }

     void disable()
     {
        if(pin_callback != 0)
        {
            return;
        }
        gpioSetWatchdog(SENSOR_PIN, 0);
        pin_callback = 0;
     }
     
     std::vector<std::pair<int,int>> quantize(std::vector<std::pair<int,int>> sequence,uint32_t period)
     {
     	std::vector<std::pair<int,int>> summary;
     	summary = sequence;
     	for(int i=0;i<summary.size();i++)
     	{
     		std::pair<int,int> temp = summary.at(i);
     		(summary.at(i)).second = int(round(temp.second/period));     		
     	}
     	
     	return summary;
     }
     
     uint32_t _try_decode_nec(std::vector<std::pair<int,int>> sequence)
     {
     
        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for NEC"<<std::endl;

        /*if (!((header_mark > NEC_HEADER_MARK * 0.50) && (header_mark < NEC_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > NEC_HEADER_SPACE * 0.50) && (header_space < NEC_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > NEC_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > NEC_MARK * 0.50) && (mark < NEC_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > NEC_LOW_SPACE * 0.50)&&(space < NEC_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > NEC_HIGH_SPACE * 0.50)&&(space < NEC_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_rc5(std::vector<std::pair<int,int>> sequence)
     {
         
        std::cout<<"Testing for RC5\n";

        int i=0;
        int x=0;
        std::pair<int,int> value;
        int val; 
        for (int m=0 ; m<sequence.size() ; m++)
        {
            value = sequence.at(m);
            val = value.second;
            if((val > RC5_SLICE * 0.80)&&(val < RC5_SLICE * 1.2))
                    i+=1;
            else if ((val > RC5_SLICE_ * 0.80)&&(val < RC5_SLICE_ * 1.2))
                    i+=2;
            else
            {
                x=m;
                break;
            }                    
        }

        std::vector <int> k;
        
        for (int m=0 ; m<x ; m++)
        {
            value = sequence.at(m);
            val = value.second;
            if((val > RC5_SLICE * 0.80)&&(val < RC5_SLICE * 1.2))
                    k.push_back(32);
            else if ((val > RC5_SLICE_ * 0.80)&&(val < RC5_SLICE_ * 1.2))
            {
                k.push_back(64);
                k.push_back(64);
            }                    
        }

        uint32_t result=0;
        int change=0;
        
        for(auto i = k.begin(); i != k.end(); ++i) 
        std::cout <<"("<< (*i) <<")"<< "\t";
        std::cout<<"\n" ;

        for(i=0;i<(k.size()-2);i+=2) 
        {
            int mark = k.at(i);
            if (mark==64)
            {
                if (change==0)
                {
                    result <<= 1;
                    result |= 1;
                    change=1;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    change=0;
                }                    
            }
                
            else if (mark==32)
            {
                if (change==0)
                {
                    result <<= 1;
                    change=0;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    result |= 1;
                    change=1;
                }                    
            }
        }
            

        if(result == 0)
        {
            return 0;
        }
        return result;
    }


uint32_t _try_decode_rc6(std::vector<std::pair<int,int>> sequence)
     {
 
        std::cout<<"Testing for RC6";

        int i=0;
        int x=0;
        std::pair<int,int> value;
        for (int m=2; m<sequence.size() ; m++)
        {
            value = sequence.at(m);
            int val = value.second;
            if((val > RC6_SLICE * 0.80)&&(val < RC6_SLICE * 1.2))
                    i+=1;
            else if ((val > RC6_SLICE_ * 0.80)&&(val < RC6_SLICE_ * 1.2))
                    i+=2;
            else
            {
                x=m;
                break;
            }                    
        }
                

        std::vector <int> k;
        
        for (int m=2; m<x ; m++)
        {
            value = sequence.at(m);
            int val = value.second;
            if((val > RC6_SLICE * 0.80)&&(val < RC6_SLICE * 1.2))
                    k.push_back(32);
            else if ((val > RC6_SLICE_ * 0.80)&&(val < RC6_SLICE_ * 1.2))
            {
                k.push_back(64);
                k.push_back(64);
            }                    
        }
            

        uint32_t result=0;
        int change=0;

        for(i=0;i<(k.size()-2);i+=2) 
        {
            int mark = k[i];
            if (mark==64)
            {
                if (change==0)
                {
                    result <<= 1;

                    result |= 1;
                    change=1;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    change=0;
                }                    
            }
                
            else if (mark==32)
            {
                if (change==0)
                {
                    result <<= 1;
                    change=0;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    result |= 1;
                    change=1;
                }                    
            }
        }
            

        if(result == 0)
        {
            return 0;
        }
        return result;


     }



uint32_t _try_decode_rcmm(std::vector<std::pair<int,int>> sequence)
     {
     
     
     std::cout<<"\ninside decode\n";


        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"header mark and header spacce = "<<header_mark<<" and "<<header_space<<std::endl;
        std::cout<<"Testing for RCMM"<<std::endl;

        if (!((header_mark > RCMM_HEADER_MARK * 0.80) && (header_mark < RCMM_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > RCMM_HEADER_SPACE * 0.80) && (header_space < RCMM_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > RCMM_SPACE3 * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > RCMM_MARK * 0.80)&& (mark < RCMM_MARK * 1.20)))
                {
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > RCMM_SPACE0 * 0.80)&&(space < RCMM_SPACE0 * 1.20))
            {
                result <<= 1;
                result <<= 1;
            }
                

            else if ((space > RCMM_SPACE1 * 0.80)&&(space < RCMM_SPACE1 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result |= 1;
            }
                                

            else if ((space > RCMM_SPACE2 * 0.80)&&(space < RCMM_SPACE2 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
            }
                

            else if ((space > RCMM_SPACE3 * 0.80)&&(space < RCMM_SPACE3 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;

            }
                

            else
            {
               
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
}


uint32_t _try_decode_xmp1(std::vector<std::pair<int,int>> sequence)
     {
     
     
     std::cout<<"\ninside decode\n";

        std::cout<<"Testing for XMP1"<<std::endl;
        std::pair<int,int> header ;

        uint32_t result = 0;

        for(int k=0;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if ((space > XMP1_SPACE15 * 2)||(space>XMP1_GAP))
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }

            if((k%16==0)&&(k!=0))
                continue;
                

            if (!((mark > XMP1_MARK * 0.80)&& (mark < XMP1_MARK * 1.20)))
                {
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > XMP1_SPACE0 * 0.80)&&(space < XMP1_SPACE0 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result <<= 1;
                result <<= 1;
            }
                

            else if ((space > XMP1_SPACE1 * 0.80)&&(space < XMP1_SPACE1 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result <<= 1;
                result <<= 1;
                result |= 1;
            }
                                

            else if ((space > XMP1_SPACE2 * 0.80)&&(space < XMP1_SPACE2 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
            }
                

            else if ((space > XMP1_SPACE3 * 0.80)&&(space < XMP1_SPACE3 * 1.20))
            {
                result <<= 1; 
                result <<= 1;  
                result <<= 1;
                result |= 1;  
                result <<= 1;
                result |= 1;          

            }

            else if ((space > XMP1_SPACE4 * 0.80)&&(space < XMP1_SPACE4 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;
            }
                

            else if ((space > XMP1_SPACE5 * 0.80)&&(space < XMP1_SPACE5 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;
                result |= 1;
            }
                                

            else if ((space > XMP1_SPACE6 * 0.80)&&(space < XMP1_SPACE6 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
            }
                

            else if ((space > XMP1_SPACE7 * 0.80)&&(space < XMP1_SPACE7 * 1.20))
            {
                result <<= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;

            }

            else if ((space > XMP1_SPACE8 * 0.80)&&(space < XMP1_SPACE8 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;
                result <<= 1;
            }
                

            else if ((space > XMP1_SPACE9 * 0.80)&&(space < XMP1_SPACE9 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;
                result <<= 1;
                result |= 1;
            }
                                

            else if ((space > XMP1_SPACE10 * 0.80)&&(space < XMP1_SPACE10 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
            }
                

            else if ((space > XMP1_SPACE11 * 0.80)&&(space < XMP1_SPACE11 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;

            }

            else if ((space > XMP1_SPACE12 * 0.80)&&(space < XMP1_SPACE12 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;

            }
                

            else if ((space > XMP1_SPACE13 * 0.80)&&(space < XMP1_SPACE13 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result <<= 1;
                result |= 1;
            }
                                

            else if ((space > XMP1_SPACE14 * 0.80)&&(space < XMP1_SPACE14 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
            }
                

            else if ((space > XMP1_SPACE15 * 0.80)&&(space < XMP1_SPACE15 * 1.20))
            {
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;
                result <<= 1;
                result |= 1;

            }
                

            else
            {
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_rc5_57(std::vector<std::pair<int,int>> sequence)
     {
     
         
        std::cout<<"Testing for RC5_57";

        int i=0;
        int x=0;
        std::pair<int,int> value;
        for (int m ; m<sequence.size() ; m++)
        {
            value = sequence.at(m);
            int val = value.second;
            if((val > RC5_57_SLICE * 0.80)&&(val < RC5_57_SLICE * 1.2))
                    i+=1;
            else if ((val > RC5_57_SLICE_ * 0.80)&&(val < RC5_57_SLICE_ * 1.2))
                    i+=2;
            else
            {
                x=m;
                break;
            }                    
        }
                

        std::vector <int> k;
        if(x<6)
        
                return 0;
        
        for (int m ; m<x ; m++)
        {
            value = sequence.at(m);
            int val = value.second;
            if((val > RC5_57_SLICE * 0.80)&&(val < RC5_57_SLICE * 1.2))
                    k.push_back(52);
            else if ((val > RC5_57_SLICE_ * 0.80)&&(val < RC5_57_SLICE_ * 1.2))
            {
                k.push_back(104);
                k.push_back(104);
            }                    
        }
            

        uint32_t result=0;
        int change=0;

        for(i=6;i<k.size();i+=2) 
        {
            int mark = k[i];
            if (mark==104)
            {
                if (change==0)
                {
                    result <<= 1;
                    result |= 1;
                    change=1;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    change=0;
                }                    
            }
                
            else if (mark==52)
            {
                if (change==0)
                {
                    result <<= 1;
                    change=0;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    result |= 1;
                    change=1;
                }                    
            }
        }
            

        if(result == 0)
        {
            return 0;
        }
        return result;
     }


 uint32_t _try_decode_nec_short(std::vector<std::pair<int,int>> sequence)
     {

        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for NEC"<<std::endl;

        if (!((header_mark > NEC_SHORT_HEADER_MARK * 0.80) && (header_mark < NEC_SHORT_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > NEC_SHORT_HEADER_SPACE * 0.80) && (header_space < NEC_SHORT_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > NEC_SHORT_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > NEC_SHORT_MARK * 0.80) && (mark < NEC_SHORT_MARK * 1.20)))
                {
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > NEC_SHORT_LOW_SPACE * 0.80)&&(space < NEC_SHORT_LOW_SPACE * 1.20))
            {
                result <<= 1;
            }
                

            else if ((space > NEC_SHORT_HIGH_SPACE * 0.80)&&(space < NEC_SHORT_HIGH_SPACE* 1.20))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }

uint32_t _try_decode_sony(std::vector<std::pair<int,int>> sequence)
     {        

        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for sony"<<std::endl;

        /*if (!((header_mark > SONY_HEADER_MARK * 0.80) && (header_mark < SONY_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > SONY_HEADER_SPACE * 0.80) && (header_space < SONY_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (mark > SONY_HIGH_MARK * 2)
            {
                std::cout<<"mark greater than HIGH_MARK detected\n";
                break;
            }
                

            if (!((space > SONY_SPACE * 0.80) && (space < SONY_SPACE * 1.20)))
                {
                std::cout<<"Bad space Detected\n";
                result = 0;
                break;
            }

            if ((mark > SONY_LOW_MARK * 0.80)&&(mark < SONY_LOW_MARK * 1.20))
            {
                result <<= 1;
            }
                

            else if ((mark > SONY_HIGH_MARK * 0.80)&&(mark < SONY_HIGH_MARK* 1.20))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                std::cout<<"Bad mark Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_panasonic(std::vector<std::pair<int,int>> sequence)
     {
        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for Panasonic"<<std::endl;

        /*if (!((header_mark > PANA_HEADER_MARK * 0.50) && (header_mark < PANA_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > PANA_HEADER_SPACE * 0.50) && (header_space < PANA_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > PANA_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > PANA_MARK * 0.50) && (mark < PANA_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > PANA_LOW_SPACE * 0.50)&&(space < PANA_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > PANA_HIGH_SPACE * 0.50)&&(space < PANA_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_jvc(std::vector<std::pair<int,int>> sequence)
     {
        
        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for JVC"<<std::endl;

        /*if (!((header_mark > JVC_HEADER_MARK * 0.50) && (header_mark < JVC_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > JVC_HEADER_SPACE * 0.50) && (header_space < JVC_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > JVC_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > JVC_MARK * 0.50) && (mark < JVC_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > JVC_LOW_SPACE * 0.50)&&(space < JVC_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > JVC_HIGH_SPACE * 0.50)&&(space < JVC_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_rc5_38(std::vector<std::pair<int,int>> sequence)
     {
         
        std::cout<<"Testing for RC5_38";

        int i=0; 
        int x=0;
        std::pair<int,int> value;
        for (int m ; m<sequence.size() ; m++)
        {
            value = sequence.at(m);
            int val = value.second;
            if((val > RC5_38_SLICE * 0.80)&&(val < RC5_38_SLICE * 1.2))
                    i+=1;
            else if ((val > RC5_38_SLICE_ * 0.80)&&(val < RC5_38_SLICE_ * 1.2))
                    i+=2;
            else
            {
                x=m;
                break;
            }                    
        }
                

        std::vector <int> k;
        if(x<6)
                return 0;
        
        for (int m ; m<x ; m++)
        {
            value = sequence.at(m);
            int val = value.second;
            if((val > RC5_38_SLICE * 0.80)&&(val < RC5_38_SLICE * 1.2))
                    k.push_back(32);
            else if ((val > RC5_38_SLICE_ * 0.80)&&(val < RC5_38_SLICE_ * 1.2))
            {
                k.push_back(64);
                k.push_back(64);
            }                    
        }
            

        uint32_t result=0;
        int change=0;

        for(i=6;i<k.size();i+=2) 
        {
            int mark = k[i];
            if (mark==64)
            {
                if (change==0)
                {
                    result <<= 1;
                    result |= 1;
                    change=1;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    change=0;
                }                    
            }
                
            else if (mark==32)
            {
                if (change==0)
                {
                    result <<= 1;
                    change=0;
                }                    
                else if (change==1)
                {
                    result <<= 1;
                    result |= 1;
                    change=1;
                }                    
            }
        }
            

        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_sharp(std::vector<std::pair<int,int>> sequence)
     {
        
        std::cout<<"Testing for SHARP"<<std::endl;
        //No header
        std::pair<int,int> header ;

        uint32_t result = 0;

        for(int k=0;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > SHARP_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > SHARP_MARK * 0.50) && (mark < SHARP_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > SHARP_LOW_SPACE * 0.50)&&(space < SHARP_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > SHARP_HIGH_SPACE * 0.50)&&(space < SHARP_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_rca_38(std::vector<std::pair<int,int>> sequence)
     {
        
        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for RCA_38"<<std::endl;

        /*if (!((header_mark > RCA38_HEADER_MARK * 0.50) && (header_mark < RCA38_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > RCA38_HEADER_SPACE * 0.50) && (header_space < RCA38_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > RCA38_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > RCA38_MARK * 0.50) && (mark < RCA38_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > RCA38_LOW_SPACE * 0.50)&&(space < RCA38_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > RCA38_HIGH_SPACE * 0.50)&&(space < RCA38_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }


uint32_t _try_decode_rca_57(std::vector<std::pair<int,int>> sequence)
     {        
        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for RCA_57"<<std::endl;

        /*if (!((header_mark > RCA57_HEADER_MARK * 0.50) && (header_mark < RCA57_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > RCA57_HEADER_SPACE * 0.50) && (header_space < RCA57_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > RCA57_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > RCA57_MARK * 0.50) && (mark < RCA57_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > RCA57_LOW_SPACE * 0.50)&&(space < RCA57_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > RCA57_HIGH_SPACE * 0.50)&&(space < RCA57_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }

uint32_t _try_decode_mitsubishi(std::vector<std::pair<int,int>> sequence)
     {
        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for MITSUBISHI"<<std::endl;

        /*if (!((header_mark > MITSUBISHI_HEADER_MARK * 0.50) && (header_mark < MITSUBISHI_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > MITSUBISHI_HEADER_SPACE * 0.50) && (header_space < MITSUBISHI_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > MITSUBISHI_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > MITSUBISHI_MARK * 0.50) && (mark < MITSUBISHI_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
                }

            if ((space > MITSUBISHI_LOW_SPACE * 0.50)&&(space < MITSUBISHI_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > MITSUBISHI_HIGH_SPACE * 0.50)&&(space < MITSUBISHI_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;
     }

uint32_t _try_decode_konka(std::vector<std::pair<int,int>> sequence)
     {        
        std::pair<int,int> header = sequence.at(0);
        int header_mark = header.second;
        header = sequence.at(1);
        int header_space = header.second;
        std::cout<<"Testing for JVC"<<std::endl;

        /*if (!((header_mark > KONKA_HEADER_MARK * 0.50) && (header_mark < KONKA_HEADER_MARK * 1.20)))
            {
                std::cout<<"Header Mark Failed to match : " << header_mark<<std::endl;
                return 0;
            }
        if (!((header_space > KONKA_HEADER_SPACE * 0.50) && (header_space < KONKA_HEADER_SPACE * 1.20)))
            {
                std::cout<<"Header Space Failed to match: " << header_space<<std::endl;
                return 0;
            }*/

        uint32_t result = 0;

        for(int k=2;k<(sequence.size());k=k+2)
        {
            header = sequence.at(k);
            int mark = header.second;
            header = sequence.at(k+1);
            int space = header.second;

            if (space > KONKA_HIGH_SPACE * 2)
            {
                std::cout<<"Space greater than HIGH_SPACE detected\n";
                break;
            }
                

            if (!((mark > KONKA_MARK * 0.50) && (mark < KONKA_MARK * 2)))
                {//Bad decode
                std::cout<<"Bad Mark Detected\n";
                result = 0;
                break;
            }

            if ((space > KONKA_LOW_SPACE * 0.50)&&(space < KONKA_LOW_SPACE * 2))
            {
                result <<= 1;
            }
                

            else if ((space > KONKA_HIGH_SPACE * 0.50)&&(space < KONKA_HIGH_SPACE* 2))
            {
                result <<= 1;
                result |= 1;
            }

            else
            {
                //Bad decode.
                std::cout<<"Bad Space Detected\n";
                result = 0;
                break;
            }              

        }
        if(result == 0)
        {
            return 0;
        }
        return result;


     }
 
     int decode_ir_sequence()
     {        std::list <uint32_t> sequence;
             std::vector<std::pair<int,int>> summary;
        uint32_t first_tick = 0;
	    uint32_t last_tick = 0;
        uint32_t diff = 0; 
        int length = frames.size();
        std::list<std::pair<int,uint32_t>>::iterator itr;
        
        std::cout<<"frames size = "<<frames.size()<<std::endl;
        std::cout<<"sequence size = "<<sequence.size()<<std::endl;
        std::cout<<"summary size = "<<summary.size()<<std::endl;
        
        for (itr = frames.begin(); itr != frames.end(); ++itr)
        {
            int level = itr->first;
            uint32_t tick = itr->second;

            if(level == 0)
            {
                if(sequence.size()==0)
                {
                    sequence.push_back(0);
                    first_tick = tick;
                }
                else
                {
                    diff = tick - first_tick;
                    if(diff<0)
                    {
                        diff = (4294967295 - last_tick) + tick;
                    }
                    
                }
sequence.push_back(diff);
            }
        }
        uint32_t sum_value = 0;
        int num_samples = 0;
        uint32_t last = 0;

        std::list<uint32_t>::iterator it; 
        for(it = sequence.begin(); it != sequence.end(); ++it) 
        {
            uint32_t entry = *it;
            if(entry == 0)
                continue;
            diff = entry - last;
            last = entry;
            if (diff>100 || diff<10)
                continue;
     
            sum_value += diff;
            num_samples += 1;

        }
         
        if(num_samples==0)
        {
            return 0;
        }

        float period = (float)sum_value/num_samples;
        frequency =(uint32_t) 1000000/period;
        

        
        float approximate_period = period * 1.15;

        std::list <uint32_t> differences;
        differences.push_back(0);
        uint32_t previous = sequence.front();
        std::list<uint32_t>::iterator it1; 
        for(it1 = sequence.begin(); it1 != sequence.end(); ++it1)
        {
            differences.push_back(*it1 - previous);
            previous = *it1;
        }

        int current = 1;
        length = 0;
        std::list<uint32_t>::iterator it2; 
        for(it2 = differences.begin(); it2 != differences.end(); ++it2)
        {
            if(*it2 < approximate_period)
            {
                if(current == 1)
                    length += (*it2);//tracking a mark. just add the length.
                else
                {
                    //Tracking a space. Switch to tracking mark
                    length -= int(period);
                    summary.push_back(std::make_pair(0, length));
                    length = *it2;
                    current = 1;
                }
            }
            else
            {
                if(current == 1)
                {
                   //Tracking a mark. Switch to tracking space.
                    length += int(period);
                    summary.push_back(std::make_pair(1, length));
                    length = *it2;
                    current = 0;
                }
                else
                {
                    length += (*it2);                    
                }
            }
        }
        
        
        std::cout<<"frames size = "<<frames.size()<<std::endl;
        std::cout<<"sequence size = "<<sequence.size()<<std::endl;
        std::cout<<"summary size = "<<summary.size()<<std::endl;
        

        summary = quantize(summary,period);
               for(auto i = summary.begin(); i != summary.end(); ++i) 
		std::cout <<"("<< (*i).first << " "<<(*i).second <<")"<< "\t";
		std::cout<<"\n" ;
        if(summary.size () == 0)
        {
        	result =0;
        }
        else
        {
        try
        {
        if((frequency > 38000 * 0.97) && (frequency < 38000 * 1.03))
        {
        	if(result==0){
        	result = _try_decode_nec(summary);
        	protocol_id = protocol_nec;}
        	/*if(result==0){
        	result = _try_decode_nec_short(summary);
        	protocol_id = protocol_nec_short;}
        	if(result==0){
        	result = _try_decode_xmp1(summary);
        	protocol_id = protocol_xmp1;}
        	if(result==0){
        	result = _try_decode_panasonic(summary);
        	protocol_id = protocol_panasonic;}
        	if(result==0){
        	result = _try_decode_jvc(summary);
        	protocol_id = protocol_jvc;}
        	if(result==0){
        	result = _try_decode_sharp(summary);
        	protocol_id = protocol_sharp;}
        	if(result==0){
        	result = _try_decode_rc5_38(summary);
        	protocol_id = protocol_rc5_38;}
        	if(result==0){
        	result = _try_decode_rca_38(summary);
        	protocol_id = protocol_rca_38;}
        	if(result==0){
        	result = _try_decode_mitsubishi(summary);
        	protocol_id = protocol_mitsubishi;}
        	if(result==0){
        	result = _try_decode_konka(summary);
        	protocol_id = protocol_konka;}*/
        	
        }
        else if((frequency > 36000 * 0.80) && (frequency < 36000 * 1.20))
        {
        	if(result==0){
        	result = _try_decode_rcmm(summary);
        	protocol_id = protocol_rcmm;}
        	if(result==0){
        	result = _try_decode_rc5(summary);
        	protocol_id = protocol_rc5;}
        	if(result==0){
        	result = _try_decode_rc6(summary);
        	protocol_id = protocol_rc6;}
        }
        else if((frequency > 57000 * 0.80) && (frequency < 57000 * 1.20))
        {
        	if(result==0){
        	result = _try_decode_rc5_57(summary);
        	protocol_id = protocol_rc5_57;}
        	if(result==0){
        	result = _try_decode_rca_57(summary);
        	protocol_id = protocol_rca_57;}
        }
        else if((frequency > 40000 * 0.80) && (frequency < 40000 * 1.20))
        {
        	result = _try_decode_sony(summary);
        	protocol_id = protocol_sony;
        }
        
        
        }
        catch(std::out_of_range e)
        {
        	result =0;
        }
        }
        std::cout<<"\nThe protocol id is "<<protocol_id<<" and the protocol is "<<protocol[protocol_id]<<std::endl;

        if(result!=0)
        { 
        return 1;
        }
        
        else
        {
        return 0;
        }

     }
     
     
     
 int analyse_ir_pulses()
     {
        int z = decode_ir_sequence();
        frames.clear();
        if(z==0)
        return 0;
        else
        return 1;
     }

}


 