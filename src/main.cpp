#include <QApplication>
#include <QPushButton>

#include <driver/commands/facade/facade.h>
#include <driver/commands/conf_commands.h>


int main(int argc, char *argv[]) {
    using namespace driver::commands;

    int res = 0;
    LoadConfCmd cmd({}, [=](LoadConfCmdResult) mutable {
        QApplication a(argc, argv);
        QPushButton button("Hello world!", nullptr);
        button.resize(200, 100);
        button.show();
        res = QApplication::exec();
    });
    Facade::execute(cmd);

    return res;
}
