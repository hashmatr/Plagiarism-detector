#ifndef PLAGIARISMCHECKER_H
#define PLAGIARISMCHECKER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include <QSet>

// AVL Tree Node Template
template <typename K, typename V>
struct AVLNode {
    K key;
    V value;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const K& k, const V& v)
        : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree Template
template <typename K, typename V>
class AVLTree {
private:
    AVLNode<K, V>* root;

    int getHeight(AVLNode<K, V>* node);
    int getBalance(AVLNode<K, V>* node);
    AVLNode<K, V>* rightRotate(AVLNode<K, V>* y);
    AVLNode<K, V>* leftRotate(AVLNode<K, V>* x);
    AVLNode<K, V>* insert(AVLNode<K, V>* node, const K& key, const V& value);
    AVLNode<K, V>* findMin(AVLNode<K, V>* node);
    AVLNode<K, V>* deleteNode(AVLNode<K, V>* root, const K& key);
    void clear(AVLNode<K, V>* node);

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear(); }

    void insert(const K& key, const V& value);
    void remove(const K& key);
    V* find(const K& key);
    void clear();
};

struct PlagiarismResult {
    double percentage;
    QStringList matchedPhrases;
    QString matchedText;
};

class PlagiarismChecker {
public:
    PlagiarismChecker();

    // Main functions
    QString extractTextFromFile(const QString& filePath);
    QMap<QString, PlagiarismResult> compareWithTarget(
        const QString& targetContent,
        const QStringList& comparisonContents
        );

private:
    // Text processing
    QString getFileContent(const QString& filePath);
    void cleanString(QString& str);
    QStringList tokenizeText(const QString& text);
    void removeStopwords(QStringList& tokens);

    // Analysis methods
    QMap<QString, int> getFrequency(const QStringList& tokens);
    float getMultiplier(const QString& word);
    float dotProduct(const QVector<int>& a, const QVector<int>& b);
    float cosineScore(const QVector<int>& bvector, const QVector<int>& tvector);

    // TF-IDF methods
    float calculateTF(const QString& term, const QStringList& document);
    float calculateIDF(const QString& term, const QVector<QStringList>& documents);
    QVector<float> computeTFIDFVector(
        const QStringList& document,
        const QSet<QString>& vocabulary,
        const QVector<QStringList>& allDocuments
        );
    float tfidfCosineSimilarity(
        const QStringList& doc1,
        const QStringList& doc2,
        const QVector<QStringList>& corpus
        );
    float tfidfTest(const QStringList& baseTokens, const QStringList& targetTokens);

    // Different test methods
    float tokenizeTest(const QStringList& baseTokens, const QStringList& targetTokens);
    float ngramTest(const QStringList& baseTokens, const QStringList& targetTokens);
    float ngramScore(const QStringList& base, const QStringList& target, int n);
    float cosineTest(const QStringList& baseTokens, const QStringList& targetTokens);
    float kmpTest(const QString& text, const QString& pattern);
    float bmTest(const QString& text, const QString& pattern);

    // Pattern matching helpers
    QVector<int> computeKmpLps(const QString& pattern);
    QMap<QChar, int> preprocessBM(const QString& pattern);

    // Results processing
    float getVerdict(const QVector<float>& scores, const QStringList& matchedFiles);
};

#endif // PLAGIARISMCHECKER_H
