#pragma once
#ifndef TREE_H
#define TREE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 256
#define DISK_BLOCK_SIZE 1024 // 假设每个磁盘块大小为1024字节
#define MAX_DISK_SIZE 1024 // 假设虚拟磁盘总大小为1024个磁盘块

typedef enum { File, directory } NodeType;//枚举

typedef struct Node {
    char name[MAX_NAME];       // 文件名或目录名
    NodeType type;             // 类型：文件或目录
    time_t createTime;         // 创建时间
    long fileSize;             // 文件大小，对于目录可以设置为0
    int first_disk;            // 首块磁盘块号
    struct Node* parent;       // 父节点指针
    struct Node* firstChild;   // 第一个孩子节点指针
    struct Node* nextSibling;  // 下一个兄弟节点指针
} Node;

#endif // TREE_H

Node* root = NULL;
Node* current = NULL;
Node* Dir_current = NULL;//是目录类1
int last_disk_used = 0; // 跟踪最后分配的磁盘块号
int Sum = 0;

// 创建新节点的函数
Node* CreateNode(const char* name, NodeType type, time_t times, long fileSize) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->name, name);
    newNode->type = type;
    newNode->createTime = (times != 0) ? times : time(NULL); // 如果传入时间为0，则使用当前时间
    newNode->fileSize = fileSize;
    newNode->first_disk = last_disk_used; // 设置磁盘块号为当前最后使用的磁盘块号
    newNode->parent = NULL;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    // 更新已使用的磁盘块号
    int blocksNeeded = (fileSize + DISK_BLOCK_SIZE - 1) / DISK_BLOCK_SIZE;
    if (last_disk_used + blocksNeeded > MAX_DISK_SIZE) {
        fprintf(stderr, "Error: Not enough disk space to create %s.\n", name);
        free(newNode);
        return NULL;
    }
    last_disk_used += blocksNeeded;

    return newNode;
}

// 查找父节点的函数
Node* findParent(Node* root, const char* parentName) {
    if (root == NULL) return NULL;
    if (strcmp(root->name, parentName) == 0) return root;
    Node* found = findParent(root->firstChild, parentName);
    if (found) return found;
    return findParent(root->nextSibling, parentName);
}

// 添加节点的函数
void AddNode(Node* newOne, char* parentName) {
    if (strcmp(parentName, "null") == 0) {
        if (root == NULL) {
            root = newOne;
            root->parent = NULL; // 设置父节点为 NULL
        }
        //else {
        //    Node* temp = root;
        //    while (temp->nextSibling != NULL) {
        //        temp = temp->nextSibling;
        //    }
        //    temp->nextSibling = newOne;
        //    newOne->parent = root; // 设置父节点为 root
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
