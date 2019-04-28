#include  "bsp_esp32.h"
#include "stm32f1xx_hal.h"

 /*******************
 set wifi  ap
 ********************/ 
 const uint8_t at_command[]="AT\r\n";          //check module  normal
 const uint8_t at_cwmode_command[]="AT+CWMODE=2\r\n";  //AP  server  open wifi  hot pints
 const uint8_t at_rst_command[]="AT+RST\r\n";    //reset valid
 const uint8_t at_cipmux[]="AT+CIPMUX=1\r\n";            //0 : one connection  1: multi connection
 //const uint8_t at_cipservermaxconn[]="AT+CIPSERVERMAXCONN=2\r\n";   //set the maximum number of connections   ,allow two  join in
 const uint8_t at_cipserver[]="AT+CIPSERVER=1,8080\r\n";   // build TCP SERVER  PORT :8080 
// const uint8_t at_cipserver[]="AT+CIPSERVER=1,433,SSL,1";   // build ssL SERVER  PORT :8080

 
 
 
 /********************
 set wifi sta
  *1: send at command
	*2: send at_cwmode command   1 set sta  client mode
	*3: send at_rst    command    reset valid
	*4: send at_cwjap  command    connection tcp server
 *********************/
 
 const uint8_t at_cwjap[]="AT+CWJAP=adc,0123456\r\n";
 
 
 /*********************
     set ble 
		 *1:                        ble  init
		 *2:                        set ble addr
		 *3:                        set ble name
     *4:		                    set  ble param
		 *5:                        
 **********************/
   
 
 const uint8_t at_bleinit[]="AT+BLEINTIT=1\r\n"; //0: logout ble   1: client role  2: server role
 const uint8_t at_bleaddr[]="AT+BLEADDR=1,F8:7F:24:87:1C:F7\r\n"; // 0 :public addr 1:random address     
 const uint8_t at_blename[]="AT+BLENAME=\"esp_demo\"\r\n";   /// set ble name esp_demo
 const uint8_t at_blescanparam[]="AT+BLESCANPARAM=0,0,0,100,50\r\n"; //scan_type 0: PASSIVE SCAN  1:ACTIVE SCAN   addr_type 0:public address  1:random address 2:rpa public address  	 
 const uint8_t at_blescan[]="AT+BLESCAN=1\r\n"; //0: stop scan  1: start scan
// const uint8_t at_blescanrspdata[]="112233445566\r\n"; //AT+BLEINIT=2 init to server,can to use
// const uint8_t at_bleadvparam[]="AT+BLEADVPARAM=50,50,0,0,4,0,0,"12:34:45:78:66:88"; //AT+BLEINIT=2 init to server,can to use
// const uint8_t at_bleadvdata[]="112233445566\r\n";  set ble broadcast data ,init to server,can to use

//const uint8_t at_bleadvstart[]="AT+BLEADVSTART\r\n";   //start  broadcast  init to server,can to use
 const uint8_t at_bleconn[]="AT+BLECONN=024:0a:c4:09:34:23\r\n"; // AT+BLEINIT=1	 set ble mode is client  ,connection is valid;


 
 
 
 
 
    
 
 
  
 