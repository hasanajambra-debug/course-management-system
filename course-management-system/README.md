# Course Management System (C)

A console-based course management system written in C. It supports three user roles (administrator, teacher, student) and stores all data in binary files, so everything is saved between runs.

> **Author:** Ambra Hasanaj, 2025
> **Language:** C (the source file is `C_Ambra_Hasanaj_2025.cpp`; it compiles with both `gcc` and `g++`)

## Features

### Administrator
- Add, modify, delete and list users
- Add, modify, delete and list courses
- Manage the subjects (lessons) inside each course

### Teacher
- Add, modify and delete courses
- Manage the subjects inside a course

### Student
- Register for a course
- Mark subjects as completed
- View progress (as a percentage) for each course

### General
- Self-registration as a teacher or student
- Login / logout with username and password
- Persistent storage in binary files

## Limits

| Item                       | Maximum |
|----------------------------|---------|
| Users                      | 100     |
| Courses                    | 50      |
| Subjects per course        | 10      |

## Data files

The program creates these files automatically in the folder where it runs:

| File            | Contents                          |
|-----------------|-----------------------------------|
| `listaUser.bin` | Users                             |
| `grupet.bin`    | Courses and their subjects        |
| `progres.bin`   | Student progress per course       |

## Default administrator account

On the first run, an administrator account is created:

- **Username:** `admin`
- **Password:** `admin123`

> ⚠️ Change this before using the program for anything real. Passwords are currently stored in plain text, so this project is meant for learning purposes only.

## Build and run

You need a C compiler such as `gcc`.

```bash
# Compile
gcc -x c C_Ambra_Hasanaj_2025.cpp -o course_manager

# Run (Linux / macOS)
./course_manager

# Run (Windows)
course_manager.exe
```

Alternatively, with `g++`:

```bash
g++ C_Ambra_Hasanaj_2025.cpp -o course_manager
```

## Project structure

```
.
├── C_Ambra_Hasanaj_2025.cpp   # Source code
├── README.md                  # This file
└── .gitignore                 # Ignores binaries and data files
```

## Possible improvements

- Hash passwords instead of storing them in plain text
- Use dynamic memory instead of fixed-size arrays
- Split the code into several files (users, courses, progress)
- Add input validation for menu choices

## License

This project was created for educational purposes. Add a license of your choice (for example MIT) if you want others to reuse it.
