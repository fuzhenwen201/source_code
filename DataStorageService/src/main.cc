/*************************************************************************
    > File Name: main.cc
    > Author: Kevin
    > Mail: kevin.fu@fintechstar.cn
    > Created Time: Fri 09 Mar 2018 10:38:00 AM CST
 ************************************************************************/

#include "datastorage.h"

int main(int argc, char *argv[])
{
    DataStorageService dataStorageService;
    dataStorageService.Init(argc, argv);
    dataStorageService.Run();
    return EXIT_SUCCESS;
}