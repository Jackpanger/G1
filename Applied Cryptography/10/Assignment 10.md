**14- The Onion Router**

>The Onion Router provides confidentiality by separating identification and routing.
>
>Tor selects 3 nodes in the network by the path selection algorithm. These three nodes called Guard relay, Middle relay, and exit relay are used to encrypt the data at the application layer of the network connection. This method is called the onion router. Therefore, the guard relay only knows the user's IP but does not know the website they visit, while the exit relay only knows the data, but does not know the real user. Hence, it ensures confidentiality.

**15- Firewall Analysis**

Given the firewall rules presented in the table below, identify which of the following inbound packets will reach the server with IP address 1.1.1.200:

 (i) srcIP=9.9.9.9, srcPort=22, destIP=1.1.1.200, destPort=2500

>Port=3500 is Not allowed.

(ii) srcIP=3.0.3.3, srcPort=9876, destIP=1.1.1.200, destPort=443

>srcIP is not in IP range.

(iii) srcIP=3.0.3.3, srcPort=9876, destIP=1.1.1.200, destPort=80

>destIP is not in IP range.

(iv) srcIP=8.8.0.5, srcPort=9876, destIP=1.1.1.200, destPort=8888

>srcIP not 8.8.8.8/32.

(v) srcIP=8.8.8.8, srcPort=9876, destIP=1.1.1.200, destPort=22

>Can reach.