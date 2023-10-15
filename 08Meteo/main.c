#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "curl/curl.h"
#include "curl/typecheck-gcc.h"
#include "curl/easy.h"
#include "cJSON.h"


char *readBuffer = "";
char * append_string(char*str1, char*str2)
{
    char * new_str ;
    if((new_str = malloc(strlen(str1)+strlen(str2)+1)) != NULL){
        new_str[0] = '\0';   // ensures the memory is an empty string
        strcat(new_str,str1);
        strcat(new_str,str2);
    } else {
        printf("malloc failed!\n");
        // exit?
    }
    return new_str;
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    readBuffer = append_string(readBuffer, (char*)contents);
    return size * nmemb;
}


void get_data(char *city)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, append_string(append_string("https://wttr.in/", city), "?format=j1"));

        /* Use HTTP/3 but fallback to earlier HTTP if necessary */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,  readBuffer);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "D:\\Git\\C\\08 - Lib\\Meteo\\cacert.pem");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_3);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

void print_data_from_json(char * data, char * input)
{
    printf(data);
    cJSON *root = cJSON_Parse(data);
    if(root != NULL)
    {
        char * city = cJSON_GetStringByKeyName(root, "areaName");

        printf("City %s\n", city);
        if(strcmp(city, input) == 0)
        {
            printf("Weather %s\n", cJSON_GetStringByKeyName(root, "weatherDesc"));
            printf("Wind - %s %s degree %s km/h\n", cJSON_GetStringByKeyName(root, "winddir16Point"), cJSON_GetStringByKeyName(root, "winddirDegree"), cJSON_GetStringByKeyName(root, "windspeedKmph"));
            printf("Temperature %s C\n", cJSON_GetStringByKeyName(root, "temp_C"));
        }
        else
        {
            printf("Wrong input.");
        }
    }
}


int main()
{
    char** instr;
    printf("Insert city :\n");
    scanf("%s", &instr);

    get_data(instr);
    print_data_from_json(readBuffer, instr);
    return 0;
}
