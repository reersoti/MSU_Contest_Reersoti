#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "os_file.h"

char root = '/';

char abs_path[128]; // абсолютный путь

char rel_path[128]; // относительный путь

char now_path[128]; // нынешний путь

int GLOBAL_DISK_SIZE = 0; // глобальный размер диска

typedef struct elem {
	char name[32];
	int isFile;
	elem* mass_pointers;
	elem* prev;
	elem* next;
}elem;

//*************************************

int my_create(int disk_size);

int my_destroy(); // нуждается в доработке

//int my_create_dir(const char* path);

//int my_create_file(const char* path, int file_size);

//int my_remove(const char* path, int recursive);

//int my_change_dir(const char* path);

//void my_get_cur_dir(char* dst);

//int my_move(const char* old_path, const char* new_path);

//*************************************

void setup_file_manager(file_manager_t* fm)
{
	fm->create = my_create;
	fm->destroy = my_destroy;
	//fm->create_dir = my_create_dir;
	//fm->create_file = my_create_file;
	//fm->remove = my_remove;
	//fm->change_dir = my_change_dir;
	//fm->get_cur_dir = my_get_cur_dir;
	//fm->move = my_move;
}

//*************************************

int my_create(int disk_size)
{
	if (disk_size && GLOBAL_DISK_SIZE == 0)
	{
		GLOBAL_DISK_SIZE = disk_size;
		now_path[0] = root;
		return 1;
	}
	return 0;
}

elem* new_dir()
{
	elem* newnote = (elem*)malloc(sizeof(elem));
	
}


int my_destroy() // не доделана, нужно рекурсивное удаление
{
	if (GLOBAL_DISK_SIZE)
	{
		GLOBAL_DISK_SIZE = 0;
		return 1;
	}
	return 0;
}

int main()
{
	my_create(10);
	my_create(10);
	my_destroy();
	return 0;
}