
#include "Public.h"

int main(int argc, char *argv[])
{
	if(-1 == ReadConfig()){
        return -1;
    }
	
	InitLogSys();
	
	if(-1 == InitDatabase()){
        return -1;
    }

    if(-1 == ProcessLastFailTask()){
        return -1;
    }

    if(-1 == ChainCreateProcess()){
        return -1;
    }

    if(-1 == DespositProcess()){
        return -1;
    }

    if(-1 == BlockConfirmProcess()){
        return -1;
    }

    if(-1 == TransferCoinProcess()){
        return -1;
    }

	CheckExit();
	
	return 0;
}
