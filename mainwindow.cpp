#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QPalette>
#include <QFont>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <PlagiarismChecker.h>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileInfo>
QString csvFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/user_data.csv";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the background color of the main window to white (blank at the start)
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);  // Set the background to white
    setPalette(palette);

    // The window starts completely blank, no image or content at all

    // Create a QLabel instance dynamically for the logo
    QLabel *imageLabel = new QLabel(this);  // QLabel to display the image
    imageLabel->setAlignment(Qt::AlignCenter);  // Center the label in the window

    // Load an image using QPixmap
    QPixmap pixmap("D:/0x0.png");  // Use the correct image path

    // Set the image as the pixmap of the QLabel
    imageLabel->setPixmap(pixmap);

    // Optional: Scale the image to fit inside the QLabel
    imageLabel->setScaledContents(true);

    // Set a fixed size for the label (optional)
    imageLabel->setFixedSize(300, 300);  // Start with a fixed size for the logo

    // Create a layout to add the QLabel to the main window
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(imageLabel);  // Add the QLabel to the layout
    layout->setAlignment(Qt::AlignCenter);  // Center the label in the layout

    // Create a central widget and set the layout to it
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);  // Set the layout to the central widget
    setCentralWidget(centralWidget);  // Set the central widget of the main window

    // Initially hide the label so it is not visible at first
    imageLabel->setVisible(false);  // Hide the image initially (blank screen)

    // Apply opacity effect to the label for fade-in and fade-out
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    imageLabel->setGraphicsEffect(opacityEffect);

    // Create a fade-in animation (to make the logo appear)
    QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    fadeInAnimation->setDuration(1000);  // 1 second fade-in
    fadeInAnimation->setStartValue(0);   // Start with opacity 0 (invisible)
    fadeInAnimation->setEndValue(1);     // End with opacity 1 (fully visible)

    // Create a fade-out animation (to make the logo disappear)
    QPropertyAnimation *fadeOutAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    fadeOutAnimation->setDuration(1000);  // 1 second fade-out
    fadeOutAnimation->setStartValue(1);   // Start with opacity 1 (fully visible)
    fadeOutAnimation->setEndValue(0);     // End with opacity 0 (invisible)

    // Use a QTimer to start the fade-in animation after 1 second
    QTimer::singleShot(1000, [imageLabel, fadeInAnimation]() {
        imageLabel->setVisible(true);  // Make the image visible
        fadeInAnimation->start();  // Start fade-in after 1 second
    });

    // Use a QTimer to start the fade-out animation after 6 seconds (1s fade-in + 5s display time)
    QTimer::singleShot(6000, [fadeOutAnimation, this]() {
        fadeOutAnimation->start();  // Start fade-out after 6 seconds
        QTimer::singleShot(1000, this, &MainWindow::showRegistrationPage); // Show registration page after the fade-out
    });
}

void MainWindow::showRegistrationPage()
{
    // Create the registration form layout
    QWidget *registrationWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(registrationWidget);

    // Title Label (Registration & Login)
    QLabel *titleLabel = new QLabel("Registration and Login", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));  // Large title font
    titleLabel->setStyleSheet("QLabel { color: #333333; padding-bottom: 20px; }");  // Style the title
    mainLayout->addWidget(titleLabel);

    // Add some space before the form
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Create a form layout for the registration inputs
    QFormLayout *formLayout = new QFormLayout();

    // Set the font for the form labels
    QFont labelFont("Arial", 12);
    QPalette labelPalette;
    labelPalette.setColor(QPalette::WindowText, Qt::black);  // Set text color to black for visibility
    labelFont.setBold(true);  // Set bold font for labels

    // Create input fields for registration
    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *fatherNameEdit = new QLineEdit();
    QLineEdit *phoneNumberEdit = new QLineEdit();
    QLineEdit *emailEdit = new QLineEdit();
    QLineEdit *usernameEdit = new QLineEdit();
    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);  // Hide password input

    // Add form inputs with labels before each field
    formLayout->addRow(new QLabel("Name:"), nameEdit);
    formLayout->addRow(new QLabel("Father's Name:"), fatherNameEdit);
    formLayout->addRow(new QLabel("Phone Number:"), phoneNumberEdit);
    formLayout->addRow(new QLabel("Email:"), emailEdit);
    formLayout->addRow(new QLabel("Username:"), usernameEdit);
    formLayout->addRow(new QLabel("Password:"), passwordEdit);

    // Set label font for the form layout (making them bold and black)
    for (int i = 0; i < formLayout->rowCount(); ++i) {
        QLayoutItem *labelItem = formLayout->itemAt(i, QFormLayout::LabelRole);
        if (labelItem) {
            QLabel *label = qobject_cast<QLabel *>(labelItem->widget());
            if (label) {
                label->setFont(labelFont);
                label->setPalette(labelPalette);  // Apply black color for the text

            }
        }
    }

    // Add the form layout to the main layout
    mainLayout->addLayout(formLayout);

    // Add some space between the form and the submit button
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Add submit button
    QPushButton *submitButton = new QPushButton("Submit");
    submitButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 5px; padding: 10px; font-size: 14px; }"
                                "QPushButton:hover { background-color: #45a049; }");

    // Add submit button to layout
    mainLayout->addWidget(submitButton);

    // Add Login button at the bottom
    QLabel *alreadyRegisteredLabel = new QLabel("                  You are already registered? ");
    alreadyRegisteredLabel->setStyleSheet("QLabel { color: BLACK; font-size: 14px; padding-right: 5px; }");

    QPushButton *loginButton = new QPushButton("Login");

    // Style the login button
    loginButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border-radius: 5px; padding: 10px; font-size: 14px; }"
                               "QPushButton:hover { background-color: #1976D2; }");

    // Layout for the login and registration message
    QHBoxLayout *loginLayout = new QHBoxLayout;
    loginLayout->addWidget(alreadyRegisteredLabel);
    loginLayout->addWidget(loginButton);

    // Add the login layout to the main layout
    mainLayout->addLayout(loginLayout);

    // Connect the submit button to validate and show message
    connect(submitButton, &QPushButton::clicked, this, [this, nameEdit, fatherNameEdit, phoneNumberEdit, emailEdit, usernameEdit, passwordEdit]() {
        // Ensure no field is empty
        if (nameEdit->text().isEmpty() || fatherNameEdit->text().isEmpty() ||
            phoneNumberEdit->text().isEmpty() || emailEdit->text().isEmpty() ||
            usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Form Incomplete", "Please fill out all fields.");
            return;
        }

        // Validate phone number (11 digits)
        QRegularExpression phoneRegex("^\\d{11}$");
        if (!phoneRegex.match(phoneNumberEdit->text()).hasMatch()) {
            QMessageBox::warning(this, "Invalid Phone Number", "Phone number must be exactly 11 digits.");
            return;
        }

        // Validate email
        QString email = emailEdit->text();
        if (!email.endsWith("@gmail.com")) {
            QMessageBox::warning(this, "Invalid Email", "Email must end with @gmail.com.");
            return;
        }

        // Validate password (at least 8 characters and contains both letters and digits)
        QRegularExpression passwordRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d]{8,}$");
        if (!passwordRegex.match(passwordEdit->text()).hasMatch()) {
            QMessageBox::warning(this, "Invalid Password", "Password must be at least 8 characters long and contain both letters and digits.");
            return;
        }

        QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        QFile file(csvFilePath);
        bool fileExists = file.exists();

        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            if (!fileExists) {
                stream << "Name,Father's Name,Phone Number,Email,Username,Password\n";
            }
            stream << nameEdit->text() << ","
                   << fatherNameEdit->text() << ","
                   << phoneNumberEdit->text() << ","
                   << emailEdit->text() << ","
                   << usernameEdit->text() << ","
                   << passwordEdit->text() << "\n";
            file.close();
            QMessageBox::information(this, "Registration Successful", "You have registered successfully!");

            // After successful registration, switch to the login page
            showLoginPage();
        } else {
            QMessageBox::critical(this, "Error", "Unable to save registration details.");
        }
    });

    // Handle the login button click (go directly to the login page)
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::showLoginPage);

    // Set the layout for the registration widget
    registrationWidget->setLayout(mainLayout);
    setCentralWidget(registrationWidget);  // Set the registration form as the central widget
}

void MainWindow::showLoginPage()
{
    // Create the login form layout
    QWidget *loginWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(loginWidget);

    // Title Label (Login)
    QLabel *titleLabel = new QLabel("Login Credentials", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 30, QFont::Bold));  // Large title font
    titleLabel->setStyleSheet("QLabel { color: #333333; padding-bottom: 20px; }");  // Style the title
    mainLayout->addWidget(titleLabel);


    // Add some space before the form
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Create a form layout for the login inputs
    QFormLayout *formLayout = new QFormLayout();

    // Set the font for the form labels
    QFont labelFont("Arial", 12);
    QPalette labelPalette;
    labelPalette.setColor(QPalette::WindowText, Qt::black);  // Set text color to black for visibility
    labelFont.setBold(true);  // Set bold font for labels

    // Create input fields for login
    QLineEdit *usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Enter your username");  // Placeholder text for username
    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);  // Hide password input
    passwordEdit->setPlaceholderText("Enter your password");  // Placeholder text for password

    // Add form inputs with labels before each field
    formLayout->addRow(new QLabel("Username:"), usernameEdit);
    formLayout->addRow(new QLabel("Password:"), passwordEdit);

    // Set label font for the form layout (making them bold and black)
    for (int i = 0; i < formLayout->rowCount(); ++i) {
        QLayoutItem *labelItem = formLayout->itemAt(i, QFormLayout::LabelRole);
        if (labelItem) {
            QLabel *label = qobject_cast<QLabel *>(labelItem->widget());
            if (label) {
                label->setFont(labelFont);
                label->setPalette(labelPalette);  // Apply black color for the text
            }
        }
    }

    // Add the form layout to the main layout
    mainLayout->addLayout(formLayout);

    // Add some space between the form and the login button
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Add login button
    QPushButton *loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border-radius: 5px; padding: 10px; font-size: 14px; }"
                               "QPushButton:hover { background-color: #1976D2; }");

    // Add login button to layout
    mainLayout->addWidget(loginButton);

    // Handle the login button click
    connect(loginButton, &QPushButton::clicked, this, [this, usernameEdit, passwordEdit]() {
        // Check if username and password fields are not empty
        if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Login Failed", "Please enter both username and password.");
            return;
        }

        QFile file(csvFilePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Unable to access user data.");
            return;
        }

        QTextStream stream(&file);
        QString line;
        bool loginSuccessful = false;

        // Skip the header row
        stream.readLine();

        while (stream.readLineInto(&line)) {
            QStringList fields = line.split(',');
            if (fields.size() >= 6) {
                QString storedUsername = fields[4].trimmed();
                QString storedPassword = fields[5].trimmed();

                if (usernameEdit->text() == storedUsername && passwordEdit->text() == storedPassword) {
                    loginSuccessful = true;
                    break;
                }
            }
        }
        file.close();

        if (loginSuccessful) {
            QMessageBox::information(this, "Login Successful", "You have logged in successfully!");

            // Transition to the Plagiarism Checker (Add Documents page)
            showPlagiarismCheckerPage();
        } else {
            QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
        }
    });

    // Set the layout for the login widget
    loginWidget->setLayout(mainLayout);
    setCentralWidget(loginWidget);  // Set the login form as the central widget
}

void MainWindow::showPlagiarismCheckerPage() {
    // Create main widget and layout
    QWidget *plagiarismWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(plagiarismWidget);

    // Create results table
    QTableWidget *resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(3);
    resultsTable->setHorizontalHeaderLabels({"File Name", "Plagiarism %", "Verdict"});
    resultsTable->setEditTriggers(QTableWidget::NoEditTriggers);
    resultsTable->setSelectionBehavior(QTableWidget::SelectRows);
    resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Store the table widget for later use
    ui->tableResults = resultsTable;

    // Title and other existing UI elements (keep existing code)
    QLabel *titleLabel = new QLabel("Plagiarism Checker and Calculator", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setStyleSheet("QLabel { color: #333333; padding-bottom: 20px; }");
    mainLayout->addWidget(titleLabel);

    // Add existing buttons and labels (keep existing code)
    // ... (keep the existing button creation code)

    // Add the results table to the layout
    mainLayout->addWidget(resultsTable);

    // Add some space before the buttons
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Button 1: Add Documents
    QPushButton *addDocumentsButton = new QPushButton("Add Documents");
    addDocumentsButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 5px; padding: 10px; font-size: 14px; }"
                                      "QPushButton:hover { background-color: #45a049; }");
    QLabel *addDocumentsLabel = new QLabel("This button allows you to add documents whose wording needs to be compared with the target document.", this);
    addDocumentsLabel->setAlignment(Qt::AlignCenter);
    addDocumentsLabel->setFont(QFont("Arial", 12));
    addDocumentsLabel->setStyleSheet("QLabel { color: #555555; padding-bottom: 20px; }");

    // Button 2: Target Document
    QPushButton *targetDocumentButton = new QPushButton("Target Document");
    targetDocumentButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border-radius: 5px; padding: 10px; font-size: 14px; }"
                                        "QPushButton:hover { background-color: #1976D2; }");
    QLabel *targetDocumentLabel = new QLabel("This button allows you to select the target document whose plagiarism will be calculated.", this);
    targetDocumentLabel->setAlignment(Qt::AlignCenter);
    targetDocumentLabel->setFont(QFont("Arial", 12));
    targetDocumentLabel->setStyleSheet("QLabel { color: #555555; padding-bottom: 20px; }");

    // Button 3: Check Plagiarism Calculator
    QPushButton *checkPlagiarismButton = new QPushButton("Check Plagiarism Calculator");
    checkPlagiarismButton->setStyleSheet("QPushButton { background-color: #FF9800; color: white; border-radius: 5px; padding: 10px; font-size: 14px; }"
                                         "QPushButton:hover { background-color: #FB8C00; }");
    QLabel *checkPlagiarismLabel = new QLabel("Click to check the plagiarism percentage between the added documents and the target document.", this);
    checkPlagiarismLabel->setAlignment(Qt::AlignCenter);
    checkPlagiarismLabel->setFont(QFont("Arial", 12));
    checkPlagiarismLabel->setStyleSheet("QLabel { color: #555555; padding-bottom: 20px; }");

    // Add buttons and labels to the layout
    mainLayout->addWidget(addDocumentsButton);
    mainLayout->addWidget(addDocumentsLabel);

    mainLayout->addWidget(targetDocumentButton);
    mainLayout->addWidget(targetDocumentLabel);

    mainLayout->addWidget(checkPlagiarismButton);
    mainLayout->addWidget(checkPlagiarismLabel);

    // Connect the buttons to their respective slots (you will define these slots)
    connect(addDocumentsButton, &QPushButton::clicked, this, &MainWindow::onAddDocumentsClicked);
    connect(targetDocumentButton, &QPushButton::clicked, this, &MainWindow::onTargetDocumentClicked);
    connect(checkPlagiarismButton, &QPushButton::clicked, this, &MainWindow::onCheckPlagiarismClicked);

    // Set the layout for the plagiarism widget
    plagiarismWidget->setLayout(mainLayout);
    setCentralWidget(plagiarismWidget);  // Set the plagiarism checker page as the central widget
}
QString targetDocument;
QMap<QString, QString> comparisonDocuments; // filename -> content
PlagiarismChecker checker;

// Replace these functions in mainwindow.cpp
void MainWindow::onAddDocumentsClicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        "Select Documents",
        QDir::homePath(),
        "All Supported Files (.txt *.pdf *.doc *.docx);;Text Files (.txt);;PDF Files (.pdf);;Word Documents (.doc .docx);;All Files ()"
        );

    if (fileNames.isEmpty()) {
        QMessageBox::information(this, "No Files Selected", "No files were selected. Please try again.");
        return;
    }

    // Clear existing documents if any
    comparisonDocuments.clear();

    // Process each selected file
    for (const QString &fileName : fileNames) {
        QString fileContent = checker.extractTextFromFile(fileName);
        if (!fileContent.isEmpty()) {
            comparisonDocuments.insert(QFileInfo(fileName).fileName(), fileContent);
        }
    }

    QMessageBox::information(this, "Documents Added",
                             QString("%1 documents have been loaded for comparison.").arg(comparisonDocuments.size()));
}

void MainWindow::onTargetDocumentClicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Target Document",
        QDir::homePath(),
        "All Supported Files (.txt *.pdf *.doc *.docx);;Text Files (.txt);;PDF Files (.pdf);;Word Documents (.doc .docx);;All Files ()"
        );

    if (fileName.isEmpty()) {
        QMessageBox::information(this, "No File Selected", "No file was selected. Please try again.");
        return;
    }

    QString fileContent = checker.extractTextFromFile(fileName);
    if (fileContent.isEmpty()) {
        QMessageBox::warning(this, "Empty File", "The selected file is empty or could not be read.");
        return;
    }

    targetDocument = fileContent;
    QMessageBox::information(this, "File Selected", "The target document has been loaded successfully.");
}

void MainWindow::onCheckPlagiarismClicked() {
    if (targetDocument.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a target document first.");
        return;
    }

    if (comparisonDocuments.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please add comparison documents first.");
        return;
    }

    QMap<QString, double> results;
    try {
        // Compare target document with each comparison document
        for (auto it = comparisonDocuments.begin(); it != comparisonDocuments.end(); ++it) {
            // Create a QStringList with a single QString (the comparison file)
            QStringList comparisonFileList;
            comparisonFileList << it.value();

            // Get the raw plagiarism results
            QMap<QString, PlagiarismResult> rawResults = checker.compareWithTarget(targetDocument, comparisonFileList);

            // Check if there are results for the current comparison file
            if (!rawResults.isEmpty()) {
                // Extract the similarity percentage from the PlagiarismResult
                double similarity = rawResults.first().percentage; // Assuming 'percentage' holds the similarity score
                results.insert(it.key(), similarity);
            } else {
                // If no plagiarism is detected, set similarity to 0 or handle as needed
                results.insert(it.key(), 0.0);
            }
        }
    } catch (const std::exception &e) {
        // Handle known exceptions
        QMessageBox::critical(this, "Error", QString("An error occurred during plagiarism checking: %1").arg(e.what()));
        return;
    } catch (...) {
        // Handle unknown exceptions
        QMessageBox::critical(this, "Error", "An unknown error occurred during plagiarism checking.");
        return;
    }

    if (results.isEmpty()) {
        QMessageBox::warning(this, "Results", "No plagiarism detected or no results to display.");
        return;
    }

    // Update the results table
    updateTable(results);
    QMessageBox::information(this, "Plagiarism Check Completed", "Plagiarism check is complete. Results have been updated.");
}
void MainWindow::updateTable(const QMap<QString, double> &results) {
    if (!ui->tableResults) {
        return;
    }

    ui->tableResults->clearContents();
    ui->tableResults->setRowCount(results.size());

    int row = 0;
    for (auto it = results.begin(); it != results.end(); ++it) {
        const QString &filePath = it.key();
        const double plagiarismPercentage = it.value();

        QFileInfo fileInfo(filePath);

        // Set file name
        QTableWidgetItem *fileItem = new QTableWidgetItem(fileInfo.fileName());
        ui->tableResults->setItem(row, 0, fileItem);

        // Set plagiarism percentage
        QString percentageText = QString::number(plagiarismPercentage, 'f', 2) + "%";
        QTableWidgetItem *percentageItem = new QTableWidgetItem(percentageText);
        ui->tableResults->setItem(row, 1, percentageItem);

        // Determine verdict based on percentage
        QString verdict;
        QColor color;
        if (std::isnan(plagiarismPercentage) || plagiarismPercentage < 1.0) {
            verdict = "No Plagiarism";
            color = QColor(0, 255, 0, 127);  // Green
        } else if (plagiarismPercentage < 25.0) {
            verdict = "Low Similarity";
            color = QColor(144, 238, 144, 127);  // Light green
        } else if (plagiarismPercentage < 50.0) {
            verdict = "Moderate Similarity";
            color = QColor(255, 255, 0, 127);  // Yellow
        } else if (plagiarismPercentage < 75.0) {
            verdict = "High Similarity";
            color = QColor(255, 165, 0, 127);  // Orange
        } else {
            verdict = "Very High Similarity";
            color = QColor(255, 0, 0, 127);  // Red
        }

        // Set verdict
        QTableWidgetItem *verdictItem = new QTableWidgetItem(verdict);
        ui->tableResults->setItem(row, 2, verdictItem);

        // Set matched words (calculated based on the similarity percentage)
        int estimatedMatchedWords = static_cast<int>((plagiarismPercentage / 100.0) *
                                                     getWordCount(comparisonDocuments[filePath]));
        QTableWidgetItem *matchedWordsItem = new QTableWidgetItem(
            QString::number(estimatedMatchedWords) + " words");
        ui->tableResults->setItem(row, 3, matchedWordsItem);

        // Apply colors and alignment
        for (int col = 0; col < ui->tableResults->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableResults->item(row, col);
            if (item) {
                item->setBackground(color);
                item->setTextAlignment(Qt::AlignCenter);
            }
        }

        ++row;
    }

    ui->tableResults->resizeColumnsToContents();
}

// Helper function to count words in a text
int MainWindow::getWordCount(const QString& text) {
    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    return words.size();
}
MainWindow::~MainWindow()
{
    delete ui;
}
