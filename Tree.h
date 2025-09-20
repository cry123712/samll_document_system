#pragma once
#ifndef TREE_H
#define TREE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 256
#define DISK_BLOCK_SIZE 1024 // ����ÿ�����̿��СΪ1024�ֽ�
#define MAX_DISK_SIZE 1024 // ������������ܴ�СΪ1024�����̿�

typedef enum { File, directory } NodeType;//ö��

typedef struct Node {
    char name[MAX_NAME];       // �ļ�����Ŀ¼��
    NodeType type;             // ���ͣ��ļ���Ŀ¼
    time_t createTime;         // ����ʱ��
    long fileSize;             // �ļ���С������Ŀ¼��������Ϊ0
    int first_disk;            // �׿���̿��
    struct Node* parent;       // ���ڵ�ָ��
    struct Node* firstChild;   // ��һ�����ӽڵ�ָ��
    struct Node* nextSibling;  // ��һ���ֵܽڵ�ָ��
} Node;

#endif // TREE_H

Node* root = NULL;
Node* current = NULL;
Node* Dir_current = NULL;//��Ŀ¼��1
int last_disk_used = 0; // ����������Ĵ��̿��
int Sum = 0;

// �����½ڵ�ĺ���
Node* CreateNode(const char* name, NodeType type, time_t times, long fileSize) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->name, name);
    newNode->type = type;
    newNode->createTime = (times != 0) ? times : time(NULL); // �������ʱ��Ϊ0����ʹ�õ�ǰʱ��
    newNode->fileSize = fileSize;
    newNode->first_disk = last_disk_used; // ���ô��̿��Ϊ��ǰ���ʹ�õĴ��̿��
    newNode->parent = NULL;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    // ������ʹ�õĴ��̿��
    int blocksNeeded = (fileSize + DISK_BLOCK_SIZE - 1) / DISK_BLOCK_SIZE;
    if (last_disk_used + blocksNeeded > MAX_DISK_SIZE) {
        fprintf(stderr, "Error: Not enough disk space to create %s.\n", name);
        free(newNode);
        return NULL;
    }
    last_disk_used += blocksNeeded;

    return newNode;
}

// ���Ҹ��ڵ�ĺ���
Node* findParent(Node* root, const char* parentName) {
    if (root == NULL) return NULL;
    if (strcmp(root->name, parentName) == 0) return root;
    Node* found = findParent(root->firstChild, parentName);
    if (found) return found;
    return findParent(root->nextSibling, parentName);
}

// ��ӽڵ�ĺ���
void AddNode(Node* newOne, char* parentName) {
    if (strcmp(parentName, "null") == 0) {
        if (root == NULL) {
            root = newOne;
            root->parent = NULL; // ���ø��ڵ�Ϊ NULL
        }
        //else {
        //    Node* temp = root;
        //    while (temp->nextSibling != NULL) {
        //        temp = temp->nextSibling;
        //    }
        //    temp->nextSibling = newOne;
        //    newOne->parent = root; // ���ø��ڵ�Ϊ root
        //}
    }
    else {
        Node* parent = findParent(root, parentName);
        if (parent != NULL) {
            if (parent->firstChild == NULL) {
                parent->firstChild = newOne;
                newOne->parent = parent;
            }
            else {
                Node* temp = parent->firstChild;
                while (temp->nextSibling != NULL) {
                    temp = temp->nextSibling;
                }
                temp->nextSibling = newOne;
                newOne->parent = parent;
            }
        }
    }
}
