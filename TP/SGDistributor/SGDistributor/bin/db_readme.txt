SGD的DB配置：
sqlite表:sg_ipurl_map
必填字段:sg_internetip(SG的外网IP),sg_localip(SG的内网IP),sg_localport(SG的PORT),url(SG对应的域名)
例子:
INSERT INTO sg_ipurl_map(sg_internetip,sg_localip,sg_localport,url) 
values("35.154.44.66","172.18.0.5",5011,"sg1.dns-proxy.com");
