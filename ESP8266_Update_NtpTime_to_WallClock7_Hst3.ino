//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@     UPDATE  NTP  TIME  TO  DIGITAL WALL CLOCK      @@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


//#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   //ESP Web Server Library to host a web page
#include <ESP8266Ping.h>

#include <DS1302.h>
#include <EEPROM.h>


//-------------   For DS1302 Clock Module

// Init the DS1302
DS1302 rtc(D2, D3, D4);         //   PORT Pin
 
// Init a Time-data structure
Time t;

String d_mix = "",d_day="",d_date="",d_month="",d_year="",d_temp="";
String t_mix = "";
String t_hour="",t_minute="",t_second="";
String ip_txt="",ip_txt1,ip_txt2,ip_txt3,ip_txt4;

String min_last="",hour_last="";


int cnt_ds=0;

const String ntp_ip1=""; 
const String ntp_ip2="";

int last_hour_active=0;


//-----------  An Hour History Memorizing

int eeadr;
int cnt_test,cnt_clear,cnt_update;

//-------------------------------   // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  NTP  NTP  NTP  NTP  NTP  @@@@@@@@@@@@@@@@@@@@@@@@  Need to be used for NTP Server Getting Time
//#include <NTPClient.h>
//#include <WiFiUdp.h>
#include <time.h>


const long utcOffsetInSeconds = 25200;
//const long utcOffsetInSeconds = 28800;

// If..
//For UTC -5.00 : -5 * 60 * 60 : -18000
//For UTC +1.00 : 1 * 60 * 60 : 3600
//For UTC +0.00 : 0 * 60 * 60 : 0
// utcOffsetInSeconds = ..

int dst = 0; //กำหนดค่า Date Swing Time
const String month_name[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
const String day_name[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "172.168.1.1", utcOffsetInSeconds);


//NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
//NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);
//NTPClient timeClient(ntpUDP, "north-america.pool.ntp.org", utcOffsetInSeconds);
//NTPClient timeClient(ntpUDP, "oceania.pool.ntp.org", utcOffsetInSeconds);
//NTPClient timeClient(ntpUDP, "south-america.pool.ntp.org", utcOffsetInSeconds);

//------------------------------------------------------------------------------------------------------------


//#include <TridentTD_LineNotify.h>

//---------
#define SERVER_PORT 80            //ค่า port ที่ต้องการเชื่อมต่อเพื่อใช้สื่อสาร 

//WiFiServer server(SERVER_PORT); //สร้าง object server และกำหนด port ที่ต้องการใช้งาน

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//-------- Static IP address configuration กำหนดค่า IP ต่างๆให้กับเครื่อง ถ้าไม่กำหนดจะเป็นอัตโนมัติ

IPAddress staticIP(172,168,16,10); //ESP static ip
//IPAddress staticIP(172,168,1,32); //ESP static ip
IPAddress gateway(172,168,1,254);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255,255,0,0);  //Subnet mask
IPAddress dns(172,168,1,1);  //DNS


/*
IPAddress staticIP(192,168,43,220); //ESP static ip
IPAddress gateway(192,168,43,1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255,255,255,0);  //Subnet mask
IPAddress dns(192,168,43,1);  //DNS
*/
/*
IPAddress staticIP(192,168,1,222); //ESP static ip
IPAddress gateway(192,168,1,1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255,255,255,0);  //Subnet mask
IPAddress dns(192,168,1,1);  //DNS
*/

/*
IPAddress staticIP(172,168,1,69); //ESP static ip
IPAddress gateway(172,168,1,254);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255,255,0,0);  //Subnet mask
IPAddress dns(172,168,1,1);  //DNS
*/

//-----------
const IPAddress remote_ip(172,168,1,1); // Remote host (for ping)
//const IPAddress remote_ip(192,168,43,1); // Remote host (for ping)
//const IPAddress remote_ip(192,168,1,1); // Remote host (for ping)

const char* web_ping = "www.google.co.th"; // Remote host (for ping)

const char* deviceName = "iTlaB";

//const char* password = "p@ssw0rd"; //ค่าของ SSID (อย่าลืมแก้เป็น password ของตัวเอง)

//const char* ssid = "wifiCAn";         //ค่าของ SSID (อย่าลืมแก้เป็น ชื่อ SSID ของตัวเอง)
//const char* password = "abcd1234"; //ค่าของ SSID (อย่าลืมแก้เป็น password ของตัวเอง)

//const char* ssid = "AndroidCAn";         //ค่าของ SSID (อย่าลืมแก้เป็น ชื่อ SSID ของตัวเอง)
//const char* password = "12345678"; //ค่าของ SSID (อย่าลืมแก้เป็น password ของตัวเอง)

const char* ssid = "AP_Ashimori-2.4GHz";         //ค่าของ SSID (อย่าลืมแก้เป็น ชื่อ SSID ของตัวเอง)
//const char* ssid = "AP_MeetingZone";         //ค่าของ SSID (อย่าลืมแก้เป็น ชื่อ SSID ของตัวเอง)
const char* password = "p@ssw0rd"; //ค่าของ SSID (อย่าลืมแก้เป็น password ของตัวเอง)
 
//---------
//On board LED Connected to GPIO2
#define LED 2  

#define o_reset D5  

#define         MINUTE_END        "10"       //  @ The last minute to UPDATE WiFi Time when the HOUR has changed ( ** according to an IP Addess of a WiFi Digital CLOCK)  
                                             //  Ex. #define         MINUTE_END        "06"
                                             //      #define         MINUTE_END        "28" 


#define i_test D9  
#define i_up D7  
#define i_down D6  
#define i_update D1
                                            
 /*
#define i_start D1  
#define i_select D2  
#define i_auto D9  

int in_port = 0;
int out_port = 0;
*/



void(* resetfunction) (void) = 0;  //declare reset function @ address 0
 
//***************************   Our HTML webpage contents in program memory    *************************

/*
const char MAIN_page[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <body>
  <center>
  <h1>WiFi LED on off demo: 1</h1><br>
  Ciclk to turn <a href="ledOn">LED ON</a><br>
  Ciclk to turn <a href="ledOff">LED OFF</a><br>
  <hr>
  <a href="https://circuits4you.com">circuits4you.com</a>
  </center>
  
  </body>
  </html>
)=====";
*/



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*********************************************************************************
//*******************              SUB Routine             ************************
//*********************************************************************************
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// This routine is executed when you open its IP in browser


//*********************************************************************************
//*******************         UPDATE Machine Clock DS1302          ****************
//*********************************************************************************

void update_ds1302()    //   Update Machine Clock with WiFi Date Time
{
    String m_hour,m_minute,m_second;
 
//        Serial.println(" Date and Time Updating Date and Time .....");
 
    
    getting_wifi_date_time();    //  Getting Date(time.h) and Time(timeClient) from NTP Sever 

    rtc.setTime(t_hour.toInt(), t_minute.toInt(), t_second.toInt()); // Set the time (24hr format)
    rtc.setDOW(d_day.toInt()); // Set Day-of-Week (0 to 6)   0 Sunday     6 Saturday
    rtc.setDate(d_date.toInt(), d_month.toInt(), d_year.toInt() + 2000); // Date(1-31) Month(1-12) Year(2000 up) Set the date 

//    rtc.setTime(18, 47, 29); //  18:47:29         Set the time (24hr format)
//    rtc.setDOW(5); //   Friday                Set Day-of-Week (0 to 6)
//    rtc.setDate(4, 12, 2000); //   4 December 2000      Set the date Date(1-31) Month(1-12) Year(2000 up)


    //----------------------   Try to get time from machine
    t = rtc.getTime();

    m_hour = String(t.hour);
    m_minute = String(t.min);
    m_second = String(t.sec);

    if(m_hour.length()==1)
        m_hour = "0" + m_hour;
    if(m_minute.length()==1)
        m_minute = "0" + m_minute;
    if(m_second.length()==1)
        m_second = "0" + m_second;
    
    //------------------------------   Check correct Time : WiFi VS Machine
    if((t_hour!=m_hour)||(t_minute!=m_minute)||(t_second!=m_second))
    {
        cnt_ds=0;  //  Clear
        while (true)
        {

            //  Send Time from WiFi
            Serial.print("date_"+String(d_year.charAt(0))+String(d_year.charAt(1))+"."+String(d_month.charAt(0))+String(d_month.charAt(1))+"."+String(d_date.charAt(0))+String(d_date.charAt(1))+"."+d_day+"_"+
                          String(t_hour.charAt(0))+String(t_hour.charAt(1))+"."+String(t_minute.charAt(0))+String(t_minute.charAt(1))+"."+String(t_second.charAt(0))+String(t_second.charAt(1))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(1000);

            //  Send Time from Machine
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+d_day+"_"+
                          String(m_hour.charAt(0))+String(m_hour.charAt(1))+"."+String(m_minute.charAt(0))+String(m_minute.charAt(1))+"."+String(m_second.charAt(0))+String(m_second.charAt(1))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(1000);

             
            cnt_ds++;
            if(cnt_ds>9)
            {
                resetfunction();   //  Restart ARDUINO from address "0"
    
            }    

        }

      
    }

    //------------------------------



  
}



//*********************************************************************************
//*******************             Getting Date and time From wifi network           ****************
//*********************************************************************************

void getting_wifi_date_time()    //  
{
    re_check_network();
    
    //-------  Getting Now's time  (with timeClient)   

//    timeClient.update();
    
//    t_hour = timeClient.getHours();
//    t_minute = timeClient.getMinutes();
//    t_second = timeClient.getSeconds();

//    if(t_hour.length()==1)
//        t_hour = "0" + t_hour;
//    if(t_minute.length()==1)
//        t_minute = "0" + t_minute;
//    if(t_second.length()==1)
//        t_second = "0" + t_second;


    //-------------------------------------
    cnt_ds=0;  //  Clear
    while (!time(nullptr))
    {

        //  Send "-------dtE^^--"
        Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                      String(char(0x40))+String(char(0x5e))+"."+String(char(0x78))+String(char(0x79))+"."+String(char(0x50))+String(cnt_ds)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
        cnt_ds++;
        if(cnt_ds>9)
        {
            resetfunction();   //  Restart ARDUINO from address "0"
        }    

        delay(500); 

    }

    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);

    //-------   Getting Now's time  (with time.h)
    t_hour = p_tm->tm_hour;
    t_minute = p_tm->tm_min;
    t_second = p_tm->tm_sec;

    if(t_hour.length()==1)
        t_hour = "0" + t_hour;
    if(t_minute.length()==1)
        t_minute = "0" + t_minute;
    if(t_second.length()==1)
        t_second = "0" + t_second;
    

    //-------   Getting Now's date  (with time.h)
    d_day = p_tm->tm_wday;
    d_date = p_tm->tm_mday;
    d_month = p_tm->tm_mon;
    d_month = String(d_month.toInt() + 1);
    d_year = p_tm->tm_year;
    d_year = String(d_year.toInt() + 1900);
    d_year = String(d_year.charAt(d_year.length()-2)) + String(d_year.charAt(d_year.length()-1));

    if(d_date.length()==1)
        d_date = "0" + d_date;
    if(d_month.length()==1)
        d_month = "0" + d_month;
    if(d_year.length()==1)
        d_year = "0" + d_year;
}



//*********************************************************************************
//*******************               Re-Check Network (Connection)            ****************
//*********************************************************************************
void re_check_network(void)
{

//    NTPClient timeClient(ntpUDP, "172.168.1.1", utcOffsetInSeconds);

  
    int cnt_ping_ok = 0,cnt_ping_ng = 0,cnt_ping_ng1 = 0;  

    while(true)
    {
        if(Ping.ping(remote_ip))
        {
            cnt_ping_ng=0;  //  Clear
            cnt_ping_ok++;
            
            if(cnt_ping_ok>=3)
            {
                break;     //  Re-Check Network PINGING PASS!!
            }

        }
        else
        {
            cnt_ping_ok=0;  //  Clear
            cnt_ping_ng++;

            if(cnt_ping_ng>=3)
            {
                //  Send "-------PnG++--"
                Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                              String(char(0x40))+String(char(0x73))+"."+String(char(0x54))+String(char(0x7d))+"."+String(char(0x50))+String(cnt_ping_ng)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));

                delay(1000);
                Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                              String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));

                delay(500);
                 
                if(cnt_ping_ng>9)
                {
                    resetfunction();   //  Restart ARDUINO from address "0"
                }
            }
        }
    }
}




//*********************************************************************************
//*******************             Getting Date and time From Machine           ****************
//*********************************************************************************

void getting_machine_date_time()    //   
{
    int cnt_err_month,cnt_err_day;

    t = rtc.getTime();
/*    
    d_mix = String(rtc.getDOWStr()) + " " + String(t.date) + ", " + String(rtc.getMonthStr()) + " " + String(t.year);
    t_mix = String(t.hour) + ":" + String(t.min) + ":" + String(t.sec);
    Serial.println(d_mix);
    Serial.println(t_mix);
*/

    //---------  Getting Time
    t_hour = String(t.hour);
    t_minute = String(t.min);
    t_second = String(t.sec);

    if(t_hour.length()==1)
        t_hour = "0" + t_hour;
    if(t_minute.length()==1)
        t_minute = "0" + t_minute;
    if(t_second.length()==1)
        t_second = "0" + t_second;

    //------  Get Day of week as a number
    d_day = String(rtc.getDOWStr());

    if(d_day=="Sunday")
        d_day = "0";
    else if(d_day=="Monday")
        d_day = "1";
    else if(d_day=="Tuesday")
        d_day = "2";
    else if(d_day=="Wednesday")  
        d_day = "3";
    else if(d_day=="Thursday")
        d_day = "4";
    else if(d_day=="Friday")
        d_day = "5";
    else if(d_day=="Saturday")
        d_day = "6";
    else 
    {
        cnt_err_day = 0;   //  Clear
        
        while(true)
        {
            //  Send "---------"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                          String(char(0x40))+String(char(0x5e))+"."+String(char(0x77))+String(char(0x6e))+"."+String(char(0x79))+String(char(0x50))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(1000); 

            //  Send "--------------"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+"_"+String(char(0x40))+
                       String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(500);  

            cnt_err_day++;
            if(cnt_err_day>=10)
            {
                resetfunction();    //  Restart ARDUINO from address "0"                
            }
            
        }
    }

    //-----------------  
    d_date = String(t.date);

    if(d_date.length()==1)
        d_date = "0" + d_date;

    //-----------------  Get Month as a number
    d_month = String(rtc.getMonthStr());

    if(d_month=="January")
        d_month = "01";
    else if(d_month=="February")
        d_month = "02";
    else if(d_month=="March")
        d_month = "03";
    else if(d_day=="April")  
        d_month = "04";
    else if(d_month=="May")
        d_month = "05";
    else if(d_month=="June")
        d_month = "06";
    else if(d_month=="July")       //  january february march april may june july august september october november december
        d_month = "07";
    else if(d_month=="August")
        d_month = "08";
    else if(d_month=="September")
        d_month = "09";
    else if(d_month=="October")
        d_month = "10";
    else if(d_month=="November")
        d_month = "11";
    else if(d_month=="December")
        d_month = "12";
    else 
    {
        cnt_err_month = 0;   //  Clear

        while(true)
        {
            //  Send "-------monEr--"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                          String(char(0x40))+String(char(0x37))+"."+String(char(0x5c))+String(char(0x54))+"."+String(char(0x79))+String(char(0x50))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(1000); 

            //  Send "--------------"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+"_"+String(char(0x40))+
                       String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(500); 
            
            cnt_err_month++;
            if(cnt_err_month>=10)
            {
                resetfunction();    //  Restart ARDUINO from address "0"                
            }
                   
        }
    }

    
    //-----------------
    d_year = String(t.year);

    d_year = String(d_year.charAt(d_year.length()-2)) + String(d_year.charAt(d_year.length()-1));

    if(d_year.length()==1)
        d_year = "0" + d_year;
    

}


//*********************************************************************************
//*******************                 Showing an Hour History Show            *******************
//*********************************************************************************
void hour_history_show(void)
{
    int wifi_hour1,wifi_hour0,machine_hour1,machine_hour0,date1,date0,ee_data;
    int d0,d1,d2;

    eeadr=EEPROM.read(1000);     //  Read Now's address

       
    //  Send "-------tESt--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x78))+"."+String(char(0x79))+String(char(0x6d))+"."+String(char(0x78))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    delay(1000); 

    //  Send "--------------"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+"_"+String(char(0x40))+
                  String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    delay(500); 

    ee_data = EEPROM.read(eeadr);         //  read WiFi Hour
    wifi_hour0 = ee_data % 10;
    wifi_hour1 = ee_data / 10;
        
    ee_data = EEPROM.read(eeadr + 300);   //  read Machine Hour
    machine_hour0 = ee_data % 10;
    machine_hour1 = ee_data / 10;
        
    ee_data = EEPROM.read(eeadr + 600);   //  read Memorizing Date
    date0 = ee_data % 10;
    date1 = ee_data / 10;

    d2 = eeadr/100;
    d1 = (eeadr%100) / 10;
    d0 = (eeadr%100) % 10;
       
    //-----------  Send to Clock 
    Serial.print("date_"+String(d0)+String(char(0x40))+"."+String(char(0x40))+String(d1)+"."+String(char(0x40))+String(d2)+"."+d_day+"_"+
                  String(wifi_hour1)+String(wifi_hour0)+"."+String(machine_hour1)+String(machine_hour0)+"."+String(date1)+String(date0)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    delay(1000);

    while(true)
    { 
        delay(1);

        //---------------   Check SW Up/Down to watch the history      
        if((!digitalRead(i_up))||(!digitalRead(i_down)))
        {
            if(!digitalRead(i_up))
            {
                eeadr++;

                if(eeadr>299)
                    eeadr=0;
            }
            else if(!digitalRead(i_down))
            {
                if(eeadr<1)
                    eeadr=300;

                eeadr--;
            }

            ee_data = EEPROM.read(eeadr);         //  read WiFi Hour
            delay(5);
            wifi_hour0 = ee_data % 10;
            wifi_hour1 = ee_data / 10;
        
            ee_data = EEPROM.read(eeadr + 300);   //  read Machine Hour
            delay(5);
            machine_hour0 = ee_data % 10;
            machine_hour1 = ee_data / 10;
          
            ee_data = EEPROM.read(eeadr + 600);   //  read Memorizing Date
            delay(5);
            date0 = ee_data % 10;
            date1 = ee_data / 10;
            
            d2 = eeadr/100;
            d1 = (eeadr%100) / 10;
            d0 = (eeadr%100) % 10;

            //-----------  Send to Clock (Update at minite Change ) 
            Serial.print("date_"+String(d0)+String(char(0x40))+"."+String(char(0x40))+String(d1)+"."+String(char(0x40))+String(d2)+"."+d_day+"_"+
                          String(wifi_hour1)+String(wifi_hour0)+"."+String(machine_hour1)+String(machine_hour0)+"."+String(date1)+String(date0)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));

            while((!digitalRead(i_up))||(!digitalRead(i_down)))
            {
                delay(200);
            }
             
        } 
        //---------------   Check SW SET to Exit     
        if(!digitalRead(i_test))
        {

            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x5c))+"."+String(char(0x1c))+String(char(0x78))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
            delay(1000); 
               
            while(!digitalRead(i_test))
            {
                delay(200);
            }

            break;
        }

    }
      
}


//*********************************************************************************
//***        clear History of The Received WiFi and Machine HOUR             ******
//*********************************************************************************
void clear_history()
{
    int lp_ee;

            //  Send "-------CLEAr--"  
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x39))+"."+String(char(0x38))+String(char(0x79))+"."+String(char(0x77))+String(char(0x50))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
            delay(1000); 

            for(lp_ee=0;lp_ee<1001;lp_ee++)
            {
                EEPROM.write(lp_ee,0);  
            }
            EEPROM.commit();    //Store data to EEPROM
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*********************************************************************************
//*******************                   MAIN  SETUP             *******************
//*********************************************************************************
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void setup(void)
{
    // ---- Setup EEPROM

    EEPROM.begin(4096);
    delay(50);

    
    // ---- Setup Monitor
    Serial.begin(115200); Serial.println();
    Serial.println("");
    


    // ---- LINE Vertion
//    Serial.println(LINE.getVersion());

//    ntp_ip1 = chr('1') + chr('7') + chr('2') + chr('.') + chr('1') + chr('1') + 72.168.1.1";
 //  ntp_ip2 = "172.168.1.5";


    //------   Onboard LED port Direction output

    pinMode(LED,OUTPUT);

    pinMode(o_reset,OUTPUT);

    pinMode(i_test,INPUT_PULLUP);
    pinMode(i_up,INPUT_PULLUP);
    pinMode(i_down,INPUT_PULLUP);
    pinMode(i_update,INPUT_PULLUP);


/*
    pinMode(o_led1,OUTPUT);
    pinMode(o_led2,OUTPUT);
    pinMode(o_led3,OUTPUT);
    pinMode(o_led4,OUTPUT);
    pinMode(o_led5,OUTPUT);
  
           //    pinMode(D0,INPUT_PULLUP);

    
    pinMode(i_start,INPUT_PULLUP);
    pinMode(i_select,INPUT_PULLUP);
    pinMode(i_auto,INPUT_PULLUP);
*/

    digitalWrite(o_reset,HIGH);    //----  On Reset
    digitalWrite(LED,LOW);    //----  On LED
    delay(500);

//    pinMode(D0,INPUT_PULLUP);


    //**********************   Setup for the DS1302 Clock Module  **************************   
    d_year="87"; 
    d_month="12";
    d_date="29";
    d_day="2";           //  d_day 0 = Sunday
    d_temp="33";

    cnt_ds = 0;

    delay(1000);
    digitalWrite(o_reset,LOW);    //----  Off Reset
    
    delay(3000);      //  Wait for Display getting ready
    
    
    while(1)
    {
        //-- Set the clock to run-mode, and disable the write protection
        rtc.halt(false);  
        rtc.writeProtect(false);
 
        //-- Get data from the DS1302
        t = rtc.getTime();
 
        if(t.hour > 23)  // Check correct for the responding from DS1302
        {

            //----------  Error Monitor Send "-------rtcEr--"

            //  Send "-------rtcEr--"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                          String(char(0x40))+String(char(0x50))+"."+String(char(0x78))+String(char(0x58))+"."+String(char(0x79))+String(char(0x50))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(1000); 

            //  Send "--------------"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+"_"+String(char(0x40))+
                       String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(500);        


        }
        else
        {
            //----------  Okay Monitor Send "-------rtcEr--"

            //  Send "-------rtcGd--"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                          String(char(0x40))+String(char(0x50))+"."+String(char(0x78))+String(char(0x58))+"."+String(char(0x7d))+String(char(0x5e))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(500);
            //  Send "-------rtcGd--"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                          String(char(0x40))+String(char(0x50))+"."+String(char(0x78))+String(char(0x58))+"."+String(char(0x7d))+String(char(0x5e))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(500);

            //  Send "--------------"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+"_"+String(char(0x40))+
                       String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(500);        

            //  Send "-------rtcGd--"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                          String(char(0x40))+String(char(0x50))+"."+String(char(0x78))+String(char(0x58))+"."+String(char(0x7d))+String(char(0x5e))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(1000);
            //  Send "--------------"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+"_"+String(char(0x40))+
                       String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(500);        

            break; 

        }
    }    
    
    //***********************************************



     
    //****************************  Config Parameter for WiFi and Connect  ********************************

    WiFi.config(staticIP, dns, gateway, subnet);           //  Un-Comment to use Static IP  ****!!!!! IMPORTANT !!!!!****
    delay(500);

    WiFi.hostname(deviceName);      // DHCP Hostname (useful for finding device for static lease)
    //  WiFi.mode(WIFI_AP); //WiFi mode station (wifi Access Point
    WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only

    WiFi.begin(ssid, password);     // Try Connect to your WiFi router
    digitalWrite(LED,HIGH);    //----  Off LED
    delay(1000);

    WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
    digitalWrite(LED,LOW);    //----  On LED
    delay(1000);
  
    WiFi.begin(ssid, password);                //------  Start WiFi WiFi
    digitalWrite(LED,HIGH);    //----  Off LED
    delay(1000);

    digitalWrite(LED,LOW);    //----  On LED
  
    // --------->>>  Wait for connection  1ST
    //  Send "-------AtF^^--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x77))+"."+String(char(0x78))+String(char(0x71))+"."+String(char(0x60))+String(char(0x42))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
    delay(1000); 

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

    //  Send "-------AtF++--"
        Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                      String(char(0x40))+String(char(0x77))+"."+String(char(0x78))+String(char(0x71))+"."+String(char(0x50))+String(cnt_ds)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
        cnt_ds++;
        if(cnt_ds>9)
            cnt_ds=0;      
    }
    
    delay(500);
    //  Send "-------utF++--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x1c))+"."+String(char(0x78))+String(char(0x71))+"."+String(char(0x60))+String(cnt_ds)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
                   
    delay(2000);        
    
    // If connection successful show IP address in serial monitor

/*    
    Serial.println("");
    Serial.println("");
    Serial.println("----- Connected to WiFi 1st ------  with default parameter");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
    Serial.print("IP gateway: ");
    Serial.println(WiFi.gatewayIP().toString());
    Serial.print("SubnetMask: ");
    Serial.println(WiFi.subnetMask().toString());

    delay(1000);    
*/
    // --------->>>  Wait for connection 2ND Recheck connecting to WiFi again
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        
        //  Send "-------AtF++--"
        Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                      String(char(0x40))+String(char(0x77))+"."+String(char(0x78))+String(char(0x71))+"."+String(char(0x50))+String(cnt_ds)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
        cnt_ds++;
        if(cnt_ds>9)
            cnt_ds=0;      

    }
    
    //  Send "-------utFGd--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x1c))+"."+String(char(0x78))+String(char(0x71))+"."+String(char(0x7d))+String(char(0x5e))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
    delay(2000);
    
/*    
    // If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.println("----- Connected to WiFi 2nd ------  with default parameter");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
    Serial.print("IP gateway: ");
    Serial.println(WiFi.gatewayIP().toString());
    Serial.print("SubnetMask: ");
    Serial.println(WiFi.subnetMask().toString());

    // print the received signal strength:
    Serial.println("");
    Serial.println("");
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    Serial.println("");
    Serial.println("");

*/
    //***************************************  Test Ping to LAN IP  *******************************************
/*    
    digitalWrite(LED,HIGH);  //  Off LED
    delay(100);
    digitalWrite(LED,LOW);  //  On LED
    delay(100);
    digitalWrite(LED,HIGH);  //  Off LED
    delay(100);
    digitalWrite(LED,LOW);  //  On LED
    delay(100);
    digitalWrite(LED,HIGH);  //  Off LED
    delay(100);
    digitalWrite(LED,LOW);  //  On LED
    delay(100);

    Serial.println("");
    Serial.println("");
    Serial.println("---- Test Ping to LAN IP ----");
    Serial.println("Test ping to : " + String(remote_ip[0]) + "." + String(remote_ip[1]) + "." + String(remote_ip[2]) + "." + String(remote_ip[3]) + ".. ");
*/
    
    //  Send "-------PnG^^--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x73))+"."+String(char(0x54))+String(char(0x7d))+"."+String(char(0x60))+String(char(0x42))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
    delay(1000); 

    cnt_ds=0;  //  Clear
    while (!Ping.ping(remote_ip))
    {
        digitalWrite(LED,LOW);  //  On LED
        delay(500);

        //  Send "-------PnG++--"
        Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                      String(char(0x40))+String(char(0x73))+"."+String(char(0x54))+String(char(0x7d))+"."+String(char(0x50))+String(cnt_ds)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
        cnt_ds++;
        if(cnt_ds>9)
            cnt_ds=0;      

        digitalWrite(LED,HIGH);  //  Off LED
        delay(100);
    }
    
    digitalWrite(LED,HIGH);  //  Off LED
    delay(500);

    //  Send "-------PnGGd--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x73))+"."+String(char(0x54))+String(char(0x7d))+"."+String(char(0x7d))+String(char(0x5e))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
    delay(1000); 


    //-----  Title
    
//    Serial.println("++++  TCP Server  ++++");
//    Serial.print("IP address: ");
//    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//    Serial.print("Server port No. ");
//    Serial.println(SERVER_PORT);
//    Serial.println();
//    Serial.println();

    //  Send "-------Pxxxx--"    (IP: xx.xx)  <172.168.xx.xx>

    
    if(String(WiFi.localIP()[2]).length()<2)
    {
        ip_txt1="0";
        ip_txt2=String(WiFi.localIP()[2]);
    }
    else if(String(WiFi.localIP()[2]).length()==2)
    {
        ip_txt1=String(WiFi.localIP()[2]).substring(0,1);
        ip_txt2=String(WiFi.localIP()[2]).substring(1);
    }
    else
    {
        ip_txt1=String(char(0x40));
        ip_txt2=String(char(0x40));
    }

    //--

    if(String(WiFi.localIP()[3]).length()<2)
    {
        ip_txt3="0";
        ip_txt4=String(WiFi.localIP()[3]);
    }
    else if(String(WiFi.localIP()[3]).length()==2)
    {
        ip_txt3=String(WiFi.localIP()[3]).substring(0,1);
        ip_txt4=String(WiFi.localIP()[3]).substring(1);
    }
    else
    {
        ip_txt3=String(char(0x40));
        ip_txt4=String(char(0x40));
    }
       
 /*   
    ip_txt1=String(WiFi.localIP()[2]).substring(0,1);
    ip_txt2=String(WiFi.localIP()[2]).substring(1);
    ip_txt3=String(WiFi.localIP()[3]).substring(0,1);
    ip_txt4=String(WiFi.localIP()[3]).substring(1);
*/
/*
    Serial.println("ip_txt1 = " + ip_txt1);
    Serial.println("ip_txt2 = " + ip_txt2);
    Serial.println("ip_txt3 = " + ip_txt3);
    Serial.println("ip_txt4 = " + ip_txt4);
  */  


    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x73))+"."+ip_txt1+ip_txt2+"."+ip_txt3+ip_txt4+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
    delay(3000); 


    //******************************** เพิ่มเพจเก็บไวเรียกใช้  (สร้าง Events)  ************************************

/*
    server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
    server.on("/ledon", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
    server.on("/ledoff", handleLEDoff);
    server.on("/ledflash", handleLEDflash);
    server.on("/ledblink", handleLEDblink);
    server.on("/value_of_panel", handleXXX);

    server.on("/save", handleSave);
    server.on("/READ", handleReadPanel);
    server.on("/WRITE", handleWritePanel);
*/

    //******************   เปิด Server เริ่มทำงาน   *************** 
    server.begin();                  //Start server
//    Serial.println("HTTP server started");

//    timeClient.begin();   // >>>>  NTP  NTP  NTP  NTP  NTP  @@@@  <Need to be used for NTP Server Getting Time> 
    configTime(utcOffsetInSeconds, dst, "172.168.1.1", "172.168.1.5"); // Get Date and Time from Server <Need to be used for NTP Server Getting Date>

    
    //******************           Check Date Getting System ready (time.h)          *************** 
    
    //  Send "-------dtE^^--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x5e))+"."+String(char(0x78))+String(char(0x79))+"."+String(char(0x60))+String(char(0x42))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
    delay(500); 

    cnt_ds=0;  //  Clear
    while (!time(nullptr))
    {

        //  Send "-------dtE^^--"
        Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                      String(char(0x40))+String(char(0x5e))+"."+String(char(0x78))+String(char(0x79))+"."+String(char(0x50))+String(cnt_ds)+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    
        cnt_ds++;
        if(cnt_ds>9)
            cnt_ds=0;      

        delay(500); 

    }

    
    //  Send "-------dtEGd--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x5e))+"."+String(char(0x78))+String(char(0x79))+"."+String(char(0x7d))+String(char(0x5e))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    delay(1000); 

    //****************************    Update DS1302    ***********************

    //  Send "-----UPdAtE--"
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x1c))+String(char(0x73))+"."+String(char(0x5e))+String(char(0x77))+"."+String(char(0x78))+String(char(0x79))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    delay(1000); 

    update_ds1302();   //   Update Machine Clock with WiFi Date Time

    
    // **********************   Send "-------StArt--"     ****************
    Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                  String(char(0x40))+String(char(0x6d))+"."+String(char(0x78))+String(char(0x77))+"."+String(char(0x50))+String(char(0x78))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
    delay(500); 


    //----------  Preparing for 255 Hours History Memorization
    //
    //      Address  0   to 299  a WiFi Hour Memorizing    (Show on Hour State)
    //      Address  300 to 599  a Machine Hour Memorizing (Show on Minute State) 
    //      Address  600 to 899  A Date Memorizing         (Show on Second State) 
    //      Address  1000 An index number navigator
    //      
    //

    //First time PowerOn Clear
    if((EEPROM.read(0) > 23)||(EEPROM.read(300) > 23))
    {
        clear_history();
    }
 
    //-----------  Send Started Date and Time to Clock 



//   //   getting_wifi_date_time();    //  Getting Date(time.h) and Time(timeClient) from NTP Sever 

//    getting_machine_date_time();   //  Getting Date and Time from Machine    
    
    
//    Serial.print("date_"+String(d_year.charAt(0))+String(d_year.charAt(1))+"."+String(d_month.charAt(0))+String(d_month.charAt(1))+"."+String(d_date.charAt(0))+String(d_date.charAt(1))+"."+d_day+"_"+
//                  String(t_hour.charAt(0))+String(t_hour.charAt(1))+"."+String(t_minute.charAt(0))+String(t_minute.charAt(1))+"."+String(t_second.charAt(0))+String(t_second.charAt(1))+"_"+String(char(0x40))+String(char(0x40))+"_end."+String(char(13)));


}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*********************************************************************************
//*******************                   MAIN  Loop              *******************
//*********************************************************************************
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void loop(void)
{
    delay(50);    // Loop control Time
  
    //****************************

    //---  Check sw to watch the History of Recieved Hour from WiFi and Machine 
    if(!digitalRead(i_test))
    {
        cnt_test++;
        if(cnt_test>50)
        {
            hour_history_show();     
        }
    }
    else
    {
        cnt_test=0; //  Clear
    }

    //---- Check sw to clear History
    
    if((!digitalRead(i_up))&&(!digitalRead(i_down)))
    {
        cnt_clear++;
        if(cnt_clear>60)
        {
            clear_history();   //  Clear an Hour History  
        }
    }
    else
    {
        cnt_clear=0; //  Clear
    }
    
    //---- Check sw to Update Machine Clock with WiFi Date Time

    if(!digitalRead(i_update))
    {
        cnt_update++;
        if(cnt_update>50)
        {
        
            //  Send "-----UPdAtE--"
            Serial.print("date_"+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+String(char(0x40))+String(char(0x40))+"."+"5"+"_"+
                          String(char(0x1c))+String(char(0x73))+"."+String(char(0x5e))+String(char(0x77))+"."+String(char(0x78))+String(char(0x79))+"_"+String(char(0x40))+String(char(0x40))+"_err."+String(char(13)));
            delay(1000); 

            update_ds1302();   //   Update Machine Clock with WiFi Date Time

            //-----------  Send Started Time to Clock (Update at minite Change ) 
            Serial.print("date_"+String(d_year.charAt(0))+String(d_year.charAt(1))+"."+String(d_month.charAt(0))+String(d_month.charAt(1))+"."+String(d_date.charAt(0))+String(d_date.charAt(1))+"."+d_day+"_"+
                          String(t_hour.charAt(0))+String(t_hour.charAt(1))+"."+String(t_minute.charAt(0))+String(t_minute.charAt(1))+"."+String(t_second.charAt(0))+String(t_second.charAt(1))+"_"+String(char(0x40))+String(char(0x40))+"_end."+String(char(13)));
        }
    }        
    else
    {
        cnt_update=0; //  Clear
    }

  
    //****************************

    
    
    getting_machine_date_time();   //  Getting Date and Time from Machine    

    //---------------   Check Hour Change to Update Hour Running for Machine ( WiFiClock -> Machine )
    if(hour_last!=t_hour)
//    if(hour_last!=t_minute)
    {
        last_hour_active = 1;    // Setup indecating an HOUR has been changed to prepare UPDATE WiFi Time!!
        
        hour_last=t_hour;     //    Update last Hour
//        hour_last=t_minute;     //    Update last Hour
    }
   
    //---------------   Check Minute Change to Update Minute Running for Display ( MachineClock -> Display )
    if(min_last!=t_minute)
//    if(min_last!=t_second)
    {
        min_last=t_minute;    //  Update last Minute
//        min_last=t_second;    //  Update last Minute

        if(last_hour_active == 1)
        {

            if(t_minute == MINUTE_END)   //  Check to Update Time with WiFi when A Hour changed and MINUTE_END is matched
//            if(t_second == MINUTE_END)   //  Check to Update Time with WiFi when A Hour changed and MINUTE_END is matched
            {
                last_hour_active = 0;    // Clear indecating an HOUR has been changed!!
                
                update_ds1302();   //   Update Machine Clock with WiFi Date Time

                //------------------------------
                eeadr=EEPROM.read(1000);     //  Read Now's address
                delay(5);

                if(eeadr>299)
                {
                    eeadr=0; 
                }
                else
                {
                    eeadr++; 
                }

                
                EEPROM.write(eeadr + 600,d_date.toInt());   //  Memorizing a Date to EEPROM
                delay(5);
                
                EEPROM.write(eeadr,t_hour.toInt());   //  Memorizing a WiFi Hour to EEPROM
                delay(5);
                //------------------------------
                 
                getting_machine_date_time();   //  Getting Date and Time from Machine    

                //------------------------------
                EEPROM.write(eeadr+300,t_hour.toInt());   //  Memorizing a Machine Hour to EEPROM
                delay(5);
                //------------------------------
                
                EEPROM.write(1000,eeadr);   //  //  Update Now's address
                delay(5);

                //----------------------------
                
                EEPROM.commit();    //Store data to EEPROM
                delay(5); 
                

            }
                
            //-----------  Send Started Time to Clock (Update at minite Change ) 
            Serial.print("date_"+String(d_year.charAt(0))+String(d_year.charAt(1))+"."+String(d_month.charAt(0))+String(d_month.charAt(1))+"."+String(d_date.charAt(0))+String(d_date.charAt(1))+"."+d_day+"_"+
                          String(t_hour.charAt(0))+String(t_hour.charAt(1))+"."+String(t_minute.charAt(0))+String(t_minute.charAt(1))+"."+String(t_second.charAt(0))+String(t_second.charAt(1))+"_"+String(char(0x40))+String(char(0x40))+"_end."+String(char(13)));

        }
        else
        {
            //-----------  Send Started Time to Clock (Update at minite Change ) 
            Serial.print("date_"+String(d_year.charAt(0))+String(d_year.charAt(1))+"."+String(d_month.charAt(0))+String(d_month.charAt(1))+"."+String(d_date.charAt(0))+String(d_date.charAt(1))+"."+d_day+"_"+
                          String(t_hour.charAt(0))+String(t_hour.charAt(1))+"."+String(t_minute.charAt(0))+String(t_minute.charAt(1))+"."+String(t_second.charAt(0))+String(t_second.charAt(1))+"_"+String(char(0x40))+String(char(0x40))+"_end."+String(char(13)));
        }

    }
   

    //------------------------------------------------------------------------------
    
}
