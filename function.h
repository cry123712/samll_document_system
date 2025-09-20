#pragma once
#include "Tree.h"

#define PATH_MAX 4096
//�޸ĳɿ���ѡ����ĸ��ļ����루Ĭ����test.txt)

// ��ȡ�ļ����洢����Ϣ��(����11��
void readFileAndBuild() {
    char filename[256]; // �����ļ���������255���ַ�
    // ��ʾ�û������ļ���
    printf("�������ļ�����Ĭ��Ϊtest.txt��: ");
    scanf("%255s", filename); // ��ȡ�û�������ļ��������Ƴ���Ϊ255

    // ���Դ��ļ�
    FILE* fp1 = fopen(filename, "r");
    if (fp1 == NULL) {
        perror("�޷����ļ�");
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
            tm.tm_year -= 1900; // tm_year �Ǵ� 1900 �꿪ʼ�����
            tm.tm_mon -= 1;     // tm_mon �� 0 ��ʼ����
            time_t createTime = mktime(&tm);
            if (root == NULL) { // ������ڵ���δ�������򴴽����ڵ�
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
    printf("׼��������ɣ��ļ��е������Ѷ���\n");
    fclose(fp1);
}
// ��ӡ�ļ�ϵͳ��Ϣ������12��
void printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}
void printFileSystem(Node* node, int indent, Node** path, int* pathSize) {
    if (node == NULL) {
        return; // �ݹ����
    }
    // ��鵱ǰ�ڵ��Ƿ��Ѿ���·����
    for (int i = 0; i < *pathSize; ++i) {
        if (path[i] == node) {
            // �ڵ��Ѿ���·���У������Ա���ѭ������
            return;
        }
    }
    // ����ǰ�ڵ���ӵ�·����
    path[*pathSize] = node;
    (*pathSize)++;

    // ��ӡ��ǰ�ڵ����Ϣ
    printIndent(indent);
    printf("%s -- Type:%s, Size:%ld, first_disk:%d, Created:%s\n",
        node->name, (node->type == File? "File" : "Directory"), node->fileSize, node->first_disk, ctime(&node->createTime));

    // ��ӡ�ӽڵ���Ϣ
    if (node->firstChild != NULL) {
        printFileSystem(node->firstChild, indent + 1, path, pathSize);
    }

    // ��ӡ�ֵܽڵ���Ϣ��������ɺ��˳�
    Node* temp = node->nextSibling;
    if (temp != NULL) {
        printFileSystem(temp, indent, path, pathSize);
        temp = temp->nextSibling;
    }

    // ����·�����Ƴ���ǰ�ڵ�
    (*pathSize)--;
}
//��1����ʾ��ǰ·����Ϣ��(��ǰ�����ӣ�
void addNameToPath(char* path, const char* name, int pathSize) {
    int nameLen = strlen(name);
    int pathLen = strlen(path);
    // ���·���Ƿ������������Ƿ�̫��
    if (pathLen + nameLen + 1 >= pathSize) {
        // ·������������̫�����޷����
        return;
    }
    // �ƶ�����·�����ڳ��ռ�
    memmove(path + nameLen + 1, path, pathLen + 1);
    // ��·����ͷ������ƺ�б��
    path[0] = '/';
    memcpy(path + 1, name, nameLen);
    // ȷ����·���Կ��ַ���β
    path[pathLen + nameLen + 1] = '\0';
}
void showCurrentPath(Node*currentNode) {
    printf("��ǰ�ڵ�ľ���·��Ϊ\n");
    if (currentNode == root) {
        printf("root\n");
        return;
    }
    char path[PATH_MAX] = { 0 };
    Node* p = currentNode;
    // �ӵ�ǰ�ڵ���ݵ����ڵ㣬�ռ�·��
    do {
        addNameToPath(path, p->name, sizeof(path));
        p = p->parent;
    } while (p != root);
    // ��ӡ·��
    printf("root%s\n", path);
}
//��2����ʾ��ǰ·���������ļ�����Ŀ¼��Ϣ��
void showCurrentInformation(Node**currentNode) {
    if (*currentNode == NULL) {
        printf("��ǰ·����Ϣ������\n");
        return;
    }
    printf("��ǰĿ¼��Ϊ '%s':\n", (*currentNode)->name); // ��ӡ��ǰĿ¼����
    printf("��ǰ·���������ļ�����Ŀ¼��Ϣ����\n");
    Node* child = (*currentNode)->firstChild; // ��ȡ��һ���ӽڵ�

    // ��������ӡ��ǰĿ¼�µ�������Ŀ¼���ļ�
    while (child != NULL) {
        printIndent(1); // ����һ��
        printf("%s -- Type:%s, Size:%ld, first_disk:%d, Created:%s\n",
            child->name, (child->type == File ? "File" : "directory"), child->fileSize, child->first_disk, ctime(&child->createTime));
        child = child->nextSibling; // �ƶ�����һ���ֵܽڵ�
        }
}
//��3���л���ǰĿ¼����һ����Ŀ¼��
void changeToParentDirectory(Node**currentNode) {
    if (currentNode == NULL || *currentNode == NULL) {
        printf("��ǰĿ¼������\n");
       
    }else if (*currentNode ==root) {
        printf("�Ѿ��ڸ�Ŀ¼\n");
    }
    else {
        *currentNode = (*currentNode)->parent;
        printf("��һ��Ŀ¼Ϊ: %s\n", (*currentNode)->name);
    }
}
//ǰ�ù��ܣ��鿴��ǰĿ¼����Ϣ�Ա�������ѡ��/�ж��Ƿ�����˲���
void printForChoice(Node *currentNode) {
    if (currentNode == NULL) {
        printf("û�����Ŀ¼��\n");
        return;
    }
    else {
        showCurrentInformation(&currentNode);
    }
}
//��4���л���ǰĿ¼����һ��ѡ������Ŀ¼��
void changeToSubDirectory(Node**currentNode) {
    printForChoice(*currentNode);
    printf("��ѡ��Ҫ��ת��Ŀ¼��\n");
    char subdirName[MAX_NAME];
    scanf("%s", subdirName);
    Node* child = (*currentNode)->firstChild;
    while (child != NULL) {
        if (strcmp(child->name, subdirName) == 0 && child->type == directory) {
            (*currentNode) = child;
            printf("�л���Ŀ¼ %s\n", (*currentNode)->name);
            printForChoice(*currentNode);
            return;
        }
        child = child->nextSibling;
    }
    printf("Ŀ¼��'%s' δ�ҵ�\n", subdirName);
}
int NameToFind(char Name[MAX_NAME]);
//��5���ڵ�ǰĿ¼�´���һ���µĿ��ļ���
void createNewFile(Node*currentNode) {
    printForChoice(currentNode);
    printf("������Ҫ�������ļ���\n");
    char fileName[MAX_NAME];
    scanf("%s", fileName);
    // ����һ���µĿ��ļ��ڵ�
    Node* newFile = CreateNode(fileName, File, time(NULL), 0);
    if (newFile == NULL) {
        printf("����ʧ��\n");
        return;
    }
    int a = NameToFind(fileName);
    if (a == 1) {
        printf("����Ϊ%s���ļ���Ŀ¼�Ѿ�����,�޷�����Ϊ�������ļ�\n", fileName);
    }
    // �����ļ���ӵ���ǰĿ¼���ӽڵ��б���
    else {
        AddNode(newFile, currentNode->name);
        printf("�ļ�'%s'������Ŀ¼ '%s'��\n", fileName, currentNode->name);
        printForChoice(currentNode);
    }
}
//��6���ڵ�ǰĿ¼��ɾ��һ���ļ���
void deleteFile(Node*currentNode) {
    printForChoice(currentNode);
    printf("��ѡ����Ҫɾ�����ļ�\n");
    char fileName[MAX_NAME];
    scanf("%s", fileName);
    Node** prev = &(currentNode->firstChild); // ָ��ǰ�ӽڵ��ǰһ���ڵ��ָ��
    Node* child = currentNode->firstChild;
    Node* toDelete = NULL;

    // ������ǰĿ¼���ӽڵ��б��ҵ�Ҫɾ�����ļ�
    while (child != NULL) {
        if (strcmp(child->name, fileName) == 0 && child->type == File) {
            toDelete = child;
            break;
        }
        prev = &(child->nextSibling);
        child = child->nextSibling;
    }
    // ����ҵ���Ҫɾ�����ļ�
    if (toDelete != NULL) {
        // ���������Ƴ��ýڵ�
        *prev = toDelete->nextSibling;
        // �ͷŽڵ�ռ�õ��ڴ�
        free(toDelete);
        printf("�ļ�'%s'�Ѿ���Ŀ¼'%s'��ɾ��\n", fileName, currentNode->name);
        printForChoice(currentNode);
    }
    else {
        printf("�ļ� '%s' ��Ŀ¼'%s'��δ�ҵ�\n", fileName, currentNode->name);
    }
}
//��7���ڵ�ǰĿ¼�´���һ�����ļ��У�
void createNewDirectory(Node*currentNode) {
    printForChoice(currentNode);
    printf("������Ҫ�������ļ�����\n");
    char dirName[MAX_NAME];
    scanf("%s", dirName);
    // ����һ���µ�Ŀ¼�ڵ�
    Node* newDir = CreateNode(dirName, directory, time(NULL), 0);
    if (newDir == NULL) {
        printf("�����ļ���ʧ��\n");
        return;
    }int a = NameToFind(dirName);
    if (a == 1) {
        printf("����Ϊ%s���ļ���Ŀ¼�Ѿ�����,�޷�����Ϊ�������ļ���\n", dirName);
    }
    else {
        // ����Ŀ¼��ӵ���ǰĿ¼���ӽڵ��б���
        AddNode(newDir, currentNode->name);
        printf("�ļ���'%s'�����Ѿ���'%s'Ŀ¼��\n", dirName, currentNode->name);
        printForChoice(currentNode);
    }
}
//��8���ڵ�ǰĿ¼��ɾ��һ����Ŀ¼���趨ֻ��ɾ����Ŀ¼���������ݵ�Ŀ¼����ɾ����)
                               // (�趨�κ�Ŀ¼������ɾ������ɾ����Ŀ¼�Լ���Ŀ¼���������ݣ���
//ֻɾ����Ŀ¼
void delete1(Node* currentNode, char dirname[MAX_NAME]) {
    Node* child = currentNode->firstChild;
    while (child != NULL) {
        if (strcmp(child->name, dirname) == 0 && child->type == directory && child->firstChild == NULL) {
            Node* temp = child;
            child = child->nextSibling;
            // ���������Ƴ��ýڵ�
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
            printf("��Ŀ¼ '%s'�Ѿ�ɾ��\n", dirname);
            printForChoice(currentNode);
            return;
        }
        else {
            child = child->nextSibling;
        }
    }
    printf("Ŀ¼'%s'��Ϊ�ջ���û�ҵ�\n", dirname);
}
//������ɾ��
   // �ݹ�ɾ���ڵ㼰�������ӽڵ�
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
                deleteNode(child->firstChild); // ɾ����Ŀ¼����������
            }
            if (prev == NULL) {
                currentNode->firstChild = child->nextSibling;
            }
            else {
                prev->nextSibling = child->nextSibling;
            }
            free(child);
            printf("Ŀ¼'%s'��ɾ��\n", dirname);
            printForChoice(currentNode);
            return;
        }
        else {
            prev = child;
            child = child->nextSibling;
        }
    }
    printf("Ŀ¼'%s'δ�ҵ�\n", dirname);
}
void deleteDirectory(Node* currentNode) {
    printForChoice(currentNode);
    printf("��ǰ������ɾ��Ŀ¼��ʽ��ѡ��\n");
    printf("1��ֻ��ɾ����Ŀ¼\n");
    printf("2���κ�Ŀ¼������ɾ��(������ѡ��)\n");
    
    int pick=0;
    printf("��ѡ��ɾ����ʽ\n");
    scanf("%d", &pick);
    printf("������Ҫɾ����Ŀ¼��\n");
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
        printf("ѡ����Ч\n");
        break;
    }
}
//��9�����ң�����һ���ַ���
// ���������ļ�����Ŀ¼��Ϊ���ַ������ļ���Ŀ¼��
// ��ӡ������ҵ��������ļ���Ŀ¼�ľ���·����Ϣ��
    // �ݹ麯�����ڲ����ļ�ϵͳ
const int MAX_DEPTH = 100; 
Node* searchFileSystem(Node* node, const char* nameToFind, int indent) {
    if (node == NULL) {
        return NULL;
    }
    // ����ﵽ���������ƣ�ֹͣ�ݹ�
    if (indent >= MAX_DEPTH) {
        return NULL;
    }
    if (strcmp(node->name, nameToFind) == 0) {
        // �ҵ�ͬ���ڵ㣬���ظýڵ�
        return node;
    }
    // �ݹ������ӽڵ�
    Node* foundNode = searchFileSystem(node->firstChild, nameToFind, indent + 1);
    if (foundNode != NULL) {
        return foundNode; // ������ӽڵ����ҵ��������ҵ��Ľڵ�
    }
    return searchFileSystem(node->nextSibling, nameToFind, indent);
}
int NameToFind(char Name[MAX_NAME]) {
    Node* temp = searchFileSystem(root, Name, 0);
    if (temp == NULL) {
        printf("����Ϊ���ַ������ļ���Ŀ¼������\n");
        return 0;
    }
    else {
        showCurrentPath(temp);
        return 1;
    }
}
//��10���ѵ�ǰ�ļ�ϵͳ����ĳһ���ļ��У�
// �������ڽ��ڵ���Ϣд���ļ�
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
// �������ڽ��ļ�ϵͳ�����ļ�
void saveFileSystem(Node* root) {
    char filename[MAX_NAME]; // ���ڴ洢�ļ����Ļ�����
    printf("����Ҫ���ļ�ϵͳ������ļ���: ");
    
    scanf("%s", filename); // ��ȡ�ļ���
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("���ļ�ʧ��");
        return;
    }
    // ʹ��ջ���е�������
    Node* stack[MAX_DISK_SIZE]; // �����ļ�ϵͳ��Ȳ��ᳬ�� MAX_DISK_SIZE
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
    printf("��ǰ�ļ�ϵͳ�Ѵ���%s�ļ�\n", filename);
    fclose(file);
}
//(12)�������ļ���Ŀ¼
void rename() {
    char oldName[MAX_NAME]; 
    printf("������Ҫ���������ļ�������Ŀ¼��: ");
    scanf("%s", oldName); 
    char newName[MAX_NAME];
    printf("������ļ���Ŀ¼Ҫ���ĵ�����: ");
    scanf("%s", newName);
    Node* temp = searchFileSystem(root,oldName,0);
    if (temp == NULL) {
        printf("����Ϊ���ַ������ļ���Ŀ¼������\n");
    }
    else{
       int a= NameToFind(newName);
        if (a==1) {
            printf("����Ϊ%s���ļ���Ŀ¼�Ѿ�����,�޷�����Ϊ����\n",oldName);
        }
        else {
            strcpy(temp->name, newName);
            printf("����Ϊ%s���ļ���Ŀ¼�Ѹ���Ϊ%s\n", oldName, newName);
        }

    }
    
}
//(13)����һ���ǿ��ļ������뵽ָ��Ŀ¼��
const char* findLastPathComponent(const char* path) {
    const char* lastSlash = strrchr(path, '/'); // �������һ�� '/'

    if (lastSlash == NULL) {
        // û���ҵ� '/'������·���������һ������
        return path;
    }
    else {
        // ��ȡ���һ�� '/' ����Ĳ���
        const char* lastComponent = lastSlash + 1;
       return lastComponent;
    }
}
void addNotNullFile() {
    printf("������ǿ��ļ�������\n");
    char FileName[MAX_NAME];
    long FileSize=0;
    NodeType FileType = File;
    scanf("%s", FileName);
    printf("������ǿ��ļ��Ĵ�С\n");
    scanf("%ld", &FileSize);
    Node* newFile = CreateNode(FileName,FileType,time(NULL), FileSize);
    printf("��ָ�����ļ��Ĵ洢·��\n");
    char path[PATH_MAX];
    scanf("%s", path);
    const char* parent = findLastPathComponent(path);
    char copyparentName[MAX_NAME]; // ȷ�����㹻�Ŀռ�
    strcpy(copyparentName, parent); // �����ַ���
    AddNode(newFile, copyparentName);
    printf("�ļ�%s�Ѿ����뵽Ŀ¼%s��\n",FileName,copyparentName);
}
//(14)������������
// ��������
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Windowsϵͳʹ��cls����
#endif
}