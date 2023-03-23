1. a. False

   b. False

   c. False

   d. True 

2. a. Data structure is simple and the whole system is not expected to change.

   b. It is very expansive and hard to maintain because of the complexity and it is difficult to provide a unified view

   c. It will be of benifit for the encryption for keeping the data security and good for the concurrency.

3. a. D

   b. D

   c. A

   d. D

4. a. 

   b. Minimum number 1; Maximum number n. Because E1 to En is entity sets, which means there is no duplicates in the sets. 

5. A

6. ![IMG_7119AF2F208B-1](/Users/jack/Downloads/IMG_7119AF2F208B-1.jpeg)

7. ![IMG_A1EAC8548D45-1](/Users/jack/Downloads/IMG_A1EAC8548D45-1.jpeg)

   ![IMG_8FA2CB99A390-1](/Users/jack/Downloads/IMG_8FA2CB99A390-1.jpeg)

8. a. ![IMG_A3484139BAC8-1](/Users/jack/Downloads/IMG_A3484139BAC8-1.jpeg)

   ![IMG_FBB3AFADF278-1](/Users/jack/Downloads/IMG_FBB3AFADF278-1.jpeg)

   b. 

   It is not possible. As shown in the figure, if storing the mapping information, $a_4$ and $b_4$ will be lost. Hence, information will be lost if not creating a new relation.![IMG_2EAB5252A8D1-1](/Users/jack/Downloads/IMG_2EAB5252A8D1-1.jpeg)

9. a. $\pi_{barName}(Bar*_{Bar.barName = Frequent.barName}Frequent)$

   b. $\pi_{drinkerName}(Drinker*_{Drinker.street=Bar.street}(Bar*_{Bar.barName = Frequent.barName}Frequent))$

   c. $\pi_{barName}(\sigma_{age<37}(Drinker*_{Drinker.drinkerName=Frequent.drinkerName}(Bar*_{Bar.barName = Frequent.barName}Frequent)))$

10. a.

    $\{t.name|Student(t)\; AND\; (\exist s)(\exist c)(\exist r)(Student(s) \; AND\; Course(c) \; AND\; Registered(r)\; AND\; \\r.Code=c.Code\; AND\; r.SSN=s.SSN\; AND\; c.Title=\text{"Database System"}  \}$

    b. 

    ```sql
    Select c.title, count(s.Name)
    From Course c, student s, registered r
    Where r.code =c.code AND
    		r.SSN=s.SSN 
    Group by c.code desc
    ```

    c.

    ```sql
    Select c.title, max(s.gpa)
    From Course c, student s, registered r
    Where r.code =c.code AND
    		r.SSN=s.SSN 
    Group by c.Dept 
    Having count(c.code)>30)
    ```

    d. Retrieve the title of the course who have more registered students than the maximum number of student who participates in the course in other departments