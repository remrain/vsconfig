#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vs_config.h"

static char *vs_load_file(const char *file, size_t *len) {
    struct stat file_stat;
    FILE *fp;
    char *buff;

    fp = fopen(file, "rb");
    if (fp == NULL) {
        return NULL;
    }

    if (fstat(fileno(fp), &file_stat) < 0) {
        fclose(fp);
        return NULL;
    }
    *len = (size_t)file_stat.st_size;
    
    buff = (char *)malloc(*len + 1);
    *len = fread(buff, 1, *len, fp);
    buff[*len] = '\0';
    fclose(fp);
    return buff;
}

vs_config *vs_load_config(const char *file) {
    size_t len, i, k_start, k_end, v_start, v_end;
    char *buff, r;
    int key_begin = 0, key_end = 0, value_begin = 0, is_comment = 0;
    vs_config *config;
    buff = vs_load_file(file, &len);
    if (buff == NULL) {
        return NULL;
    }
    config = (vs_config *)calloc(1, sizeof(vs_config));
    for (i = 0; i < len; i++) {
        r = buff[i];
        if (vs_config_is_newline(r)) {
            if (value_begin) {
                buff[k_end + 1] = '\0';
                buff[v_end + 1] = '\0';
                vs_config_set(config, &buff[k_start], &buff[v_start]);
            }
            is_comment = 0;
            key_begin = 0, key_end = 0, value_begin = 0;
        } else if (is_comment) {
            // do nothing
        } else {
            if (vs_config_is_comment(r) && !key_begin) {
                is_comment = 1;
            } else if ((!key_begin) && vs_config_is_space(r)) {
                // do nothing
            } else if (!key_begin) {
                key_begin = 1;
                k_start = i;
                k_end = i;
            } else if (!key_end && vs_config_is_delimiter(r)) {
                key_end = 1;
            } else if (!key_end && vs_config_is_space(r)) {
                // do nothing
            } else if (!key_end && !vs_config_is_space(r)) {
                k_end = i;
            } else if (!value_begin && vs_config_is_space(r)) {
                // do nothing
            } else if (!value_begin) {
                value_begin = 1;
                v_start = i;
                v_end = i;
            } else if (!vs_config_is_space(r)) {
                v_end = i;
            }
        }
    }
    free(buff);
    return config;
}

char *vs_config_get(vs_config *config, const char *key) {
    vs_config_node *r;
    r = vs_config_get_node(config, key);
    if (r) {
        return r->value;
    }
    return NULL;
}

int vs_config_get_int(vs_config *config, const char *key) {
    char *value = vs_config_get(config, key);
    if (value == NULL) {
        return 0;
    }
    return atoi(value);
}

int vs_config_get_bool(vs_config *config, const char *key) {
    char *value = vs_config_get(config, key);
    if (value == NULL) {
        return 0;
    }
    if (strcasecmp(value, "yes") == 0 || strcasecmp(value, "true") == 0 || strcasecmp(value, "enable") == 0
            || strcasecmp(value, "1") == 0 || strcasecmp(value, "on") == 0
            || strcasecmp(value, "y") == 0 || strcasecmp(value, "enabled") == 0) {
        return 1;
    }
    return 0;
}

double vs_config_get_double(vs_config *config, const char *key) {
    char *value = vs_config_get(config, key);
    if (value == NULL) {
        return 0;
    }
    return atof(value);
}

vs_config_node *vs_config_get_node(vs_config *config, const char *key) {
    vs_config_node *r;
    r = config->data;
    int cmp;
    while (r) {
        cmp = strcmp(key, r->key);
        if (cmp < 0) {
            r = r->left;
        } else if (cmp > 0) {
            r = r->right;
        } else {
            break;
        }
    }
    return r;
}

void vs_config_set(vs_config *config, const char *key, const char *value) {
    vs_config_node *pre = NULL, *r;
    int cmp = 0;
    r = config->data;
    while (r) {
        pre = r;
        cmp = strcmp(key, r->key);
        if (cmp < 0) {
            r = r->left;
        } else if (cmp > 0) {
            r = r->right;
        } else {
            free(r->value);
            r->value = strdup(value);
            return;
        }
    }
    r = (vs_config_node *)calloc(1, sizeof(vs_config_node));
    r->key = strdup(key);
    r->value = strdup(value);
    if (cmp < 0) {
        pre->left = r;
    } else if (cmp > 0) {
        pre->right = r;
    } else {
        config->data = r;
    }
}

void vs_config_destory(vs_config *config) {
    if (config) {
        if (config->data) {
            vs_config_destory_node(config->data);
        }
        free(config);
    }
}

void vs_config_destory_node(vs_config_node *node) {
    if (node) {
        vs_config_destory_node(node->left);
        vs_config_destory_node(node->right);
        free(node);
    }
}

