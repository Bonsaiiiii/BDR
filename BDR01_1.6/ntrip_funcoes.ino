String scrtbl(const char* mountpoint, String ip, uint16_t port){
  String body = "STR;";
  body = body + mountpoint + String(";;;;0;;HUGEN;BRA;0.00;0.00;0;0;sNTRIP;none;N;N;0;none;\r\n"
            "NET;HUGEN;;N;N;None;") + ip + String(":") + port + String(";None;;\r\n"
            "ENDSOURCETABLE\r\n");
  uint16_t tamanho = body.length();
  String header = "Server:"; 
  header = header + String(" Hugenplus ntrip simple ntrip caster/R1.0\r\n"
            //"Date: ") + ntp.getFormattedDate() + String("\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: ") + String(tamanho) + String("\r\n");
  return header + String("\r\n") + body + String("\r\n");
}

int check_aut(String data, const char* user, const char* pwr){
  char base64[50];
  unsigned char string[50];

  int posicao = data.indexOf(": Basic");
  posicao = posicao + 8;
  String usuario_encoded = data.substring(posicao);
  usuario_encoded.toCharArray(base64,50);
  unsigned int string_length = decode_base64((unsigned char *)base64, string);
  string[string_length] = '\0';
  String usuario_decoded = String((char *)string);
  int separador = usuario_decoded.indexOf(":");
  String cliente_usuario = usuario_decoded.substring(0,separador);
  separador ++;
  String cliente_senha = usuario_decoded.substring(separador);
  if(cliente_usuario.equals(usuario)&cliente_senha.equals(senha)){
    return 8;// senha correta
  }else{
    return 9;// senha incorreta
  }
}

int check_ver(String data){
  int posicao_inicial = data.indexOf("HTTP");
  int posicao_final = data.indexOf("\r\n");
  if (posicao_inicial < 0){
    return 4; //ERRO1 
  }
  String versao_http = data.substring(posicao_inicial, posicao_final);
  //Serial.println(versao_http);
  if(versao_http == "HTTP/1.0"){
    return 1; //REV1
  }
  if(versao_http == "HTTP/1.1"){
    return 2; //REV2
  }
}

int check_mountpoint(String data, const char* mountpoint){
  int posicao_inicial = data.indexOf("/");
  int posicao_final = data.indexOf("HTTP");
  posicao_final--;
  String cliente_mountpoint = data.substring(posicao_inicial,posicao_final);
  String mp = "/";
  mp = mp + mountpoint;
  //Serial.println(cliente_mountpoint);
  //Serial.println(mp);
  if(cliente_mountpoint == mp){
    return 5; //igual
  }
  if(cliente_mountpoint == "/SOURCETABLE.TXT " || cliente_mountpoint == "/"){
    return 6; //solicita
  }
  return 7; //ERRO2
} 