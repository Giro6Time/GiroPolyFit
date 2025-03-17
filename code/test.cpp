#include "./test.h"
#include "logger.h"
#include "progress.h"
#include "point_set_io.h"
#include <memory>

namespace MyUi
{
    // CoutLogger测试
    // TODO: 将某个UI界面重载为LoggerClient，并且在界面内的输出框输出Log
    class Form : public Ui::Form, public CoutLogger, public ProgressClient, public std::enable_shared_from_this<Form>
    {
    public:
        Form(QWidget *parent)
        {

            parent->setWindowTitle("Qt5 Test Window"); // 设置窗口标题
            parent->resize(400, 300);                  // 设置窗口大小

            QLabel *label = new QLabel(parent); // 创建一个QLabel标签
            label->setText("Hello, Qt5!");      // 设置标签文本
            label->move(100, 100);              // 移动标签到窗口中的位置

            parent->show();
            Logger::initialize();
            out_message("test\n");
            Logger::out("Feature1") << "Logger::out() test" << std::endl;
            Logger::out("Feature2") << "Logger::out() test" << std::endl;
        }
        virtual void setupUi(QWidget *Form) override
        {
            Ui::Form::setupUi(Form);
            Progress::instance()->register_client(shared_from_this());
            PointSetIO::read("D:/Study/Polyfit/GiroPolyFit/test/pointset.vg");
        }

        virtual void notify(std::size_t new_val) override { Logger::out("Progress") << new_val << std::endl; }
    };
}
