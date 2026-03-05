##For Root

* del sdkconfig 
* idf.py fullclean 
* idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.root" build
* idf.py -p COM flash  

##For Node

* del sdkconfig 
* idf.py fullclean
* idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.node" build
* idf.py -p COM flash  
