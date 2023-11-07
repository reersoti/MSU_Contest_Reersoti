#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "os_mem.h"
#pragma warning(disable:4996)

typedef struct element
{
	char name[32];
	int numb_of_links;
	element* links[16];

}element;

element* ARR = NULL;
int NUM_OBJ_MAX = 0;
int NUM_OBJ_CURR = 0;
// Инициализация вспомогательных функций
void copy_plus_vibe(element* destination, element* old_arr); // очень вайбовая фукнция, спасла весь код

int checkMemoryManagerAvilability(int new_elem); // делает основные проверки

void sort_strings_in_element(element* arr, int size); // сортирует строки 

void good_realloc(int new_size, int old_size); // нормальный реаллок без утечек (в теории)

int find_element(element* arr, const char* name, int arr_size); // возвращает индекс элемента

void destroy_elem_and_shift_array(int index, int size); // удаляет с массива элемент
// ************************************


// Инициализация моих функций
int my_create(int size, int num_pages);

int my_destroy();

int my_create_object(const char* name);

int my_destroy_object(const char* name);

void my_print_objects();

int my_link(const char* object1_name, const char* object2_name);

void print_link_counts();
// ************************************
// Инициализация мемори менеджера

void setup_memory_manager(memory_manager_t* mm) {
    mm->create = my_create; 
    mm->destroy = my_destroy; 
	mm->create_object = my_create_object;
	mm->destroy_object = my_destroy_object;
	mm->print_objects = my_print_objects;
	mm->link = my_link;
	mm->print_link_counts = print_link_counts;
}

// ************************************

// мейн ого
// ************************************

// Работа вспомогательных функций

void copy_plus_vibe(element* destination, element* old_arr)
{
	for (int j = 0; j < old_arr->numb_of_links; j++)
	{
		destination->links[j] = old_arr->links[j];
	}
	strcpy(destination->name, old_arr->name);
	destination->numb_of_links = old_arr->numb_of_links;
}

int checkMemoryManagerAvilability(int new_elem)
{
	return ((NUM_OBJ_MAX) && (NUM_OBJ_CURR + new_elem <= NUM_OBJ_MAX));
}

void good_realloc(int new_size, int old_size)
{
	if (!old_size)
	{
		// Если создается первый элемент
		ARR = (element*)malloc(sizeof(element));
		return;
	}
	/*element* new_arr = (element*)malloc(sizeof(element) * old_size);
	for (int i = 0; i < old_size; i++)
	{
		copy_plus_vibe(&new_arr[i], &ARR[i]);
	}
	free(ARR);

	ARR = (element*)malloc(sizeof(element) * new_size);
	for (int i = 0; i < old_size; i++)
	{
		copy_plus_vibe(&ARR[i], &new_arr[i]);
	}
	free(new_arr);*/
	element* new_arr = (element*)malloc(sizeof(element) * new_size);
	for (int i = 0; i < old_size; i++)
	{
		copy_plus_vibe(&new_arr[i], &ARR[i]);
	}
	free(ARR);
	ARR = new_arr;
}

void sort_strings_in_element(element* arr, int size) {
	for (int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			if (strcmp(arr[i].name, arr[j].name) > 0) {
				// Если текущая строка больше следующей, меняем их местами
				element temp;
				copy_plus_vibe(&temp, &arr[i]);
				copy_plus_vibe(&arr[i], &arr[j]);
				copy_plus_vibe(&arr[j], &temp);
			}
		}
	}
}

int find_element(element* arr, const char* name, int arr_size)
{
	int i = 0;
	for (; i < arr_size; i++)
	{
		if (!strcmp(arr[i].name, name))
		{
			return i;
		}
	}
	return -1;
}

void destroy_elem_and_shift_array(int index, int size)
{
	int shift = 0;
	int new_size = size - 1;
	element* new_arr = (element*)malloc(sizeof(element) * (new_size));
	for (int i = 0; i < size; i++)
	{
		if (i == index)
		{
			shift++;
			element* link = ARR[i].links[0];
			int j = 0; // check
			while (link)
			{
				link[j].numb_of_links--;
				j++;
			}
			continue;
		}
		copy_plus_vibe(&new_arr[i], &ARR[i]);
	}
	free(ARR);
	ARR = (element*)malloc(sizeof(element) * (new_size));
	for (int i = 0; i < new_size; i++)
	{
		copy_plus_vibe(&ARR[i], &new_arr[i]);
	}
	free(new_arr);
}
// ************************************

// Работа моих функций
int my_create(int size, int num_pages)
{
	if (NUM_OBJ_MAX) return 0;
	NUM_OBJ_MAX = size;
	return 1;
}

int my_destroy()
{
	if (!checkMemoryManagerAvilability(0)) return 0;
	NUM_OBJ_MAX = 0;
	free(ARR);
	return 1;
}

int my_create_object(const char* name)
{
	if (!checkMemoryManagerAvilability(1)) return 0;
	int new_size = NUM_OBJ_CURR + 1;
	good_realloc(new_size, NUM_OBJ_CURR);
	for (int i = 0; i < 16; i++)
	{
		ARR[new_size - 1].links[i] = NULL;
	}
	ARR[new_size - 1].numb_of_links = 0;
	/*for (int i = 0; i < lenght; i++)
	{
		ARR[new_size - 1].name[i] = name[i];
	}*/
	strcpy(ARR[new_size - 1].name, name);
	NUM_OBJ_CURR++;
	return 1;
}

int my_destroy_object(const char* name)
{
	if (!checkMemoryManagerAvilability(0)) return 0;
	int elem_index = find_element(ARR, name, NUM_OBJ_CURR);
	if (elem_index == -1) return 0;
	destroy_elem_and_shift_array(elem_index, NUM_OBJ_CURR);
	NUM_OBJ_CURR--;
	return 1;
}

int my_link(const char* object1_name, const char* object2_name)
{
	if (!checkMemoryManagerAvilability(0)) return 0;
	int parent_index = find_element(ARR, object1_name, NUM_OBJ_CURR);
	int child_index = find_element(ARR, object2_name, NUM_OBJ_CURR);
	if ((parent_index == -1) || (child_index == -1)) return 0;
	//element* link = ARR[parent_index].links[0]; // check
	//int j = 0;
	//while (link)
	//{
	//	j++;
	//	link = ARR[parent_index].links[j];
	//}
	//ARR[parent_index].links[j] = &ARR[child_index];
	//if (j < 16)
	//{
	//	ARR[parent_index].links[j] = NULL;
	//}
	element* link = ARR[parent_index].links[0]; // Начнем с первого элемента
	int j = 0;
	while (link) 
	{
		j++;
		if (j >= 16) 
		{
			// В массиве links не осталось доступных мест
			break;
		}
		link = ARR[parent_index].links[j];
	}
	if (j < 16) 
	{
		ARR[parent_index].links[j] = &ARR[child_index]; // Установить на ARR[child_index]
	}
	ARR[child_index].numb_of_links++;
	return 1;
}

void my_print_objects()
{
	if (!checkMemoryManagerAvilability(0)) return;
	sort_strings_in_element(ARR, NUM_OBJ_CURR);
	for (int i = 0; i < NUM_OBJ_CURR; i++)
	{
		printf("%s\n", ARR[i].name);
	}
}

void print_link_counts()
{
	if (!checkMemoryManagerAvilability(0)) return;
	sort_strings_in_element(ARR, NUM_OBJ_CURR);
	for (int i = 0; i < NUM_OBJ_CURR; i++)
	{
		printf("%s,%d\n", ARR[i].name, ARR[i].numb_of_links);
	}
}
// ************************************