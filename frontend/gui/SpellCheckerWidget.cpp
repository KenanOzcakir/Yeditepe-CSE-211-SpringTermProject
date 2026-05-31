#include "SpellCheckerWidget.hpp"

SpellCheckerWidget::SpellCheckerWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);

    auto *title = new QLabel("Type text below (last word will be checked):", this);
    mainLayout->addWidget(title);

    m_input = new QLineEdit(this);
    mainLayout->addWidget(m_input);

    m_statusLabel = new QLabel("Status: -", this);
    mainLayout->addWidget(m_statusLabel);

    auto *btnRow = new QHBoxLayout();
    m_suggestion1 = createSuggestionButton();
    m_suggestion2 = createSuggestionButton();
    m_suggestion3 = createSuggestionButton();
    btnRow->addWidget(m_suggestion1);
    btnRow->addWidget(m_suggestion2);
    btnRow->addWidget(m_suggestion3);
    mainLayout->addLayout(btnRow);

    connect(m_input, &QLineEdit::textChanged, this, &SpellCheckerWidget::updateSuggestions);

    connect(m_suggestion1, &QPushButton::clicked, this, [this]() { onSuggestionClicked(m_suggestion1); });
    connect(m_suggestion2, &QPushButton::clicked, this, [this]() { onSuggestionClicked(m_suggestion2); });
    connect(m_suggestion3, &QPushButton::clicked, this, [this]() { onSuggestionClicked(m_suggestion3); });

    updateSuggestions(m_input->text());
}

void SpellCheckerWidget::updateSuggestions(const QString &text) {
    if (!text.isEmpty() && text.back().isSpace()) {
        m_statusLabel->setText("Status: -");
        setSuggestionButtons(nullptr, 0);
        return;
    }

    const QString last = extractLastWord(text);

    if (last.isEmpty()) {
        m_statusLabel->setText("Status: -");
        setSuggestionButtons(nullptr, 0);
        return;
    }

    std::string word = last.toStdString();
    std::string results[3];

    if (isWordCorrect(word)) {
        int count = getCompletions(word, results, 3);
        if (count > 0) {
            m_statusLabel->setText("Status: correct; autocomplete");
        } else {
            m_statusLabel->setText("Status: correct");
        }
        setSuggestionButtons(results, count);
        return;
    }

    int count = getSuggestions(word, results, 3);

    if (count == 0) {
        count = getCompletions(word, results, 3);
        if (count == 0) {
            m_statusLabel->setText("Status: incorrect (no suggestions)");
        } else {
            m_statusLabel->setText("Status: prefix autocomplete");
        }
    } else {
        m_statusLabel->setText("Status: suggestions for '" + last + "'");
    }

    setSuggestionButtons(results, count);
}

QPushButton* SpellCheckerWidget::createSuggestionButton() {
    auto *btn = new QPushButton("-", this);
    btn->setEnabled(false);
    return btn;
}

void SpellCheckerWidget::setSuggestionButtons(const std::string suggestions[], int count) {
    QPushButton* buttons[3] = {m_suggestion1, m_suggestion2, m_suggestion3};

    for (int i = 0; i < 3; i++) {
        if (suggestions && i < count) {
            buttons[i]->setText(QString::fromStdString(suggestions[i]));
            buttons[i]->setEnabled(true);
        } else {
            buttons[i]->setText("-");
            buttons[i]->setEnabled(false);
        }
    }
}

void SpellCheckerWidget::onSuggestionClicked(QPushButton *btn) {
    if (!btn || !btn->isEnabled()) return;
    replaceLastWord(btn->text());
}

QString SpellCheckerWidget::extractLastWord(const QString &text) const {
    QString t = text;
    int end = t.length() - 1;
    while (end >= 0 && t[end].isSpace()) end--;
    if (end < 0) return "";

    int start = end;
    while (start >= 0 && !t[start].isSpace()) start--;
    start++;

    return t.mid(start, end - start + 1);
}

void SpellCheckerWidget::replaceLastWord(const QString &replacement) {
    QString currentText = m_input->text();

    int trailingSpaces = 0;
    for (int i = currentText.length() - 1; i >= 0; --i) {
        if (currentText[i].isSpace()) trailingSpaces++;
        else break;
    }

    QString trimmed = currentText;
    while (!trimmed.isEmpty() && trimmed.back().isSpace()) trimmed.chop(1);

    if (trimmed.isEmpty()) {
        QString result = replacement + QString(trailingSpaces, ' ');
        m_input->setText(result);
        return;
    }

    int end = trimmed.length() - 1;
    int start = end;
    while (start >= 0 && !trimmed[start].isSpace()) start--;
    start++;

    QString before = trimmed.left(start);
    QString result = before + replacement + QString(trailingSpaces, ' ');
    m_input->setText(result);
}
