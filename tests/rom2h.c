#include <stdio.h>

int main()
{
    unsigned char buffer[0x10000] = {0};
    FILE *file;

    /* TODO: allow generic path rather than predefined */
    file = fopen("tests/cpu_instrs.gb", "rb");
    if (!file) {
        perror("failed to open cpu_instrs.gb\n");
        return -1;
    }

    fread(buffer, sizeof(buffer), 1, file);
    fclose(file);

    file = fopen("build/cpu_instrs.h", "wb");
    if (!file) {
        perror("failed to open build/cpu_instrs.h for generating.\n");
        return -1;
    }

    fputs("#ifndef CPU_INSTRS_ROM_H\n#define CPU_INSTRS_ROM_H\n", file);
    fputs("\nstatic unsigned char cpu_instrs_gb[] = {", file);
    for (int i = 0; i < sizeof(buffer); i++) {
        if ((i % 0x10) == 0)
            fputs("\n\t", file);
        fprintf(file, "0x%02X,", buffer[i]);
    }
    fputs("\n};\n", file);

    fputs("\n#endif /* CPU_INSTRS_ROM_H */\n", file);
    fclose(file);

    return 0;
}
