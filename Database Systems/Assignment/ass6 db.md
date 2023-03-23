```sql
SELECT e.Fname, e.Lname
FROM (EMPLOYEE e JOIN WORKS_ON w on e.Ssn = w. Essn)
Join PROJECT p on w.Pno = p.Pnumber
WHERE p.name = 'ProductX' AND w.Hours>10 AND e.Dno = 5
```

```sql
SELECT e.Fname, e.Lname
FROM EMPLOYEE e Join DEPENDENT d on e.Ssn=d.Essn
WHERE e.Fname = d.Dependent_name
```

```sql
SELECT Fname, Lname
FROM EMPLOYEE
WHERE Super_ssn = 
(SELECT e.Ssn
FROM EMPLOYEE e
WHERE e.Fname ='Franklin' AND e.Lname = 'Wong')
```

```sql
CREATE TABLE EMPLOYEE_BACKUP LIKE EMPLOYEE 
(SELECT E.*
FROM EMPLOYEE AS E
) WITH DATA;
```

```sql
SELECT t.*
FROM TRIP t Join EXPENSE e on t.Trip_id = e.Trip_id
WHERE e.Amount > 2000
```

```sql
SELECT p.Ssn
FROM SALESPERSON p Join TRIP t on p.Ssn = t.Ssn
WHERE t.To_city = 'Honolulu'
```

```sql
SELECT sum(e.Amount)
FROM TRIP t Join EXPENSE e on t.Trip_id = e.Trip_id
WHERE t.Ssn = '234-56-7890' 
```

6.

```sql
SELECT d.Dname, count(*)
FROM EMPLOYEE e Join DEPARTMENT d on e.Dno = d.Dnumber
Group by d.Dname
Having AVG(e.Salary)>30000
```

```sql
SELECT d.Dname, count(*)
FROM EMPLOYEE e Join DEPARTMENT d on e.Dno = d.Dnumber
Where e.Sex='M'and d.Dnumber in 
(SELECT Dno
 FROM EMPLOYEE
Group by Dno
Having AVG(e.Salary)>30000)
Group by d.Dname
```

```sql
SELECT Name, Major
FROM STUDENT s
WHERE not exists (
SELECT * 
  FROM GRADE_REPORT g
  WHERE s.StudentNumber = g.Student_number and Not(g.Grade == 'A')
)
```

```sql
SELECT Name, Major
FROM STUDENT s
WHERE not exists (
SELECT * 
  FROM GRADE_REPORT g
  WHERE s.StudentNumber = g.Student_number and g.Grade == 'A'
)
```

```sql
SELECT Fname,Lname
FROM EMPLOYEE
WHERE Dno in (
SELECT Dno 
FROM EMPLOYEE 
WHERE Salary = 
  (SELECT Max(Salary) FROM EMPLOYEE)
)
```

```sql
SELECT Fname,Lname
FROM EMPLOYEE
WHERE Super_ssn in (
SELECT Ssn 
FROM EMPLOYEE 
WHERE Super_ssn = '888665555' 
)
```

```sql
SELECT Fname,Lname
FROM EMPLOYEE
WHERE Salary-1000 > (
SELECT min(Salary) 
FROM EMPLOYEE
)
```

```sql
CREATE VIEW DPM as 
SELECT d.Dname, e.Fname, e.Lname, e.Salary
From DEPARTMENT d, EMPLOYEE e
WHERE d.Mgr_ssn = e.Ssn
```

```sql
CREATE VIEW ESER as 
SELECT e1.Fname, e1.Lname, e2.Fname, e2.Lname, e1.Salary
From DEPARTMENT d, EMPLOYEE e1,EMPLOYEE e2
WHERE d.Dnumber = e1.Dno and d.Dname = 'Research', e2.Ssn = e1.Super_ssn
```

```sql
CREATE VIEW PCNT as 
SELECT p.Pname, d.Dname, count(w.Essn), sum(w.Hours)
From DEPARTMENT d, PROJECT p, WORKS_ON w
WHERE d.Dnumber = p.Dnum and p.Pnumber=w.Pno
Group by p.Pname
```

```sql
CREATE VIEW PCNTM as 
SELECT p.Pname, d.Dname, count(w.Essn), sum(w.Hours)
From DEPARTMENT d, PROJECT p, WORKS_ON w
WHERE d.Dnumber = p.Dnum and p.Pnumber=w.Pno
Group by p.Pname
Having count(w.Essn)>1
```

```sql
SELECT Dno, COUNT (*), SUM (Salary), AVG (Salary) 
FROM EMPLOYEE
GROUP BY Dno;
```

```sql
SELECT Dno, COUNT (*)
FROM EMPLOYEE
GROUP BY Dno;
Having  SUM (Salary) > 100000
```

```sql
SELECT Dno, AVG (Salary) 
FROM EMPLOYEE
GROUP BY Dno
Having Dno = 4;
```



final

2(a)

```sql
SELECT sum(f1) FROM
(SELECT count(s.Team_ID) as f1)
From Room r join StayingAt s on r.Hotel_ID=s.Hotel_ID
WHERE CostPerNight = (SELECT max(CostPerNight) from Room group by Hotel_ID)
Group by Hotel_ID
);
```

(b)

```sql
SELECT sum(*)*CostPerNight
From StayingAt s join Hotel h on h.Hotel_ID=s.Hotel_ID
Where Date_To='2022-10-16'
Group by Hotel_ID
```

3 

Person(ID, name) PK=(ID)

Emails(ID, email) PK=(ID, email), FK=(ID->Person)

Emplyee(ID, salary, joining_date) PK=(ID), FK=(ID->Person)

Customer(ID, membership_points) PK=(ID), FK=(ID->Person)

Order(order_number, ID, ID,total_cost, quantity, product_number, name) PK=(order_number), FK=(ID->Customer, ID->Employee, product_number->Product, name->Delivery_Company)

Product(product_number, name, price) PK=(product_number), FK=(ID->Customer, ID->Employee)

Delivery_Company(name, address) PK=(name)

4 

a. 

P\[Author_Name\](S\[CountryOfOrigin='Australia' and (Publisher_Name='Pearson' or year='2022')\](Book CJ[Publisher_ID] Publisher CJ[Title = Book_Title] WrittenBy CJ[Author_ID]  Author))

b.

P\[Publisher_Name\](Publisher) D P\[Publisher_Name\](S\[year='2022'\](Publisher CJ[Publisher_ID] Book))



8

```sql
CREATE ASSERTION budgetcheck CHECK 
((SELECT sum(salary) FROM Employee group by position-number)
< SELECT total-salary-budget FROM Job group by position-number)
```

