#include "aquarium_parser.h"
#include "Aquarium.h"
#include "config_parser.h"
#include "logger.h"

// parser : $(LIBDIR)/libset.a $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(BIN))) $(BUILDDIR)/aquarium_parser.o
// 	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o parser $(LDLIBS)

int count_lines(char *file_name)
{
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        log_message(WARN, "File name does not exist");
        return -1;
    }
    int c;
    int nLignes = 0;
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '\n')
            nLignes++;
    }
    fclose(f);
    return nLignes;
}

char **item_from_line(char str[])
{
    char **tableau = (char **)malloc(MAX_WORDS * sizeof(char *));
    char delim[] = " x+";
    int i = 0;
    char *p = strtok(str, delim);
    while (p != NULL)
    {
        tableau[i] = p;
        i++;
        p = strtok(NULL, delim);
    }
    return tableau;
}

char **to_array(char *file_name, int Nlines)
{
    FILE *f;
    f = fopen(file_name, "r");
    if (f == NULL)
    {
        log_message(WARN, "File name does not exist");
        return NULL;
    }
    char **tableau = (char **)malloc(MAX_LINES * sizeof(char *));
    char buffer[MAX_LENGTH];
    for (int i = 0; i < Nlines; i++)
    {
        fscanf(f, "%[^\n]\n", buffer);
        int len = strlen(buffer);
        tableau[i] = malloc(len * 2 * sizeof(char));
        strcpy(tableau[i], buffer);
    }
    fclose(f);
    return tableau;
}

int is_in_string(char *str, char letters[], char forbidden[])
{
    int occ = 0;
    int j = 0;
    int end_letters = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == letters[j])
        {
            occ++;
            if (letters[j + 1] != '\0')
            {
                j++;
                end_letters = 1;
            }
        }
        // checking if " x++" exactly
        else if (end_letters == 1)
        {
            for (int k = 0; letters[k] != '\0'; k++)
            {
                if (str[i] == letters[k])
                {
                    return 0;
                }
            }
        }
        // checking forbidden characters
        for (int l = 0; forbidden[l] != '\0'; l++)
        {
            if (str[i] == forbidden[l])
            {
                return 0;
            }
        }
    }
    return occ;
}

int parser(char *filename, int display)
{
    if (filename)
    {
        int Nlines = 1 + count_lines(filename);
        if (Nlines <= 0)
        {
            return 1;
        }
        int n_frames = Nlines - 1;
        int existing_frames[MAX_LINES][4] = {{0}};
        char **tableau = to_array(filename, Nlines);
        if (tableau == NULL)
        {
            return 1;
        }

        for (int i = 0; i < Nlines; i++)
        {
            int w_count = is_in_string(tableau[i], " x++", ",?;./:!*$^{([)]}=\\");
            if (w_count != 4 && i != 0)
            {
                log_message(CRITIC, "Error in Config_File parser : Error in input file.\n\tPlease enter frames like that :\n\tN1 vue_x x vue_y + vue_width + vue_height");
                n_frames--;
                free(tableau[i]);
            }
            else
            {
                char **items = item_from_line(tableau[i]);
                if (i == 0)
                {
                    Aquarium_create("aquarium", atoi(items[0]), atoi(items[1]), get_display_timeout());
                    if (display)
                        log_message(INFO, "Config_File parser : dim : %d %d\n", Aquarium_getWidth(aq), Aquarium_getHeight(aq));
                }
                else
                {
                    int already_exists = 0;
                    for (int j = 0; j < i - 2; j++)
                    {
                        if (existing_frames[j][0] == atoi(items[1]) &&
                            existing_frames[j][1] == atoi(items[2]) &&
                            existing_frames[j][2] == atoi(items[3]) &&
                            existing_frames[j][3] == atoi(items[4]))
                        {
                            already_exists += 1;
                        }
                    }
                    if (already_exists == 0)
                    {
                        // char *name = malloc(MAX_LENGTH * sizeof(char));
                        // sprintf(name, "%d", i);
                        char *name = items[0];
                        Aquarium_createDisplay(name, atoi(items[1]), atoi(items[2]), atoi(items[3]), atoi(items[4]));
                        existing_frames[i][0] = atoi(items[1]);
                        existing_frames[i][1] = atoi(items[2]);
                        existing_frames[i][2] = atoi(items[3]);
                        existing_frames[i][3] = atoi(items[4]);
                        if (display){
                            log_message(INFO, "Config_File parser : %s : %d * %d + %d + %d\n", name, atoi(items[1]), atoi(items[2]), atoi(items[3]), atoi(items[4]));
                            printf( "Config_File parser : %s : %d * %d + %d + %d\n", name, atoi(items[1]), atoi(items[2]), atoi(items[3]), atoi(items[4]));
                        }
                        // free(name);
                    }
                    else
                    {
                        n_frames--;
                        log_message(CRITIC, "Error in Config_File parser : 2 input frames are the same");
                    }
                }
                free(tableau[i]);
                free(items);
            }
        }
        log_message(INFO, "Config_File parser : Total : %d correct frames\n", n_frames);
        free(tableau);
        return 0;
    }
    else
    {
        log_message(CRITIC, "Error in Config_File parser : No file specified");
    }
    return 1;
}
