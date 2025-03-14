#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QStringList>
#include <QMessageBox>
#include <QTableWidget>
#include "PlagiarismChecker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation functions
    void showRegistrationPage();
    void showLoginPage();
    void showPlagiarismCheckerPage();

    // Plagiarism checker functions
    void onTargetDocumentClicked();
    void onAddDocumentsClicked();
    void onCheckPlagiarismClicked();
    void updateTable(const QMap<QString, double> &results);

private:
    Ui::MainWindow *ui;
    QString targetFilePath;
    QStringList documentPaths;
    PlagiarismChecker plagiarismChecker;
    int getWordCount(const QString& text); // Add this line


    // Helper functions
    void setupResultsTable();
    void clearResults();
    QString formatFileSize(qint64 size);
    void showError(const QString &message);
    bool validateFiles();
};

#endif // MAINWINDOW_H
