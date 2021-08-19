#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 40

typedef struct directory
{
    char name[SIZE];
    struct directory *parent;
    struct directory *firstChild;
    struct directory *nextSibling;
} DIR;

DIR *createDirectory(char *name)
{
    DIR *tmp = (DIR *)malloc(sizeof(DIR));
    strcpy(tmp->name, name);
    tmp->firstChild = NULL;
    tmp->nextSibling = NULL;
    tmp->parent = NULL;
    printf("\n> dir %s : created", name);
    return tmp;
}

void mkdir(DIR *currentDirectory, char *cmdLoc)
{
    DIR *tmp;
    int flag = 1;

    if (currentDirectory->firstChild == NULL)
    {
        currentDirectory->firstChild = createDirectory(cmdLoc);
        currentDirectory->firstChild->parent = currentDirectory;
        printf("\nResult : Directory %s created under %s", currentDirectory->firstChild->name, currentDirectory->name);
    }
    else
    {
        tmp = currentDirectory->firstChild;

        if (!strcmp(tmp->name, cmdLoc))
            flag = 0;

        while (flag && tmp->nextSibling != NULL)
        {
            printf("\n\t= %s", tmp->nextSibling->name);
            tmp = tmp->nextSibling;
            if (!strcmp(tmp->name, cmdLoc))
                flag = 0;
        }

        if (flag)
        {
            tmp->nextSibling = createDirectory(cmdLoc);
            tmp->nextSibling->parent = currentDirectory;

            printf("\nResult : Directory %s created under %s", tmp->nextSibling->name, currentDirectory->name);
        }
        else
            printf("\nError : A directory with the same name already exists");
    }
}

DIR *chdir(DIR *currentDirectory, char *cmdLoc)
{
    if (!strcmp(cmdLoc, "."))
    {
        return currentDirectory;
    }
    else if (!strcmp(cmdLoc, "..") && strcmp(currentDirectory->name, "Root"))
    {
        return currentDirectory->parent;
    }
    else if (!strcmp(cmdLoc, "..") && !strcmp(currentDirectory->name, "Root"))
    {
        printf("\nError : \tRoot files do not have a parent");
        return currentDirectory;
    }
    else
    {
        DIR *tmp;
        tmp = currentDirectory->firstChild;

        while (tmp != NULL && strcmp(tmp->name, cmdLoc))
        {
            tmp = tmp->nextSibling;
        }

        if (tmp != NULL)
        {
            printf("\nResult : Directory changed");
            return tmp;
        }
        else
        {
            printf("\nError : A directory with given name does not exist");
            return currentDirectory;
        }
    }
}

void dir(DIR *currentDirectory)
{
    DIR *tmp;
    if (currentDirectory->parent != NULL)
        printf("\n┐ %s\n   └ %s┐ ", currentDirectory->parent->name, currentDirectory->name);
    else
        printf("\n   └ %s ┐ ", currentDirectory->name);

    if (currentDirectory->firstChild != NULL)
    {
        printf("\n\t└ %s", currentDirectory->firstChild->name);
        tmp = currentDirectory->firstChild;
        while (tmp->nextSibling)
        {
            printf("\n\t└ %s", tmp->nextSibling->name);
            tmp = tmp->nextSibling;
        }
    }

    printf("\n");
}

void rmdir(DIR *currentDirectory, char *cmdLoc)
{
    DIR *tmp;

    tmp = currentDirectory->firstChild;

    if (tmp != NULL && !strcmp(tmp->name, cmdLoc))
    {
        if (tmp->firstChild != NULL)
        {
            printf("\tResult : There are one or more directories under given folder. Deleting directories with children is not avaliable");
        }
        else
        {
            currentDirectory->firstChild = tmp->nextSibling;
            free(tmp);
        }
    }
    else
    {
        DIR *left;

        while (tmp != NULL && strcmp(tmp->name, cmdLoc))
        {
            left = tmp;
            tmp = tmp->nextSibling;
        }

        if (tmp != NULL)
        {
            if (tmp->firstChild != NULL)
            {
                printf("\tResult : There are one or more directories under given folder. Deleting directories with children is not avaliable");
            }
            else
            {
                left->nextSibling = tmp->nextSibling;
                free(tmp);
            }
        }
        else
        {
            printf("\nError : Directory with given name does not exist");
        }
    }
}
void count(DIR *currentDirectory)
{
    DIR *tmp;
    int count = 0;
    if (currentDirectory->firstChild != NULL)
    {
        count++;
        tmp = currentDirectory->firstChild;

        while (tmp->nextSibling)
        {
            count++;
            tmp = tmp->nextSibling;
        }
    }
    printf("\nResult : This directory contains %d directory", count);
}

void main()
{

    DIR *root;
    int active = 1;
    char cmdLoc[SIZE];
    char cmd[SIZE];
    printf("Umutcan Sevdi\t19011091");
    root = createDirectory("Root");
    DIR *currentDirectory = root;

    while (active)
    {
        if (currentDirectory->parent != NULL)
            printf("\n../%s/%s >_\t", currentDirectory->parent->name, currentDirectory->name);
        else
            printf("\n%s >_\t", currentDirectory->name);

        scanf("%s", cmd);
        if (!strcmp(cmd, "mkdir") || !strcmp(cmd, "chdir") || !strcmp(cmd, "rmdir"))
            scanf("%s", cmdLoc);

        if (!strcmp(cmd, "mkdir"))
            mkdir(currentDirectory, cmdLoc);
        else if (!strcmp(cmd, "chdir"))
            currentDirectory = chdir(currentDirectory, cmdLoc);
        else if (!strcmp(cmd, "dir"))
            dir(currentDirectory);
        else if (!strcmp(cmd, "rmdir"))
            rmdir(currentDirectory, cmdLoc);
        else if (!strcmp(cmd, "count"))
            count(currentDirectory);
        else if (!strcmp(cmd, "exit"))
        {
            active = 0;
            printf(">exit\n>...");
        }
        else if (!strcmp(cmd, "help"))
        {
            printf(">help"
                   "\n>mkdir dirname\tCreates a directory at current directory with given name"
                   "\n>chdir dirname\tSwitches to the directory with given name"
                   "\n>rmdir dirname\tRemoves the directory with given name"
                   "\n>dir\tDisplays parent directory and all child directories"
                   "\n>count\tDisplays the number of child directories in current directory",
                   "\n>help\tDisplays avaliable commands"
                   "\n>exit\tExits from the program");
        }
        else
        {
            printf("\nError : Invalid command. Try typing help to see avaliable commands");
        }
    }
}