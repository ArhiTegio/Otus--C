#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>


bool file_exists (char *filename) {
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}

int convert(int* idxs, int* lens, unsigned char* conv_masks, FILE *stream, FILE *stream_save)
{
    unsigned char c;
    unsigned char write_[1] = {0};

    while (!feof(stream) && !ferror(stream))
    {
        fread(&c, sizeof(char), 1, stream);

        if(c <= 127)
        {
            write_[0] = c;
            fwrite(write_, 1 , sizeof(write_), stream_save);
        }
        else
        {
            int idx = idxs[c-128];
            int len = lens[c-128];

            for(int i = idx; i < idx+len; ++i)
            {
                write_[0] = conv_masks[i];
                fwrite(write_, 1 , sizeof(write_), stream_save);
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    if(argc == 4)
    {
        char file_name[FILENAME_MAX] = "cp1251.txt";
        strcpy (file_name, argv[1]);
        char file_name_save[FILENAME_MAX] = "t.txt";
        strcpy (file_name_save, argv[2]);
        char encode_type[FILENAME_MAX] = "";
        strcpy (encode_type, argv[3]);

        if(file_exists(file_name))
        {
            FILE *file;
            FILE *file_save;

            file = fopen(file_name, "rb+");
            if(file != NULL)
            {
                file_save = fopen(file_name_save, "wb+");
                if(file_save != NULL)
                {
                    if(strcmp(encode_type, "cp1251")==0)
                    {
                        int idxs[128] = {0,2,4,7,9,12,15,18,21,24,27,29,32,34,36,38,40,42,45,48,51,54,57,60,63,65,68,70,73,75,77,79,81,83,85,87,89,91,93,95,97,99,101,103,105,107,109,111,113,115,117,119,121,123,125,127,129,131,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,256,258,260,262,264,266,268,270,272};
                        int lens[128] = {2,2,3,2,3,3,3,3,3,3,2,3,2,2,2,2,2,3,3,3,3,3,3,3,2,3,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
                        unsigned char conv_masks[274] = {208,130,208,131,226,128,154,209,147,226,128,158,226,128,166,226,128,160,226,128,161,226,130,172,226,128,176,208,137,226,128,185,208,138,208,140,208,139,208,143,209,146,226,128,152,226,128,153,226,128,156,226,128,157,226,128,162,226,128,147,226,128,148,194,152,226,132,162,209,153,226,128,186,209,154,209,156,209,155,209,159,194,160,208,142,209,158,208,139,194,164,210,144,194,166,194,167,208,129,194,169,208,132,194,171,194,172,194,173,194,174,208,135,194,176,194,177,208,134,209,150,210,145,194,181,194,182,194,183,209,145,226,132,150,209,148,194,187,209,152,208,133,209,149,209,151,208,144,208,145,208,146,208,147,208,148,208,149,208,150,208,151,208,152,208,153,208,154,208,155,208,156,208,157,208,158,208,159,208,160,208,161,208,162,208,163,208,164,208,165,208,166,208,167,208,168,208,169,208,170,208,171,208,172,208,173,208,174,208,175,208,176,208,177,208,178,208,179,208,180,208,181,208,182,208,183,208,184,208,185,208,186,208,187,208,188,208,189,208,190,208,191,209,128,209,129,209,130,209,131,209,132,209,133,209,134,209,135,209,136,209,137,209,138,209,139,209,140,209,141,209,142,209,143};
                        convert(idxs, lens, conv_masks, file, file_save);
                    }
                    else if (strcmp(encode_type, "koi8_r")==0)
                    {
                        int idxs[128] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57,60,63,66,69,72,75,78,80,83,85,87,89,91,94,97,100,102,105,108,111,114,117,120,123,126,129,132,135,138,141,144,147,149,152,155,158,161,164,167,170,173,176,179,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,256,258,260,262,264,266,268,270,272,274,276,278,280,282,284,286,288,290,292,294,296,298,300,302,304,306,308,310};
                        int lens[128] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,2,2,2,2,3,3,3,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
                        unsigned char conv_masks[312] = {226,148,128,226,148,130,226,148,140,226,148,144,226,148,148,226,148,152,226,148,156,226,148,164,226,148,172,226,148,180,226,148,188,226,150,128,226,150,132,226,150,136,226,150,140,226,150,144,226,150,144,226,150,146,226,150,147,226,140,160,226,150,160,226,136,153,226,136,154,226,137,136,226,137,164,226,137,165,194,160,226,140,161,194,176,194,178,194,183,195,183,226,149,144,226,149,145,226,149,146,209,145,226,149,147,226,149,148,226,149,149,226,149,150,226,149,151,226,149,152,226,149,153,226,149,154,226,149,155,226,149,156,226,149,157,226,149,158,226,149,159,226,149,160,226,149,161,208,129,226,149,162,226,149,163,226,149,164,226,149,165,226,149,166,226,149,167,226,149,168,226,149,169,226,149,170,226,149,171,226,149,172,194,169,209,142,208,176,208,177,209,134,208,180,208,181,209,132,208,179,209,133,208,184,208,185,208,186,208,187,208,188,208,189,208,190,208,191,209,143,209,128,209,129,209,130,209,131,208,182,208,178,209,140,209,139,208,183,209,136,209,141,209,137,209,135,209,138,208,174,208,144,208,145,208,166,208,148,208,149,208,164,208,147,208,165,208,152,208,153,208,154,208,155,208,156,208,157,208,158,208,159,208,175,208,160,208,161,208,162,208,163,208,150,208,146,208,172,208,171,208,151,208,168,208,173,208,169,208,167,208,170};
                        convert(idxs, lens, conv_masks, file, file_save);
                    }
                    else if (strcmp(encode_type, "iso_8859_5")==0)
                    {
                        int idxs[128] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,227,229,231,233,235,237,239,241,243,245,247,249,251,253,255};
                        int lens[128] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
                        unsigned char conv_masks[257] = {194,128,194,129,194,130,194,131,194,132,194,133,194,134,194,135,194,136,194,137,194,138,194,139,194,140,194,141,194,142,194,143,194,144,194,145,194,146,194,147,194,148,194,149,194,150,194,151,194,152,194,153,194,154,194,155,194,156,194,157,194,158,194,159,194,160,208,129,208,130,208,131,208,132,208,133,208,134,208,135,208,136,208,137,208,138,208,139,208,140,194,173,208,142,208,143,208,144,208,145,208,146,208,147,208,148,208,149,208,150,208,151,208,152,208,153,208,154,208,155,208,156,208,157,208,158,208,159,208,160,208,161,208,162,208,163,208,164,208,165,208,166,208,167,208,168,208,169,208,170,208,171,208,172,208,173,208,174,208,175,208,176,208,177,208,178,208,179,208,180,208,181,208,182,208,183,208,184,208,185,208,186,208,187,208,188,208,189,208,190,208,191,209,128,209,129,209,130,209,131,209,132,209,133,209,134,209,135,209,136,209,137,209,138,209,139,209,140,209,141,209,142,209,143,226,132,150,209,145,209,146,209,147,209,148,209,149,209,150,209,151,209,152,209,153,209,154,209,155,209,156,194,167,209,158,209,159};
                        convert(idxs, lens, conv_masks, file, file_save);
                    }
                    fclose(file_save);
                }
                else
                {
                    printf("Файл %s не возможно перезаписать.\n", file_name_save);
                }
                fclose(file);
                printf("%s\n", "Конвертация завершина.");
            }
            else
            {
                printf("Файл %s не найден.\n", file_name);
            }
        }
        else
        {
            printf("%s\n", "Файл не найден.");

        }
        printf("Программа завершена.\n");
    }
    else
    {
        printf("Аргументы не в соответствуют. Пример main.c cp1251.txt utf-8.txt cp1251\n");
    }
    return 0;
}