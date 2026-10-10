# Sports Management System (C++)

## Requirements covered
- **5+ classes:** `Person`, `Coach`, `Player`, `Sport`, `Team`, `RegistrationTransaction`, `SportsManager`.
- **Constructors/destructors:** constructors throughout; virtual destructor in `Person`; explicit/default destructors in other classes.
- **Inheritance and polymorphism:** `Coach` and `Player` inherit `Person`; `displayDetails()` is virtual and overridden.
- **Dynamic memory:** `unique_ptr` and `make_unique` allocate objects dynamically; automatic cleanup avoids manual `delete`.
- **File persistence:** sports are loaded from and saved to `sports_data.txt`; transaction records append to `transactions.txt`.
- **CRUD/search:** add, display/read, search by ID, update, and delete sports.
- **Main transaction:** registering players updates the sport and writes a transaction log entry.
- **Diagrams:** see `diagrams/`.
- **15 test cases:** see `TEST_CASES.md`.
- **Executable and README:** compile instructions below; the executable is produced locally by the compiler.

## Compile and run

### Windows (MinGW g++)
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o sports_management.exe
sports_management.exe
```

### Linux/macOS
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o sports_management
./sports_management
```

The program creates/uses `sports_data.txt` and `transactions.txt` in its current working directory. Keep these files if you want to retain your data and transaction history.

## Important notes
- On first run, three sample sports are created if `sports_data.txt` does not contain any records.
- Use menu option 6 to register players; it is the main transaction and is logged.
- Option 10 demonstrates runtime polymorphism through base-class pointers.
- `Team` is included as a domain class to meet the class-count requirement; the core menu does not yet maintain a separate team registry.
- Diagrams are provided as PlantUML source files. Render them with a PlantUML tool for diagram images.
