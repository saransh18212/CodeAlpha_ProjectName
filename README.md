# CodeAlpha C Programming Internship

This repository contains all four CodeAlpha internship tasks:

1. Basic Calculator Program
2. Matrix Operations
3. Student Management System
4. Banking System Mini Project

## Requirements

- A C compiler such as GCC, Clang, or MinGW
- C11 or newer
- A terminal opened in the repository folder

## Build and Run

Compile any program with:

```text
gcc -std=c11 -Wall -Wextra -pedantic calculator.c -o calculator
```

Then run it:

```text
./calculator
```

On Windows MinGW, run `calculator.exe` instead. Use the same pattern for the other files:

```text
gcc -std=c11 -Wall -Wextra -pedantic matrix_operations.c -o matrix_operations
gcc -std=c11 -Wall -Wextra -pedantic student_management.c -o student_management
gcc -std=c11 -Wall -Wextra -pedantic banking_system.c -o banking_system
```

The student and banking programs create their data files in the same folder when records are saved.

## Task Details

### Task 1: Basic Calculator

Performs addition, subtraction, multiplication, division, and remainder using a switch statement. Division by zero is rejected.

### Task 2: Matrix Operations

Supports matrix addition, multiplication, and transpose using fixed-size 2D arrays and separate functions.

### Task 3: Student Management System

Provides add, delete, update, search, and display operations. Student records are stored in `students.dat` using binary file handling.

### Task 4: Banking System

Provides account creation, deposit, withdrawal, balance enquiry, and account listing. Accounts are stored in `accounts.dat` using binary file handling.

## Submission Checklist

- [ ] Compile each selected task without warnings
- [ ] Run each selected task and test normal and invalid input
- [ ] Upload this complete folder to a GitHub repository named `CodeAlpha_ProjectName`
- [ ] Add the GitHub repository link to the CodeAlpha submission form
- [ ] Post a short demo video on LinkedIn and include the repository link
