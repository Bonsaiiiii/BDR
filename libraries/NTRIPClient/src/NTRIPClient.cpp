#include"NTRIPClient.h"
int NTRIPClient::reqSrcTbl(char* host,int &port, String id)
{
  if(!connect(host,port)){
      //Serial.print("Cannot connect to ");
      //Serial.println(host);
      return 1;
  }/*p = String("GET ") + String("/") + String(" HTTP/1.0\r\n");
  p = p + String("User-Agent: NTRIP Enbeded\r\n");*/
  String p="GET /";
  p = p + String(" HTTP/1.1\r\n"
        "Host: ") + host + String(":") + port + String("\r\nUser-Agent: NTRIP BTX02Client/") + id + String("\r\n");
  print(p);
  unsigned long timeout = millis();
  while (available() == 0) {
     if (millis() - timeout > 10000) {
        
            //Serial.println("Client Timeout !");
            stop();
            return 5;
     }
     delay(10);
  }
  delay(50);
   int tamanho_do_buffer = available();
  char buffer[tamanho_do_buffer+1];
  readBytes(buffer,tamanho_do_buffer);
  if(strncmp((char*)buffer,"SOURCETABLE 200 OK",17)==0)
  {
    Serial.print((char*)buffer);
    return 4;
  }
  return 2;
}
int NTRIPClient::reqRaw(char* host,int &port,char* mntpnt,char* user,char* psw, String gga, String check, String id)
{
    if(!connect(host,port))return 1;
    String p="GET /";
    String auth="";
    //Serial.println("Request NTRIP");
    
    p = p + mntpnt + String(" HTTP/1.1\r\n"
        "Host: ") + host + String(":") + port + String("\r\nUser-Agent: NTRIP BTX02Client/") + id + String("\r\n"); //String("\r\nUser-Agent: NTRIP X-PAD\r\n");
    
    if (strlen(user)==0) {
        p = p + String(
            "Accept: */*\r\n"
            "Connection: close\r\n"
            );
    }
    else {
        auth = base64::encode(String(user) + String(":") + psw);
        #ifdef Debug
        //Serial.println(String(user) + String(":") + psw);
        #endif
        //Serial.println(String(user) + String(":") + psw);
        p = p + String("Authorization: Basic ");
        p = p + auth;
        p = p + String("\r\n");
        p = p + String(
            "Accept: */*\r\n"
            "Connection: close\r\n"
            );
    }
    p = p + String("\r\n");
    if(check=="S"){
        p = p + gga + String("\r\n");
    }
    print(p);
    //Serial.println(p);
    #ifdef Debug
    //Serial.println(p);
    #endif
    unsigned long timeout = millis();
    while (available() == 0) {
        if (millis() - timeout > 20000) {
            //Serial.println("Client Timeout !");
            return 5;
        }
        delay(10);
    }
    delay(50);
    
    int tamanho_do_buffer = available();
    //Serial.println(tamanho_do_buffer);
    char buffer[tamanho_do_buffer+1];
    readBytes(buffer,tamanho_do_buffer);
    //Serial.print("buffer= ");
    //Serial.print((char*)buffer);
    //Serial.print(" /fim");
    if(strncmp((char*)buffer,"HTTP/1.1 404",12)==0||strncmp((char*)buffer,"404 Not found",13)==0){
        //Serial.println("autentificação incoreta!");
        return 6;
    }
    /*
    if(strncmp((char*)buffer,"HTTP/1.1 403",12)==0||strncmp((char*)buffer,"403 Forbidden",13)==0){
        //Serial.println("autentificação incoreta!");
        return 7;
    }
    */
    if(strncmp((char*)buffer,"HTTP/1.1 401",12)==0||strncmp((char*)buffer,"401 Unauthorized",16)==0){
        //Serial.println("autentificação incoreta!");
        return 3;
    }
    if(strncmp((char*)buffer,"HTTP/1.1 200 OK",15)==0||strncmp((char*)buffer,"200 OK",6)==0)
    {   
      //Serial.println("teste ok");
      return 4;
    }
    if(strncmp((char*)buffer,"ICY 200 OK",10)==0)
    {   
      //Serial.println("teste ok");
      return 4;
    }
    //Serial.println("mucho loco");
    return 2;
}
int NTRIPClient::reqRaw(char* host,int &port,char* mntpnt, String id)
{
    return reqRaw(host,port,mntpnt,"","","","",id);
}
int NTRIPClient::reqRaw(char* host,int &port,char* mntpnt,char* user,char* psw, String id)
{
    return reqRaw(host,port,mntpnt,user,psw,id);
}
int NTRIPClient::sendgga(String gga){
    print(gga);
    return 4;
}
int NTRIPClient::readLine(char* _buffer,int size)
{
  int len = 0;
  while(available()) {
    _buffer[len] = read();
    len++;
    if(_buffer[len-1] == '\n' || len >= size) break;//_buffer[len-1] == '\n' ||
  }
  _buffer[len]='\0';

  return len;
}
