#pragma once
#include"Tree.h"
#include"function.h"

void M1() {
    printf("----文件系统子菜单1-----\n");
    printf("0、退出操作\n");
    printf("1、显示当前路径信息\n");
    printf("2、显示当前路径下所有文件和子目录信息\n");
    printf("3、根据字符串查找后打印绝对路径\n");
    printf("4、查看当前整个文件系统的树结构\n");
    printf("-------------------------\n");
}
void Funtion1() {
    int choice1 = 0;
    Node* path[MAX_DISK_SIZE];
    int pathSize = 0;
    do {
        M1();
        printf("请选择你的操作:\n");
        scanf("%d", &choice1);
        switch (choice1) {
        case 1:
            showCurrentPath(Dir_current);
            break;
        case 2:
            showCurrentInformation(&Dir_current);
            break;
        case 3:
            printf("请输入要查找的字符串\n");
            char Name[MAX_NAME];
            scanf("%s", Name);
            NameToFind(Name);
            break;
        case 4:
            printFileSystem(root, 0, path, &pathSize);
            break;
        default:
            break;
        }
    } while (choice1 != 0);
}

void M2() {
    printf("----文件系统子菜单2-----\n");
    printf("0、退出操作\n");
    printf("1、切换当前目录到上一级父目录\n");
    printf("2、切换当前目录到下一级选定的子目录\n");
    printf("-------------------------\n");
}
void Funtion2() {
    int choice2 = 0;
    do {
        M2();
        printf("请选择你的操作:\n");
        scanf("%d", &choice2);
        switch (choice2) {
        case 1: 
            changeToParentDirectory(&Dir_current);
            break;
        case 2:
            changeToSubDirectory(&Dir_current);
            break;
        default:
            break;
        }

    } while (choice2 != 0);
}
void M3() {
    printf("----文件系统子菜单3-----\n");
    printf("0、退出操作\n");
    printf("1、在当前目录下创建一个新的空文件\n");
    printf("2、在当前目录下删除一个文件\n");
    printf("3、在当前目录下创建一个新文件夹\n");
    printf("4、在当前目录下删除一个子目录\n");
    printf("5、创建一个非空文件并插入到指定目录下\n");
    printf("6、重命名文件或目录\n");
    printf("-------------------------\n");
}
void Funtion3() {
    int choice3 = 0;
    do {
        M3();
        printf("请选择你的操作:\n");
        scanf("%d", &choice3);
        switch (choice3) {
        case 1:
            createNewFile(Dir_current);
            break;
        case 2:
            deleteFile(Dir_current);
            break;
        case 3:
            createNewDirectory(Dir_current);
            break;
        case 4:
            deleteDirectory(Dir_current);
            break;
        case 5:
            addNotNullFile();
            break;
        case 6:
            rename();
            break;
        default:
            break;
        }
    } while (choice3 != 0);
}
void M4() {
    printf("----文件系统子菜单4-----\n");
    printf("0、退出操作\n");
    printf("1、把当前文件系统存入某一个文件中\n");
    printf("-------------------------\n");
}
void Funtion4() {
    int choice4 = 0;
    do {
        M4();
        printf("请选择你的操作:\n");
        scanf("%d", &choice4);
        switch (choice4) {
        case 1:
            saveFileSystem(root);
            break;
        default:
            break;
        }
    } while (choice4 != 0);
}
