# Student-Report-Management-System
This is a simple C console-based application that manages student academic records using text-file storage.
It supports role-based login (Admin, Staff, Guest) and provides different operations depending on the user’s access level.
The system allows the user to add, view, search, update, and (for admin only) delete student records, all stored in a persistent text file.
Features
Role-Based Login
Admin: Add, View, Search, Update, Delete students
Staff: Add, View, Search, Update students
Guest: View and Search only
Credentials are stored in credentials.txt.
Student Record Functions
Add Student — Enter registration number, name, branch, CGPA, year/sem
Display All Students — View all records in a tabular format
Search Student — Search only by registration number
Update Student Details — Modify existing records
Delete Student — Admin-only permission
Exit — Safely close the program
All records are saved in students.txt.
Technologies Used
Language: C
Concepts: File Handling, Dynamic Memory Allocation, String Parsing
Data Storage: Text files (students.txt, credentials.txt)
User Access System: Role-based permissions
Project File Structure
Student_Report_Management_System/
│── srms.c
│── srms.exe          (generated after compilation)
│── credentials.txt   (stores usernames, passwords, roles)
│── students.txt      (stores student records)
How to Run
Clone or download this repository.
Compile the program using GCC:
gcc srms.c -o srms
Run the program:
./srms
Login using one of the default credentials:
Username	Password	Role
admin	adminpass	admin
staff	staffpass	staff
guest	guestpass	guest
Data Format
credentials.txt
username,password,role
students.txt
REG001, John Doe, CSE, 8.5, 2-1
