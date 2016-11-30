vs-config
=========
A very simple config parser written in C.


Config format
=========
```
# Normal example
mysql.host = 127.0.0.1
mysql.port = 3306
mysql.user = test
mysql.pass = secret


# Boolean example
# True: 1, yes, y, enable, enabled, on
switch = on
simple = true
really = 1
pretty = yes

# False: 0, no, n, disable, disabled, off, etc.
display = no
```


Example
=========
```
#include "vs_config.h"

vs_config *config = vs_load_config("config_file");

vs_config_get(config, "foo");
vs_config_get_int(config, "bar");
vs_config_get_bool(config, "fox");

vs_config_destory(config);
```


Functions
=========
## Load config from file
```
vs_config *vs_load_config(const char *file)
```

## Destory vs_config instance
```
void vs_config_destory(vs_config *config)
```

## Get value of specified key
```
char *vs_config_get(vs_config *config, const char *key)
int vs_config_get_bool(vs_config *config, const char *key)
int vs_config_get_int(vs_config *config, const char *key)
double vs_config_get_double(vs_config *config, const char *key)
```
