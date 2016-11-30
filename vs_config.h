#ifndef _H_VS_CONFIG_
#define _H_VS_CONFIG_

typedef struct _vs_config_node {
    char *key;
    char *value;
    struct _vs_config_node *left;
    struct _vs_config_node *right;
} vs_config_node;

typedef struct {
    vs_config_node* data;
} vs_config;

/**
 * Load config from file
 */
vs_config *vs_load_config(const char *file);

char *vs_config_get(vs_config *config, const char *key);

vs_config_node *vs_config_get_node(vs_config *config, const char *key);

void vs_config_set(vs_config *config, const char *key, const char *value);

void vs_config_destory(vs_config *config);

void vs_config_destory_node(vs_config_node *node);

int vs_config_get_int(vs_config *config, const char *key);

int vs_config_get_bool(vs_config *config, const char *key);

double vs_config_get_double(vs_config *config, const char *key);

#define vs_config_is_comment(r) (r == '#')
#define vs_config_is_delimiter(r) (r == '=')
#define vs_config_is_space(r) (r == ' ' || r == '\t')
#define vs_config_is_newline(r) (r == '\r' || r == '\n' || r == '\0')

#endif
