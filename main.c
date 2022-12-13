#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// cтруктура одного запроса
typedef struct task {
    int row;
    int cupboard;
    int place;
    int id;
} book;
// количество потоков
const int threadNumber = 16;
// массив запросов
book *book_array;
// каталог в библиотеке
int catalog[15][30][35];
// размер каталога
int size;

void *drafting(void *param) {
    // смещение, высчитываемое для каждого потока, чтобы они работали с указанными диапазонами
    // запросов одновременно
    int shift = size / (threadNumber - 1);
    // вывод, для наглядности работы потоков
    printf("\nThread %d doing work", *(int *)param);
    // в данной программе выбираю threadNumber - 1 поток для вычислений диапазанов длиной size /
    // (threadNumber - 1), последний поток оставляю для оставшихся запросов, которые не были
    // распределены между (threadNumber - 1) потоками.
    if (*(int *)param == threadNumber - 1) {
        int shiftspecial = size % (threadNumber - 1);
        int specialp = ((*(int *)param)) * shift;
        for (int i = specialp; i < specialp + shiftspecial; i++) {
            catalog[book_array[i].row][book_array[i].cupboard][book_array[i].place] =
                book_array[i].id;
        }
    } else {
        int p = (*(int *)param) * shift;
        for (int i = p; i < p + shift; i++) {
            catalog[book_array[i].row][book_array[i].cupboard][book_array[i].place] =
                book_array[i].id;
        }
    }
}

int randomNum() {
    int value = rand() % 15730 + 1;
    return value;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    size = 0;
    int fileprint = 0;
    int commandinput = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    // массив из индикаторов на уникальность чисел
    char identify[15750];
    // инициализирую индикатор нулями, чтобы не попадалось одинаковых значений
    for (int i = 0; i < 15750; ++i) {
        identify[i] = 0;
    }
    // обнуляю весь массив
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 30; ++j) {
            for (int k = 0; k < 35; ++k) {
                catalog[i][j][k] = 0;
            }
        }
    }
    // произвожу считывание запросов в командную строку
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "file") == 0) {
            infile = fopen(argv[i + 1], "r");
            if ((infile == NULL)) {
                printf("Cannot open input file.\n");
                return 1;
            }

            outfile = fopen(argv[i + 2], "w");
            if (outfile == NULL) {
                printf("Cannot open output file.\n");
                fclose(infile);
                return 1;
            }
            fileprint++;
        }

        if (strcmp(argv[i], "command") == 0) {
            sscanf(argv[i + 1], "%d;", &size);
            book_array = (book *)malloc(size * sizeof(book));
            for (int j = 0; j < size; ++j) {
                sscanf(argv[i + j + 2], "%d:%d:%d:%d", &book_array[j].row, &book_array[j].cupboard,
                       &book_array[j].place, &book_array[j].id);
            }
            ++commandinput;
        }

        if (strcmp(argv[i], "random") == 0) {
            int tester;
            size = randomNum() % 300 + 1;
            printf("Number of new requests: %d\n", size);
            book_array = (book *)malloc(size * sizeof(book));
            for (int j = 0; j < size; ++j) {
                book_array[j].row = randomNum() % 15;
                book_array[j].cupboard = randomNum() % 30;
                book_array[j].place = randomNum() % 35;
                // проверка на то, что в одну и ту же ячейку не могут попасть разные книги(из логики
                // задачи)
                tester = randomNum();
                while ((int)identify[tester]) {
                    tester = randomNum();
                }
                book_array[j].id = tester;
                identify[tester] = 1;
            }
            ++commandinput;
        }
    }
    // если ввод происходит не из командной строки
    if (commandinput == 0) {
        // ввожу количество запросов в библиотеку
        fscanf(infile, "Requests:%d;", &size);
        // массив, в котором записаны запросы на внесению в каталог
        book_array = (book *)malloc(size * sizeof(book));
        // заполняю массив, в котором хранятся запросы
        for (int i = 0; i < size; ++i) {
            fscanf(infile, " row:%d:cupboard:%d:place:%d:id:%d;", &book_array[i].row,
                   &book_array[i].cupboard, &book_array[i].place, &book_array[i].id);
        }
    }

    // инициализирую массив потоков
    pthread_t thread[threadNumber];
    // инициализирую массив номеров потоков
    int number[threadNumber];

    // создание 16 дочерних потоков
    for (int i = 0; i < threadNumber; i++) {
        number[i] = i;  // для передачи параметра потоку
        pthread_create(&thread[i], NULL, drafting, (void *)(number + i));
    }
    // ожидание завершения работы дочерних потоков
    for (int i = 0; i < threadNumber; i++) {
        pthread_join(thread[i], NULL);
    }

    // вывожу результат составления каталога
    printf("\n\nIn the assembled catalog there are books:\n");
    if (fileprint) {
        fprintf(outfile, "In the assembled catalog there are books:\n");
    }
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 30; ++j) {
            for (int k = 0; k < 35; ++k) {
                if (catalog[i][j][k]) {
                    printf("Book with id:%d\n", catalog[i][j][k]);
                    if (fileprint) {
                        fprintf(outfile, "Book with id:%d\n", catalog[i][j][k]);
                    }
                }
            }
        }
    }
    // закрываю поданные файлы
    if (fileprint) {
        fclose(outfile);
        fclose(infile);
    }
    // освобождаю память
    free(book_array);
    return 0;
}