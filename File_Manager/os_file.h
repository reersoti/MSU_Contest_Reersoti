#include <stdlib.h>
#include <memory.h>

// Файловый менеждер осуществляет операции с файлами и директориями. 
// Оперирует с виртуальным деревом каталогов и файлов, каждый файл или директория имеют имя,
// состоящее из латинских прописных и заглавных букв, цифр и символов '_' и '.'. Длина имени
// ограничена 32 символами. При этом длина полного абсолютного пути каждого файла или директории
// ограничена 128 символами. Файлы имеют некоторый ненулевой размер (в байтах). 
// Считаем, что директории размера не имеют, и их можно создавать в любом количестве.
// Корневая директория '/' создается автоматически и не может быть удалена.
// Для идентификации файла или директории используется путь, состоящий из имен вложенных директорий,
// содержащих данный файл, и знаков '/', например /dir1/dir2/file.txt.
// Любой файл или директория доступны по относительным и абсолютным путям. Относительный путь
// считается относительно текущей директории, абсолютный путь начинается с корневой директории, т.е.
// '/', например /my_dir1/my_dir2/file.txt и my_dir2/file.txt указывают на один и тот же файл при
// условии, что /my_dir1 - текущая директория.
// При создании файлового менеджера корневая директория устанавливается текущей.
// Кроме имен директорий в пути могут присутствовать две специальные директории '.' и '..', первая из
// которых обозначает, что путь не меняется, а вторая означает, что путь должен пройти на директорию
// вверх, например /my_dir1/my_dir2/file.txt и ../my_dir2/file.txt являются одинаковыми при условии,
// что текущая директория имеет вид /my_dir1/my_dir3.
// В связи с этим запрещается создавать файлы или директории с именами . и .. (менеджер должен выдавать
// ошибку в случае попытки создать такой файл или директорию).
// При удалении или перемещении текущей директории корень становится текущей директорией
typedef struct
{
    // Создание файлового менеджера,
    // disk_size - размер диска
    // Возвращаемое значение: 1 - успех, 0 - неудача
    int (*create)(int disk_size);
    
    // Удаление файлового менеджера
	// Структура директорий, созданная на данный момент полностью удаляется
    // Возвращаемое значение: 1 - успех, 0 - неудача (например, менеждер не был создан)
    int (*destroy)();

    // Создает директорию с указанным именем.
	// В качестве аргумента принимает абсолютный или относительный путь 
    // Создание директории возможно только внутри уже существующей директории
    // Возвращаемое значение: 1 - успех, 0 - неудача (например, менеждер не был создан)
    int (*create_dir)(const char* path);

    // Создает файл с указанным именем и размером.
	// В качестве аргумента принимает абсолютный или относительный путь
    // Создание файла возможно только внутри уже существующей директории
    // Возвращаемое значение: 1 - успех, 0 - неудача (например, не хватило места)
    int (*create_file)(const char* path, int file_size);

    // Удаляет файл или директорию с указанным именем.
	// В качестве аргумента принимает абсолютный или относительный путь
    // Если указана непустая директория и флаг recursive равен 0, то директория не удаляется и выдается ошибка.
    // Если указана директория и флаг recursive равен 1, то директория удаляется вместе со всем ее содержимым.
	// Если получилось так, что текущая директория была удалена - текущей становится корневая директория.
    // Возвращаемое значение: 1 - успех, 0 - неудача
    int (*remove)(const char* path, int recursive);

    // Меняет текущую директорию
	// В качестве аргумента принимает абсолютный или относительный путь новой текущей директории
    // Возвращаемое значение: 1 - успех, 0 - неудача
    int (*change_dir)(const char* path);

    // Возвращает полный путь текущей директории (кладет в буфер dst)
    void (*get_cur_dir)(char *dst);

    // Перемещает (переименовывает) файл или директорию.
    // При попытке переместить файл или каталог в самого себя должен выдавать ошибку.
    // При попытке переименовать/переместить файл или каталог в существующий файл или каталог выдается ошибка.
    // Если получилось так, что текущая директория была перемещена, - текущей становится корневая директория
    // Возвращаемое значение: 1 - успех, 0 - неудача
    int (*move)(const char *old_path, const char *new_path);

} file_manager_t;

void setup_file_manager(file_manager_t *fm);