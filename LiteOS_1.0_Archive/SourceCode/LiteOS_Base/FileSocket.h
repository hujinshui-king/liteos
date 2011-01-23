#ifndef FILESOCKETH
#define FILESOCKETH

void openFileTask();

void closeFileTask();

void readFileTask();

void writeFileTask();

void seekFileTask();



void getFileMutexAddress()  __attribute__((naked));


//Get the file path address
void getFilePathAddress()  __attribute__((naked)) ;

//Get the file mode address
void getFileModeAddress()  __attribute__((naked));


void FileSocketInit(); 

#endif 
