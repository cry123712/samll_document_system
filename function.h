#pragma once
#include "Tree.h"

#define PATH_MAX 4096
//修改成可以选择从哪个文件输入（默认是test.txt)

// 读取文件（存储的信息）(功能11）
void readFileAndBuild() {
    char filename[256]; // 假设文件名不超过255个字符
    // 提示用户输入文件名
    printf("请输入文件名（默认为test.txt）: ");
    scanf("%255s", filename); // 读取用户输入的文件名，限制长度为255

    // 尝试打开文件
    FILE* fp1 = fopen(filename, "r");
    if (fp1 == NULL) {
        perror("无法打开文件");
        return;
    }
    char line[256];
    char type[50];
    char name[MAX_NAME];
    long fileSize;
    char parentName[MAX_NAME];
    struct tm tm = { 0 };
    while (fgets(line, sizeof(line), fp1)) {
        if (sscanf(line, "type: %s", type) == 1) {
            continue;
        }
        if (sscanf(line, "name: %s", name) == 1) {
            continue;
        }
        if (sscanf(line, "size: %ld", &fileSize) == 1) {
            continue;
        }
        if (sscanf(line, "parent: %s", parentName) == 1) {
            continue;
        }
        if (sscanf(line, "time: %d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
            tm.tm_year -= 1900; // tm_year 是从 1900 年开始计算的
            tm.tm_mon -= 1;     // tm_mon 从 0 开始计算
            time_t createTime = mktime(&tm);
            if (root == NULL) { // 如果根节点尚未创建，则创建根节点
                root = CreateNode(name, directory, createTime, 0);
                AddNode(root, parentName);
                current = root;
                Dir_current = root;
            }
            else if (strcmp(type, "directory") == 0) {
                Node* newDir = CreateNode(name, directory, createTime, 0);
                AddNode(newDir, parentName);
                current = newDir;
            }
            else if (strcmp(type, "file") == 0) {
                Node* newFile = CreateNode(name, File, createTime, fileSize);
                AddNode(newFile, parentName);
                current = newFile;
            }
        }
    }
    printf("准备工作完成：文件中的内容已读入\n");
    fclose(fp1);
}
// 打印文件系统信息（功能12）
void printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}
void printFileSystem(Node* node, int indent, Node** path, int* pathSize) {
    if (node == NULL) {
        return; // 递归基案
    }
    // 检查当前节点是否已经在路径中
    for (int i = 0; i < *pathSize; ++i) {
        if (path[i] == node) {
            // 节点已经在路径中，返回以避免循环引用
            return;
        }
    }
    // 将当前节点添加到路径中
    path[*pathSize] = node;
    (*pathSize)++;

    // 打印当前节点的信息
    printIndent(indent);
    printf("%s -- Type:%s, Size:%ld, first_disk:%d, Created:%s\n",
        node->name, (node->type == File? "File" : "Directory"), node->fileSize, node->first_disk, ctime(&node->createTime));

    // 打印子节点信息
    if (node->firstChild != NULL) {
        printFileSystem(node->firstChild, indent + 1, path, pathSize);
    }

    // 打印兄弟节点信息，并在完成后退出
    Node* temp = node->nextSibling;
    if (temp != NULL) {
        printFileSystem(temp, indent, path, pathSize);
        temp = temp->nextSibling;
    }

    // 回溯路径，移除当前节点
    (*pathSize)--;
}
//（1）显示当前路径信息；(往前面连接）
void addNameToPath(char* path, const char* name, int pathSize) {
    int nameLen = strlen(name);
    int pathLen = strlen(path);
    // 检查路径是否已满或名称是否太长
    if (pathLen + nameLen + 1 >= pathSize) {
        // 路径已满或名称太长，无法添加
        return;
    }
    // 移动现有路径以腾出空间
    memmove(path + nameLen + 1, path, pathLen + 1);
    // 在路径开头添加名称和斜杠
    path[0] = '/';
    memcpy(path + 1, name, nameLen);
    // 确保新路径以空字符结尾
    path[pathLen + nameLen + 1] = '\0';
}
void showCurrentPath(Node*currentNode) {
    printf("当前节点的绝对路径为\n");
    if (currentNode == root) {
        printf("root\n");
        return;
    }
    char path[PATH_MAX] = { 0 };
    Node* p = currentNode;
    // 从当前节点回溯到根节点，收集路径
    do {
        addNameToPath(path, p->name, sizeof(path));
        p = p->parent;
    } while (p != root);
    // 打印路径
    printf("root%s\n", path);
}
//（2）显示当前路径下所有文件和子目录信息；
void showCurrentInformation(Node**currentNode) {
    if (*currentNode == NULL) {
        printf("当前路径信息不存在\n");
        return;
    }
    printf("当前目录名为 '%s':\n", (*currentNode)->name); // 打印当前目录名称
    printf("当前路径下所有文件和子目录信息如下\n");
    Node* child = (*currentNode)->firstChild; // 获取第一个子节点

    // 遍历并打印当前目录下的所有子目录和文件
    while (child != NULL) {
        printIndent(1); // 缩进一级
        printf("%s -- Type:%s, Size:%ld, first_disk:%d, Created:%s\n",
            child->name, (child->type == File ? "File" : "directory"), child->fileSize, child->first_disk, ctime(&child->createTime));
        child = child->nextSibling; // 移动到下一个兄弟节点
        }
}
//（3）切换当前目录到上一级父目录；
void changeToParentDirectory(Node**currentNode) {
    if (currentNode == NULL || *currentNode == NULL) {
        printf("当前目录不存在\n");
       
    }else if (*currentNode ==root) {
        printf("已经在根目录\n");
    }
    else {
        *currentNode = (*currentNode)->parent;
        printf("上一级目录为: %s\n", (*currentNode)->name);
    }
}
//前置功能：查看当前目录下信息以便于做出选择/判断是否进行了操作
void printForChoice(Node *currentNode) {
    if (currentNode == NULL) {
        printf("没有这个目录名\n");
        return;
    }
    else {
        showCurrentInformation(&currentNode);
    }
}
//（4）切换当前目录到下一级选定的子目录；
void changeToSubDirectory(Node**currentNode) {
    printForChoice(*currentNode);
    printf("请选择要跳转的目录名\n");
    char subdirName[MAX_NAME];
    scanf("%s", subdirName);
    Node* child = (*currentNode)->firstChild;
    while (child != NULL) {
        if (strcmp(child->name, subdirName) == 0 && child->type == directory) {
            (*currentNode) = child;
            printf("切换到目录 %s\n", (*currentNode)->name);
            printForChoice(*currentNode);
            return;
        }
        child = child->nextSibling;
    }
    printf("目录名'%s' 未找到\n", subdirName);
}
int NameToFind(char Name[MAX_NAME]);
//（5）在当前目录下创建一个新的空文件；
void createNewFile(Node*currentNode) {
    printForChoice(currentNode);
    printf("请输入要创建的文件名\n");
    char fileName[MAX_NAME];
    scanf("%s", fileName);
    // 创建一个新的空文件节点
    Node* newFile = CreateNode(fileName, File, time(NULL), 0);
    if (newFile == NULL) {
        printf("创建失败\n");
        return;
    }
    int a = NameToFind(fileName);
    if (a == 1) {
        printf("名字为%s的文件或目录已经存在,无法创建为此名的文件\n", fileName);
    }
    // 将新文件添加到当前目录的子节点列表中
    else {
        AddNode(newFile, currentNode->name);
        printf("文件'%s'创建在目录 '%s'下\n", fileName, currentNode->name);
        printForChoice(currentNode);
    }
}
//（6）在当前目录下删除一个文件；
void deleteFile(Node*currentNode) {
    printForChoice(currentNode);
    printf("请选择需要删除的文件\n");
    char fileName[MAX_NAME];
    scanf("%s", fileName);
    Node** prev = &(currentNode->firstChild); // 指向当前子节点的前一个节点的指针
    Node* child = currentNode->firstChild;
    Node* toDelete = NULL;

    // 遍历当前目录的子节点列表，找到要删除的文件
    while (child != NULL) {
        if (strcmp(child->name, fileName) == 0 && child->type == File) {
            toDelete = child;
            break;
        }
        prev = &(child->nextSibling);
        child = child->nextSibling;
    }
    // 如果找到了要删除的文件
    if (toDelete != NULL) {
        // 从链表中移除该节点
        *prev = toDelete->nextSibling;
        // 释放节点占用的内存
        free(toDelete);
        printf("文件'%s'已经在目录'%s'下删除\n", fileName, currentNode->name);
        printForChoice(currentNode);
    }
    else {
        printf("文件 '%s' 在目录'%s'下未找到\n", fileName, currentNode->name);
    }
}
//（7）在当前目录下创建一个新文件夹；
void createNewDirectory(Node*currentNode) {
    printForChoice(currentNode);
    printf("请输入要创建的文件夹名\n");
    char dirName[MAX_NAME];
    scanf("%s", dirName);
    // 创建一个新的目录节点
    Node* newDir = CreateNode(dirName, directory, time(NULL), 0);
    if (newDir == NULL) {
        printf("创建文件夹失败\n");
        return;
    }int a = NameToFind(dirName);
    if (a == 1) {
        printf("名字为%s的文件或目录已经存在,无法创建为此名的文件夹\n", dirName);
    }
    else {
        // 将新目录添加到当前目录的子节点列表中
        AddNode(newDir, currentNode->name);
        printf("文件夹'%s'创建已经在'%s'目录下\n", dirName, currentNode->name);
        printForChoice(currentNode);
    }
}
//（8）在当前目录下删除一个子目录（设定只能删除空目录，则有内容的目录不让删除。)
                               // (设定任何目录都可以删除，则删除子目录以及子目录下所有内容）；
//只删除空目录
void delete1(Node* currentNode, char dirname[MAX_NAME]) {
    Node* child = currentNode->firstChild;
    while (child != NULL) {
        if (strcmp(child->name, dirname) == 0 && child->type == directory && child->firstChild == NULL) {
            Node* temp = child;
            child = child->nextSibling;
            // 从链表中移除该节点
            if (temp == currentNode->firstChild) {
                currentNode->firstChild = temp->nextSibling;
            }
            else {
                Node* sibling = currentNode->firstChild;
                while (sibling->nextSibling != temp) {
                    sibling = sibling->nextSibling;
                }
                sibling->nextSibling = temp->nextSibling;
            }
            free(temp);
            printf("空目录 '%s'已经删除\n", dirname);
            printForChoice(currentNode);
            return;
        }
        else {
            child = child->nextSibling;
        }
    }
    printf("目录'%s'不为空或者没找到\n", dirname);
}
//都可以删除
   // 递归删除节点及其所有子节点
void deleteNode(Node* node) {
    if (node == NULL) return;
    Node* child = node->firstChild;
    while (child != NULL) {
        Node* next = child->nextSibling;
        deleteNode(child);
        child = next;
    }
    free(node);
}
void delete2(Node* currentNode, char dirname[MAX_NAME]) {
    Node* child = currentNode->firstChild;
    Node* prev = NULL;
    while (child != NULL) {
        if (strcmp(child->name, dirname) == 0 && child->type == directory) {
            if (child->firstChild != NULL) {
                deleteNode(child->firstChild); // 删除子目录下所有内容
            }
            if (prev == NULL) {
                currentNode->firstChild = child->nextSibling;
            }
            else {
                prev->nextSibling = child->nextSibling;
            }
            free(child);
            printf("目录'%s'已删除\n", dirname);
            printForChoice(currentNode);
            return;
        }
        else {
            prev = child;
            child = child->nextSibling;
        }
    }
    printf("目录'%s'未找到\n", dirname);
}
void deleteDirectory(Node* currentNode) {
    printForChoice(currentNode);
    printf("当前有两种删除目录方式可选择\n");
    printf("1、只能删除空目录\n");
    printf("2、任何目录都可以删除(请慎重选择)\n");
    
    int pick=0;
    printf("请选择删除方式\n");
    scanf("%d", &pick);
    printf("请输入要删除的目录名\n");
    char dirName[MAX_NAME];
    scanf("%s", dirName);
    switch (pick) {
    case 1:
        delete1(currentNode, dirName);
        break;
    case 2:
        delete2(currentNode, dirName);
        break;
    default:
        printf("选择无效\n");
        break;
    }
}
//（9）查找：给你一个字符串
// 查找所有文件名和目录名为此字符串的文件或目录，
// 打印输出查找到的所有文件和目录的绝对路径信息；
    // 递归函数用于查找文件系统
const int MAX_DEPTH = 100; 
Node* searchFileSystem(Node* node, const char* nameToFind, int indent) {
    if (node == NULL) {
        return NULL;
    }
    // 如果达到最大深度限制，停止递归
    if (indent >= MAX_DEPTH) {
        return NULL;
    }
    if (strcmp(node->name, nameToFind) == 0) {
        // 找到同名节点，返回该节点
        return node;
    }
    // 递归搜索子节点
    Node* foundNode = searchFileSystem(node->firstChild, nameToFind, indent + 1);
    if (foundNode != NULL) {
        return foundNode; // 如果在子节点中找到，返回找到的节点
    }
    return searchFileSystem(node->nextSibling, nameToFind, indent);
}
int NameToFind(char Name[MAX_NAME]) {
    Node* temp = searchFileSystem(root, Name, 0);
    if (temp == NULL) {
        printf("名字为该字符串的文件或目录不存在\n");
        return 0;
    }
    else {
        showCurrentPath(temp);
        return 1;
    }
}
//（10）把当前文件系统存入某一个文件中；
// 函数用于将节点信息写入文件
void writeNodeToFile(FILE* file, Node* node, int indent) {
    if (node == NULL) return;
    for (int i = 0; i < indent; i++) fprintf(file, "  ");
    fprintf(file, "type: %s\n", (node->type == directory ? "directory" : "file"));
    fprintf(file, "name: %s\n", node->name);
    fprintf(file, "size: %ld\n", node->fileSize);
    fprintf(file, "parent: %s\n", (node->parent==NULL ? "null": node->parent->name));
    struct tm* timeInfo = localtime(&node->createTime);
    char timeStr[26];
    strftime(timeStr, 26, "%Y-%m-%d %H:%M:%S", timeInfo);
    fprintf(file, "time: %s\n\n", timeStr);
}
// 函数用于将文件系统存入文件
void saveFileSystem(Node* root) {
    char filename[MAX_NAME]; // 用于存储文件名的缓冲区
    printf("输入要将文件系统存入的文件名: ");
    
    scanf("%s", filename); // 读取文件名
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("打开文件失败");
        return;
    }
    // 使用栈进行迭代遍历
    Node* stack[MAX_DISK_SIZE]; // 假设文件系统深度不会超过 MAX_DISK_SIZE
    int top = -1;
    stack[++top] = root;

    while (top != -1) {
        Node* node = stack[top--];
        writeNodeToFile(file, node, 0);

        Node* child = node->firstChild;
        while (child != NULL) {
            stack[++top] = child;
            child = child->nextSibling;
        }
    }
    printf("当前文件系统已存入%s文件\n", filename);
    fclose(file);
}
//(12)重命名文件或目录
void rename() {
    char oldName[MAX_NAME]; 
    printf("请输入要重命名的文件名或者目录名: ");
    scanf("%s", oldName); 
    char newName[MAX_NAME];
    printf("输入该文件或目录要更改的名字: ");
    scanf("%s", newName);
    Node* temp = searchFileSystem(root,oldName,0);
    if (temp == NULL) {
        printf("名字为该字符串的文件或目录不存在\n");
    }
    else{
       int a= NameToFind(newName);
        if (a==1) {
            printf("名字为%s的文件或目录已经存在,无法更改为此名\n",oldName);
        }
        else {
            strcpy(temp->name, newName);
            printf("名字为%s的文件或目录已更名为%s\n", oldName, newName);
        }

    }
    
}
//(13)创建一个非空文件并插入到指定目录下
const char* findLastPathComponent(const char* path) {
    const char* lastSlash = strrchr(path, '/'); // 查找最后一个 '/'

    if (lastSlash == NULL) {
        // 没有找到 '/'，整个路径就是最后一个部分
        return path;
    }
    else {
        // 提取最后一个 '/' 后面的部分
        const char* lastComponent = lastSlash + 1;
       return lastComponent;
    }
}
void addNotNullFile() {
    printf("请输入非空文件的名称\n");
    char FileName[MAX_NAME];
    long FileSize=0;
    NodeType FileType = File;
    scanf("%s", FileName);
    printf("请输入非空文件的大小\n");
    scanf("%ld", &FileSize);
    Node* newFile = CreateNode(FileName,FileType,time(NULL), FileSize);
    printf("请指定该文件的存储路径\n");
    char path[PATH_MAX];
    scanf("%s", path);
    const char* parent = findLastPathComponent(path);
    char copyparentName[MAX_NAME]; // 确保有足够的空间
    strcpy(copyparentName, parent); // 复制字符串
    AddNode(newFile, copyparentName);
    printf("文件%s已经插入到目录%s下\n",FileName,copyparentName);
}
//(14)新增清屏功能
// 清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Windows系统使用cls命令
#endif
}