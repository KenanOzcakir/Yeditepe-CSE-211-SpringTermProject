#include <QtWidgets/QApplication>

#include "SpellCheckerWidget.hpp"

/**
 * @brief Starts the Qt spell checker frontend.
 *
 * @param argc Command-line argument count.
 * @param argv Command-line argument values.
 * @return Qt application exit code.
 * @pre Qt6 Widgets must be available at build and runtime.
 * @post The GUI event loop runs until the user closes the window.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    SpellCheckerWidget widget;
    widget.setWindowTitle("Spell Checker");
    widget.resize(640, 180);
    widget.show();

    return app.exec();
}
