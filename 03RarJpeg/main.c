#define NUL '\0'
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


#pragma pack(push, 1)
typedef struct _tagEOCD
{
    uint16_t diskNumber;
    uint16_t startDiskNumber;
    uint16_t numberCentralDirectoryRecord;
    uint16_t totalCentralDirectoryRecord;
    uint32_t sizeOfCentralDirectory;
    uint32_t centralDirectoryOffset;
    uint16_t commentLength;
} EOCD;
#pragma pack(pop)

typedef struct _tagCentralDirectoryFileHeader
{
    uint32_t signature;
    uint16_t versionMadeBy;
    uint16_t generalPurposeBitFlag;
    uint16_t compressionMethod;
    uint16_t modificationTime;
    uint16_t modificationDate;
    uint32_t crc32;
    uint32_t compressedSize;
    uint16_t uncompressedSize;
    uint16_t filenameLength;
    uint16_t extraFieldLength;
    uint16_t extraFieldLength2;
    char *filename;
    char *extraFild;
} CentralDirectoryFileHeader;


int found_eocd(int fileSize, FILE *stream, EOCD *eodc)
{
    size_t pos = -1;
    int signature;
    for (size_t offset = fileSize - sizeof(EOCD); offset > 0; --offset)
    {
        fseek(stream, offset, SEEK_SET);
        fread(&signature, sizeof(int), 1,stream);
        if (0x06054b50 == signature)
        {
            pos = offset;
            fread(eodc, sizeof(EOCD), 1, stream);
            break;
        }
    }
    return pos;
}

int get_cdfh(long fileSize, FILE *stream, CentralDirectoryFileHeader *cdfh)
{
    long pos = -1;
    int signature;

    for (long offset = fileSize - sizeof(int); offset >= 0; --offset)
    {
        fseek(stream, offset, SEEK_SET);
        fread(&signature, sizeof(int), 1,stream);
        if (0x04034b50 == signature)
        {
            pos = offset;
            fseek(stream, offset, SEEK_SET);
            fread(&cdfh->signature, sizeof(uint32_t), 1,stream);
            fread(&cdfh->versionMadeBy, sizeof(uint16_t), 1,stream);
            fread(&cdfh->generalPurposeBitFlag, sizeof(uint16_t), 1,stream);
            fread(&cdfh->compressionMethod, sizeof(uint16_t), 1,stream);
            fread(&cdfh->modificationTime, sizeof(uint16_t), 1,stream);
            fread(&cdfh->modificationDate, sizeof(uint16_t), 1,stream);
            fread(&cdfh->crc32, sizeof(uint16_t), 1,stream);
            fread(&cdfh->compressedSize, sizeof(uint32_t), 1,stream);
            fread(&cdfh->uncompressedSize, sizeof(uint32_t), 1,stream);
            fread(&cdfh->extraFieldLength, sizeof(uint16_t), 1,stream);
            fread(&cdfh->filenameLength, sizeof(uint16_t), 1,stream);

            if(cdfh->compressionMethod == 0 || cdfh->compressionMethod == 8)
            {
                fseek(stream, ftell(stream)+2, 0);
                if(cdfh->filenameLength > 0)
                {
                    char *str_filename[cdfh->filenameLength+2];
                    cdfh->filename = str_filename;
                    fgets(cdfh->filename, cdfh->filenameLength+2, stream);

                    printf("%.*s\n", cdfh->filenameLength, cdfh->filename);
                    printf("\n");
                }

                if(cdfh->extraFieldLength > 0)
                {
                    char *str_extraFild[cdfh->extraFieldLength+2];
                    cdfh->extraFild = str_extraFild;
                    fgets(cdfh->extraFild, cdfh->extraFieldLength+2, stream);
                }
            }
        }
    }
    return pos;
}

int main(int argc, char *argv[])
{
    printf("%d",argc);

    if(argc == 2)
    {
        FILE *fp;
        int file_size = 0;
        char* filename = argv[1];
        char* mode = "rb";
        printf(filename);
        fp = fopen( filename, mode );
        if(fp != NULL)
        {
            int idx = getc(fp);
            while(idx != EOF)
            {
                idx = getc(fp);
                file_size++;
            }

            printf("File size: %d\n", file_size);
            printf("sizeof(EOCD) == %ld\n", sizeof(EOCD));

            EOCD eodc = {0};
            int pos_eodc = found_eocd(file_size, fp, &eodc);
            printf("EOCD signature %d\n", pos_eodc);
            if(pos_eodc != -1)
            {
                CentralDirectoryFileHeader cdfh = {0};
                printf("centralDirectoryOffset %d\n", eodc.centralDirectoryOffset);
                printf("commentLength %d\n", eodc.commentLength);
                printf("diskNumber %d\n", eodc.diskNumber);
                printf("numberCentralDirectoryRecord %d\n", eodc.numberCentralDirectoryRecord);
                printf("sizeOfCentralDirectory %d\n", eodc.sizeOfCentralDirectory);
                printf("startDiskNumber %d\n", eodc.startDiskNumber);
                printf("totalCentralDirectoryRecord %d\n", eodc.totalCentralDirectoryRecord);


                printf("CentralDirectoryFileHeader size = %ld\n", sizeof(CentralDirectoryFileHeader));
                get_cdfh(file_size, fp, &cdfh);
            }
            else
            {

                printf("Архив не найден.");
            }


            fclose(fp);
        }
        else
        {
            printf("Нет такого файла.");
        }


    }
    else
    {

        printf("Неверное количество аргументов.");
    }

    return 0;
}
