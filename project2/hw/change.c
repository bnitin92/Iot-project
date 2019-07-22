/* Compilation Commands
[x86]
gcc   -c hw.c -o hw.o
gcc   -o hw hw.o -L/usr/lib/x86_64-linux-gnu -lcurl -lpthread

[arm]
${BUILDROOT_HOME}/output/host/usr/bin/arm-linux-gcc --sysroot=${BUILDROOT_HOME}/output/staging  -c hw.c -o hw.o
${BUILDROOT_HOME}/output/host/usr/bin/arm-linux-gcc --sysroot=${BUILDROOT_HOME}/output/staging  -o hw hw.o  -lcurl -uClibc -lc
*/



#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

#define OK          0
#define INIT_ERR    1
#define REQ_ERR     2

void help(){
  printf("Network communication on ARM via HTTP \n");
  printf("Usage:  hw [OPTION]... [URL]...\n");
  printf("  -h   --help for help info\n");
  printf("  -u   --url to name the specified URL to receive Http Request\n");
  printf("HTTP Requests:\n");
  printf("  -g   --get     GET   eg :\"./hw -g/--get -u/--url [URL]\"\n");
  printf("  -o   --post    POST  eg :\"./hw -o/--post -u/--url [URL] \"any message inside inverted quotes\"\"\n");
  printf("  -p   --put     PUT   eg :\"./hw -p/--put -u/--url [URL]\"any message inside inverted quotes\"\"\n");
  printf("  -d   --delete  DELETE  eg :\"./hw -d/--delete -u/--url [URL]\"\n");
  printf("Exit status:\n");
  printf(" 0  OK\n");
  printf(" 1  Initializing Error\n");
  printf(" 2  Requesting Error\n");
}


int main(int argc,char *argv[]){

  CURL *curl;
	CURLcode res;
  char request[10];
  char *url;
  char *postthis;
  char *putthis;

  strcpy(request,argv[1]);
  printf("%s\n", request);
  url=argv[3];
  printf("%s\n", url);

  printf("%s\n", url);
	curl = curl_easy_init();

  if (argv[4]){
    postthis = argv[4];
  }


  if (curl){
    printf("%s\n",request);

    if((strcmp(request, "-g")==0)||(strcmp(request,"--get")==0)){
      printf("this is GET request\n");
      curl_easy_setopt(curl,CURLOPT_URL,url);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // this is for arm
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    }

    else if ((strcmp(request, "-o")==0)||(strcmp(request,"--post")==0)){
      printf("this is POST request\n");
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // arm
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
    }

    else if ((strcmp(request, "-p")==0)||(strcmp(request,"--put")==0)){
      printf("this is PUT request\n");
      curl_easy_setopt(curl,CURLOPT_URL,url);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // arm
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
    }

    else if ((strcmp(request, "-d")==0)||(strcmp(request,"--delete")==0)){
      printf("this is DELETE request\n");
      curl_easy_setopt(curl,CURLOPT_URL,url);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // arm
      curl_easy_setopt(curl,CURLOPT_CUSTOMREQUEST, "DELETE");
      curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    }


    else if ((strcmp(request, "-h")==0)||(strcmp(request,"--help")==0)){
      help();    
    }

    else{
      printf("Options to enter : \n --get/-g, --post/-o, --put/-p, --delete/-d, --help/-h");
    }
    
    res=curl_easy_perform(curl);
    if(res != CURLE_OK) {  // if it is fine 
      return REQ_ERR;
    }
    curl_easy_cleanup(curl);
  }

  else{
    return INIT_ERR;
  }
  return OK;
}
