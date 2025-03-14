# Plagiarism Detection System

## Overview
This project is a **Plagiarism Detection System** developed using **C++ and the Qt Framework**. The system leverages advanced **string matching algorithms** and **data structures** to accurately detect plagiarism between multiple documents. It provides a **modern GUI**, secure **user authentication**, and detailed similarity analysis.

## Features
- **User Authentication**
  - Secure **registration and login**
  - Data persistence using CSV storage
  - Input validation and error handling

- **Document Management**
  - Upload multiple documents
  - Select a **target document** for plagiarism comparison
  - Supports various file formats (**TXT, PDF, DOC**)

- **Plagiarism Analysis**
  - Uses multiple string matching algorithms
  - Provides **percentage-based similarity scoring**
  - Displays **color-coded results** for easy analysis

## Technologies Used
- **Programming Language:** C++
- **GUI Framework:** Qt (QWidget, QLabel, QPushButton, etc.)
- **Data Structures:** AVL Trees, Hash Maps, Vectors
- **Algorithms Implemented:**
  - **TF-IDF (Term Frequency-Inverse Document Frequency)**
  - **N-gram Analysis**
  - **Cosine Similarity**
  - **KMP (Knuth-Morris-Pratt) Algorithm**
  - **Boyer-Moore Algorithm**

## Project Structure
```
📂 Plagiarism Detection System
├── 📜 main.cpp                 # Entry point of the application
├── 📜 mainwindow.h              # Main GUI header file
├── 📜 mainwindow.cpp            # GUI implementation file
├── 📜 PlagiarismChecker.h       # Plagiarism detection logic (Header)
├── 📜 PlagiarismChecker.cpp     # Plagiarism detection logic (Implementation)
├── 📜 mainwindow.ui             # Qt UI design file
├── 📜 Plagarism_detection.pro   # Qt Project file
├── 📜 user_data.csv             # User authentication data (if applicable)
└── 📜 README.md                 # Project documentation
```

## Installation & Setup
### Prerequisites
- **Qt Framework** (Qt Creator or Qt installed)
- **C++ Compiler** (MinGW, MSVC, or GCC)

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/plagiarism-detection-system.git
   ```
2. Open the project in **Qt Creator** or any Qt-compatible IDE.
3. Build and Run the project.

## How to Use
1. **Launch the application**.
2. **Register** a new user or **Login** with existing credentials.
3. **Upload multiple documents** to compare with a **target document**.
4. Click **Check Plagiarism** to analyze the similarities.
5. View results in a **color-coded table** showing plagiarism percentages and verdicts.

## Contributors
- **Hashmat Raza**
- **Syed Ali Hassan Shirazi**

## License
This project is licensed under the **MIT License**. Feel free to contribute and improve!

---
> *Maintaining academic integrity is crucial, and this tool aids in detecting and preventing plagiarism efficiently.*

