#ifndef SPELLCHECKERWIDGET_HPP
#define SPELLCHECKERWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtCore/QString>
#include "spellchecker.h"

class SpellCheckerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SpellCheckerWidget(QWidget *parent = nullptr);

private slots:
    void updateSuggestions(const QString &text);

private:
    QLineEdit   *m_input = nullptr;
    QPushButton *m_suggestion1 = nullptr;
    QPushButton *m_suggestion2 = nullptr;
    QPushButton *m_suggestion3 = nullptr;
    QLabel      *m_statusLabel = nullptr;

    QPushButton* createSuggestionButton();
    void setSuggestionButtons(const std::string suggestions[], int count);
    void onSuggestionClicked(QPushButton *btn);
    QString extractLastWord(const QString &text) const;
    void replaceLastWord(const QString &replacement);
};

#endif
