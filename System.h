#pragma once
#include"Tree.h"
#include"function.h"

void M1() {
    printf("----�ļ�ϵͳ�Ӳ˵�1-----\n");
    printf("0���˳�����\n");
    printf("1����ʾ��ǰ·����Ϣ\n");
    printf("2����ʾ��ǰ·���������ļ�����Ŀ¼��Ϣ\n");
    printf("3�������ַ������Һ��ӡ����·��\n");
    printf("4���鿴��ǰ�����ļ�ϵͳ�����ṹ\n");
    printf("-------------------------\n");
}
void Funtion1() {
    int choice1 = 0;
    Node* path[MAX_DISK_SIZE];
    int pathSize = 0;
    do {
        M1();
        printf("��ѡ����Ĳ���:\n");
        scanf("%d", &choice1);
        switch (choice1) {
        case 1:
            showCurrentPath(Dir_current);
            break;
        case 2:
            showCurrentInformation(&Dir_current);
            break;
        case 3:
            printf("������Ҫ���ҵ��ַ���\n");
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
    printf("----�ļ�ϵͳ�Ӳ˵�2-----\n");
    printf("0���˳�����\n");
    printf("1���л���ǰĿ¼����һ����Ŀ¼\n");
    printf("2���л���ǰĿ¼����һ��ѡ������Ŀ¼\n");
    printf("-------------------------\n");
}
void Funtion2() {
    int choice2 = 0;
    do {
        M2();
        printf("��ѡ����Ĳ���:\n");
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
    printf("----�ļ�ϵͳ�Ӳ˵�3-----\n");
    printf("0���˳�����\n");
    printf("1���ڵ�ǰĿ¼�´���һ���µĿ��ļ�\n");
    printf("2���ڵ�ǰĿ¼��ɾ��һ���ļ�\n");
    printf("3���ڵ�ǰĿ¼�´���һ�����ļ���\n");
    printf("4���ڵ�ǰĿ¼��ɾ��һ����Ŀ¼\n");
    printf("5������һ���ǿ��ļ������뵽ָ��Ŀ¼��\n");
    printf("6���������ļ���Ŀ¼\n");
    printf("-------------------------\n");
}
void Funtion3() {
    int choice3 = 0;
    do {
        M3();
        printf("��ѡ����Ĳ���:\n");
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
    printf("----�ļ�ϵͳ�Ӳ˵�4-----\n");
    printf("0���˳�����\n");
    printf("1���ѵ�ǰ�ļ�ϵͳ����ĳһ���ļ���\n");
    printf("-------------------------\n");
}
void Funtion4() {
    int choice4 = 0;
    do {
        M4();
        printf("��ѡ����Ĳ���:\n");
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
