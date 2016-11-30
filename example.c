#include <stdio.h>
#include "vs_config.h"

int main(int argc, char *argv[]) {
    int i;
    vs_config *config;
    char *config_file = "example.conf";

    if (argc >= 2) {
        config_file = argv[1];
    }

    config = vs_load_config(config_file);

    if (config == NULL) {
        printf("Parse '%s' failed.\n", config_file);
        return -1;
    }

    if (argc > 2) {
        for (i = 2; i < argc; i++) {
            printf("%s = %s\n", argv[i], vs_config_get(config, argv[i]));
        }
    } else {
        printf("mysql.host = %s\n", vs_config_get(config, "mysql.host"));
        printf("mysql.port = %d\n", vs_config_get_int(config, "mysql.port"));

        if (vs_config_get_bool(config, "switch")) {
            printf("switch is on\n");
        } else {
            printf("switch is off\n");
        }
    }

    vs_config_destory(config);
    return 0;
}
