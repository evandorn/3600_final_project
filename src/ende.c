#include <stdlib.h>
#include <string.h>
// key can only be a-z with no repeats
char *getKey(char *key) {
   int i;
   char enc[] = "abcdefghijklmnopqrstuvwxyz";
   char c;
   char temp[26];
   char temp2[26]; 
   strcpy(temp, "abcdefghijklmnopqrstuvwxyz");
   for (i = strlen(key)-1; i >= 0 ; i--) {
      c = key[i];
      strcpy(temp, strchr(enc, key[i])+1);
      strncpy(temp2, enc, strlen(enc)-strlen(temp));
      temp2[strlen(enc)-strlen(temp)-1] = '\0';
      strncpy(enc,temp2,strlen(enc));
      //temp = (temp[1]);
      
      
      strcat(enc, temp);
   }
   strcpy(temp, key);
   strcat(temp, enc);
   return temp;
}

char *encode(char *key, char *message) {
   int i;
   char msg[strlen(message)]; strcpy(msg, message);
   char enc[] = "abcdefghijklmnopqrstuvwxyz";
   strcpy(enc, getKey(key));
   for (i = strlen(msg)-1; i >= 0; i--) {
      msg[i] = enc[msg[i] - 'a'];
   }
   return msg;
}

char *decode(char *key, char *in_message) {
   //printf("%s\n",in_message);
   int i;
   char msg[strlen(in_message)]; strcpy(msg, in_message);
   char alpha[] = "abcdefghijklmnopqrstuvwxyz";
   char enc[] = "abcdefghijklmnopqrstuvwxyz";
   strcpy(enc, getKey(key));
   for (i = strlen(msg)-1; i >= 0; i--) {
      
      msg[i] = alpha[26-strlen(strchr(enc, msg[i]))];
   }
   return msg;
}

int main() {
   char *s = encode("key", "hello");
   char o[strlen(s)];// = "fbjjn";
   strcpy(o,s);
   char *u = decode("key", o);
   printf("%s\n",u);
   return 0;
}
