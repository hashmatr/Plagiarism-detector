#include "PlagiarismChecker.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QFileInfo>
#include <cmath>
#include <algorithm>

// AVL Tree Implementation
template <typename K, typename V>
int AVLTree<K, V>::getHeight(AVLNode<K, V>* node) {
    return node ? node->height : 0;
}

template <typename K, typename V>
int AVLTree<K, V>::getBalance(AVLNode<K, V>* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::rightRotate(AVLNode<K, V>* y) {
    AVLNode<K, V>* x = y->left;
    AVLNode<K, V>* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::leftRotate(AVLNode<K, V>* x) {
    AVLNode<K, V>* y = x->right;
    AVLNode<K, V>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::insert(AVLNode<K, V>* node, const K& key, const V& value) {
    if (!node) return new AVLNode<K, V>(key, value);

    if (key < node->key)
        node->left = insert(node->left, key, value);
    else if (key > node->key)
        node->right = insert(node->right, key, value);
    else {
        node->value = value;
        return node;
    }

    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

template <typename K, typename V>
void AVLTree<K, V>::insert(const K& key, const V& value) {
    root = insert(root, key, value);
}

template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::findMin(AVLNode<K, V>* node) {
    return node && node->left ? findMin(node->left) : node;
}

template <typename K, typename V>
AVLNode<K, V>* AVLTree<K, V>::deleteNode(AVLNode<K, V>* root, const K& key) {
    if (!root) return nullptr;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left || !root->right) {
            AVLNode<K, V>* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            AVLNode<K, V>* temp = findMin(root->right);
            root->key = temp->key;
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (!root) return nullptr;

    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;

    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

template <typename K, typename V>
void AVLTree<K, V>::remove(const K& key) {
    root = deleteNode(root, key);
}

template <typename K, typename V>
V* AVLTree<K, V>::find(const K& key) {
    AVLNode<K, V>* current = root;
    while (current) {
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else
            return &(current->value);
    }
    return nullptr;
}

template <typename K, typename V>
void AVLTree<K, V>::clear(AVLNode<K, V>* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template <typename K, typename V>
void AVLTree<K, V>::clear() {
    clear(root);
    root = nullptr;
}

// PlagiarismChecker Implementation
PlagiarismChecker::PlagiarismChecker() {}

QString PlagiarismChecker::extractTextFromFile(const QString& filePath) {
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();

    if (extension == "txt") {
        return getFileContent(filePath);
    } else if (extension == "pdf") {
        // Placeholder for PDF extraction
        return getFileContent(filePath);
    } else if (extension == "doc" || extension == "docx") {
        // Placeholder for Word document extraction
        return getFileContent(filePath);
    }

    return QString();
}

QString PlagiarismChecker::getFileContent(const QString& filePath) {
    QFile file(filePath);
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        content = in.readAll();
        file.close();
    }

    return content;
}

void PlagiarismChecker::cleanString(QString& str) {
    str = str.remove(QRegularExpression("[^a-zA-Z0-9\\s]"));
    str = str.toLower();
    str = str.simplified();
}

QStringList PlagiarismChecker::tokenizeText(const QString& text) {
    return text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
}

void PlagiarismChecker::removeStopwords(QStringList& tokens) {
    QSet<QString> stopwords = {
        "the", "is", "at", "which", "on", "in", "and", "to", "of", "a", "an",
        "for", "be", "this", "that", "with", "by", "from", "or", "as", "but"
    };

    tokens.removeAll(QString());

    for (int i = tokens.size() - 1; i >= 0; --i) {
        if (stopwords.contains(tokens[i].toLower())) {
            tokens.removeAt(i);
        }
    }
}

// Continue with all previously implemented methods...
// [Previous implementation of tokenizeTest, ngramTest, cosineTest, etc.]

float PlagiarismChecker::calculateTF(const QString& term, const QStringList& document) {
    int termFreq = 0;
    for (const QString& word : document) {
        if (word == term) {
            termFreq++;
        }
    }
    return termFreq > 0 ? (1 + log10(termFreq)) : 0;
}

float PlagiarismChecker::calculateIDF(const QString& term, const QVector<QStringList>& documents) {
    int docCount = 0;
    for (const QStringList& doc : documents) {
        if (doc.contains(term)) {
            docCount++;
        }
    }
    return docCount > 0 ? log10(static_cast<float>(documents.size()) / docCount) : 0;
}

QVector<float> PlagiarismChecker::computeTFIDFVector(
    const QStringList& document,
    const QSet<QString>& vocabulary,
    const QVector<QStringList>& allDocuments
    ) {
    QVector<float> tfidfVector;

    for (const QString& term : vocabulary) {
        float tf = calculateTF(term, document);
        float idf = calculateIDF(term, allDocuments);
        tfidfVector.append(tf * idf);
    }

    return tfidfVector;
}

float PlagiarismChecker::tfidfCosineSimilarity(
    const QStringList& doc1,
    const QStringList& doc2,
    const QVector<QStringList>& corpus
    ) {
    // Create vocabulary from both documents
    QSet<QString> vocabulary;
    for (const QString& word : doc1) vocabulary.insert(word);
    for (const QString& word : doc2) vocabulary.insert(word);

    // Compute TF-IDF vectors
    QVector<float> vector1 = computeTFIDFVector(doc1, vocabulary, corpus);
    QVector<float> vector2 = computeTFIDFVector(doc2, vocabulary, corpus);

    // Calculate cosine similarity
    float dotProduct = 0.0;
    float norm1 = 0.0;
    float norm2 = 0.0;

    for (int i = 0; i < vector1.size(); ++i) {
        dotProduct += vector1[i] * vector2[i];
        norm1 += vector1[i] * vector1[i];
        norm2 += vector2[i] * vector2[i];
    }

    norm1 = sqrt(norm1);
    norm2 = sqrt(norm2);

    if (norm1 > 0 && norm2 > 0) {
        return dotProduct / (norm1 * norm2);
    }

    return 0.0;
}

QMap<QString, int> PlagiarismChecker::getFrequency(const QStringList& tokens) {
    QMap<QString, int> freqs;
    for (const QString& token : tokens) {
        freqs[token]++;
    }
    return freqs;
}

float PlagiarismChecker::getMultiplier(const QString& word) {
    return word.length() * word.length();
}

float PlagiarismChecker::dotProduct(const QVector<int>& a, const QVector<int>& b) {
    float sum = 0;
    for (int i = 0; i < a.size(); i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

float PlagiarismChecker::cosineScore(const QVector<int>& bvector, const QVector<int>& tvector) {
    float denominator = sqrt(dotProduct(bvector, bvector)) * sqrt(dotProduct(tvector, tvector));
    if (denominator == 0) return 0;
    return dotProduct(bvector, tvector) / denominator;
}

float PlagiarismChecker::tokenizeTest(const QStringList& baseTokens, const QStringList& targetTokens) {
    QStringList bTokens = baseTokens;
    QStringList tTokens = targetTokens;

    removeStopwords(bTokens);
    removeStopwords(tTokens);

    auto tFreqs = getFrequency(tTokens);
    auto bFreqs = getFrequency(bTokens);

    int shared = 0;
    int total = 0;

    for (auto it = tFreqs.constBegin(); it != tFreqs.constEnd(); ++it) {
        if (bFreqs.contains(it.key())) {
            shared += qMin(it.value(), bFreqs[it.key()]) * getMultiplier(it.key());
        }
        total += it.value() * getMultiplier(it.key());
    }

    return total > 0 ? (10.0f * shared / total) : 0;
}

float PlagiarismChecker::ngramTest(const QStringList& baseTokens, const QStringList& targetTokens) {
    QVector<int> tests = {3, 5, 7};  // n-gram sizes to test
    QVector<int> weights = {3, 5, 7}; // corresponding weights
    QVector<float> ngresults;

    for (int n : tests) {
        ngresults.append(ngramScore(baseTokens, targetTokens, n));
    }

    float weightSum = 0;
    float scoreSum = 0;

    for (int i = 0; i < weights.size(); i++) {
        scoreSum += ngresults[i] * weights[i];
        weightSum += weights[i];
    }

    return weightSum > 0 ? 10 * pow(scoreSum / weightSum, 0.4) : 0;
}

float PlagiarismChecker::ngramScore(const QStringList& base, const QStringList& target, int n) {
    if (base.size() < n || target.size() < n) return 0;

    QVector<QStringList> bngrams;
    QVector<QStringList> tngrams;

    // Generate n-grams for base text
    for (int i = 0; i <= base.size() - n; i++) {
        QStringList temp;
        for (int j = i; j < i + n; j++) {
            temp.append(base[j]);
        }
        bngrams.append(temp);
    }

    // Generate n-grams for target text
    for (int i = 0; i <= target.size() - n; i++) {
        QStringList temp;
        for (int j = i; j < i + n; j++) {
            temp.append(target[j]);
        }
        tngrams.append(temp);
    }

    int shared = 0;
    for (const QStringList& tngram : tngrams) {
        for (const QStringList& bngram : bngrams) {
            if (tngram == bngram) {
                shared++;
                break;
            }
        }
    }

    return tngrams.isEmpty() ? 0 : (1.0f * shared / tngrams.size());
}

float PlagiarismChecker::cosineTest(const QStringList& baseTokens, const QStringList& targetTokens) {
    QStringList bTokens = baseTokens;
    QStringList tTokens = targetTokens;

    removeStopwords(bTokens);
    removeStopwords(tTokens);

    QStringList allTokens = bTokens + tTokens;
    allTokens.removeDuplicates();

    auto tFreqs = getFrequency(tTokens);
    auto bFreqs = getFrequency(bTokens);

    QVector<int> bVector;
    QVector<int> tVector;

    for (const QString& token : allTokens) {
        bVector.append(bFreqs.value(token, 0));
        tVector.append(tFreqs.value(token, 0));
    }

    return 10.0f * cosineScore(bVector, tVector);
}

QVector<int> PlagiarismChecker::computeKmpLps(const QString& pattern) {
    int m = pattern.length();
    QVector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

float PlagiarismChecker::kmpTest(const QString& text, const QString& pattern) {
    if (text.isEmpty() || pattern.isEmpty()) return 0;

    QVector<int> lps = computeKmpLps(pattern);
    int i = 0, j = 0;
    int matches = 0;

    while (i < text.length()) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == pattern.length()) {
            matches++;
            j = lps[j - 1];
        } else if (i < text.length() && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return (pattern.length() > 0) ? (10.0f * matches / (text.length() / pattern.length())) : 0;
}

QMap<QString, PlagiarismResult> PlagiarismChecker::compareWithTarget(
    const QString& targetContent,
    const QStringList& comparisonContents
    ) {
    QMap<QString, PlagiarismResult> results;

    QString cleanedTarget = targetContent;
    cleanString(cleanedTarget);
    QStringList targetTokens = tokenizeText(cleanedTarget);

    for (const QString& compContent : comparisonContents) {
        QString cleanedComp = compContent;
        cleanString(cleanedComp);
        QStringList compTokens = tokenizeText(cleanedComp);

        float tokenizeScore = tokenizeTest(compTokens, targetTokens);
        float ngramScore = ngramTest(compTokens, targetTokens);
        float cosineScore = cosineTest(compTokens, targetTokens);
        float kmpScore = kmpTest(cleanedComp, cleanedTarget);
        float bmScore = bmTest(cleanedComp, cleanedTarget);
        float tfidfScore = tfidfTest(compTokens, targetTokens);

        QVector<float> scores = {tokenizeScore, ngramScore, cosineScore,
                                 kmpScore, bmScore, tfidfScore};
        QStringList matchedFiles;

        if (scores.last() > 5) {
            matchedFiles.append("matched");
        }

        float finalScore = getVerdict(scores, matchedFiles);

        PlagiarismResult result;
        result.percentage = finalScore * 10.0;
        result.matchedText = compContent;

        results.insert(QString::number(results.size() + 1), result);
    }

    return results;
}
float PlagiarismChecker::bmTest(const QString& text, const QString& pattern) {
    if (text.isEmpty() || pattern.isEmpty()) return 0;

    QMap<QChar, int> lastOccurrence = preprocessBM(pattern);
    int n = text.length();
    int m = pattern.length();
    int matches = 0;
    int i = m - 1;  // Text position

    while (i < n) {
        int k = 0;  // Number of matched characters
        // Try to match characters from right to left
        while (k < m && pattern[m - 1 - k] == text[i - k]) {
            k++;
        }

        if (k == m) {
            // Pattern found
            matches++;
            i++;
        } else {
            // Character mismatch
            QChar mismatchChar = text[i];
            int lastOcc = lastOccurrence.contains(mismatchChar) ?
                              lastOccurrence[mismatchChar] : -1;
            int shift = m - 1 - lastOcc;
            i += qMax(1, shift);
        }
    }

    return (pattern.length() > 0) ? (10.0f * matches / (text.length() / pattern.length())) : 0;
}

QMap<QChar, int> PlagiarismChecker::preprocessBM(const QString& pattern) {
    QMap<QChar, int> lastOccurrence;

    // Store the last occurrence index of each character in the pattern
    for (int i = 0; i < pattern.length(); i++) {
        lastOccurrence[pattern[i]] = i;
    }

    return lastOccurrence;
}
float PlagiarismChecker::getVerdict(const QVector<float>& scores, const QStringList& matchedFiles) {
    // Updated weights for different algorithms
    QVector<int> weights = {4, 5, 4, 3, 3, 5}; // Adjusted weights for better accuracy

    float weightSum = 0;
    float scoreSum = 0;

    // Calculate weighted average of scores
    for (int i = 0; i < qMin(weights.size(), scores.size()); i++) {
        // Handle NaN values
        if (!std::isnan(scores[i])) {
            scoreSum += scores[i] * weights[i];
            weightSum += weights[i];
        }
    }

    // Calculate final score
    float finalScore = weightSum > 0 ? (scoreSum / weightSum) : 0;

    // Ensure the score is between 0 and 10
    finalScore = qBound(0.0f, finalScore, 10.0f);

    // Log detailed information for debugging

    return finalScore;
}

float PlagiarismChecker::tfidfTest(const QStringList& baseTokens, const QStringList& targetTokens) {
    QStringList bTokens = baseTokens;
    QStringList tTokens = targetTokens;

    removeStopwords(bTokens);
    removeStopwords(tTokens);

    QVector<QStringList> corpus;
    corpus.append(bTokens);
    corpus.append(tTokens);

    float similarity = tfidfCosineSimilarity(bTokens, tTokens, corpus);

    return similarity * 10.0;
}
