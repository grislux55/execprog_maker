#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

static const char *WRITE_ERR = "An error occured when writing to file!\n",
                  *ARG_ERR = "Plz gib me correct args.\n",
                  *FILE_ERR = "File open error!\n";

static const char *
    META_INFO = "// Created from %s\n\n",
   *SUB_ARRAY_META = "static const unsigned char sub%d[] __initconst = {\n    ",
   *ALL_ARRAY_META =
       "static const unsigned char* const primary[] __initconst = {\n    ";

static const char *NEXT_LINE = "\n};\n\n",
                  *L_INDEX = "static const int last_index __initconst = %d;\n",
                  *L_ITEM = "static const int last_items __initconst = %d;\n";

static FILE *from = NULL, *to = NULL;

inline void clean(void) {
    if (NULL != to) {
        fclose(to);
    }
    if (NULL != from) {
        fclose(from);
    }
}

template <class... Args>
inline void write_into(FILE* to, const char* format, Args... tail) {
    if (fprintf(to, format, tail...) <= 0) {
        cerr << WRITE_ERR;
        clean();
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << ARG_ERR;
        return 1;
    }

    from = fopen(argv[1], "r");
    to = fopen(argv[2], "w+");
    if (NULL == from || NULL == to) {
        cerr << FILE_ERR;
        clean();
        return 1;
    }

    unsigned char buffer[4096]{};

    write_into(to, META_INFO, argv[1]);
    int outer, last_items = 0;
    for (outer = 0;; outer++) {
        memset(buffer, 0, sizeof(buffer));

        int tmp;
        if ((tmp = fread(buffer, sizeof(unsigned char), 4096, from)) <= 0) {
            break;
        }
        last_items = tmp;

        write_into(to, SUB_ARRAY_META, outer);
        for (int i = 0; i < last_items; i++) {
            write_into(to, "%hhu, ", *(buffer + i));
        }
        write_into(to, NEXT_LINE);
    }
    write_into(to, ALL_ARRAY_META);
    for (int i = 0; i < outer; i++) {
        write_into(to, "sub%d, ", i);
    }
    write_into(to, NEXT_LINE);
    write_into(to, L_INDEX, outer);
    write_into(to, L_ITEM, last_items);

    clean();
}
