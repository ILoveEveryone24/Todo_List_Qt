#include <QApplication>
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QStyle>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cstdlib>

void updateTodoCount(QLabel *label, QListWidget* taskList){
	int cnt = 0;
	for(int i = 0; i < taskList->count(); i++){
		QListWidgetItem *item = taskList->item(i);
		if(item->checkState() != Qt::Checked){
			cnt++;
		}
	}
	label->setText(QString("Todo %1").arg(cnt));
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	QString jsonFilePath = QCoreApplication::applicationDirPath() + "/todo.json";

	std::ifstream file(jsonFilePath.toStdString());

	if(argc == 2){
		if(std::string(argv[1]) == "--edit"){
			std::string command = "vim ";
			command += jsonFilePath.toStdString();

			int result = system(command.c_str());
			if(result == 0){
			std::cout << "File: " << jsonFilePath.toStdString() << " opened in VIM" << std::endl;
			return 0;
			}
			else{
			std::cerr << "Failed to open file in VIM." << std::endl;
			}
		}
		else{
			std::cerr << "Invalid flag: " << argv[1] << std::endl;
			return -1;
		}
	}
	else if(argc > 2){
		return -1;
	}

	if(!file){
		std::cerr << "Could not open the file." << std::endl;
		return -1;
	}

	nlohmann::json data;
	file >> data;

	if(data.contains("tasks")){
		for(const nlohmann::json &task : data["tasks"]){
			std::cout << "task: " << task["task"] << std::endl;
			std::cout << "completed: " << task["completed"] << std::endl;
		}
	}

	app.setStyle("Windows");

	QWidget window;
	window.setWindowTitle("To-Do App");
	window.setWindowFlag(Qt::WindowStaysOnTopHint);
	window.setFixedSize(400, 300);

	window.setStyleSheet(
		"QWidget {"
		"	font-family: Comic Sans MS;"
		"	font-size: 20px;"
		"	background-color: #4c80d4;"
		"}"
		"QListWidget {"
		"	background-color: #ffffff;"
		"	border: 3px solid #000000;"
		"	outline:none;"
		"}"
		"QListWidget::item:select {"
		"	background-color:#ffffff;"
		"}"
	);

	//Widgets
	QVBoxLayout *mainLayout = new QVBoxLayout(&window);
	QPushButton *switchButton = new QPushButton("Minimize", &window);

	mainLayout->addWidget(switchButton);

	QStackedWidget *layoutList = new QStackedWidget;

	//Layout 1
	QWidget *maximizedLayoutWidget = new QWidget;
	QVBoxLayout *maximizedLayout = new QVBoxLayout(maximizedLayoutWidget);

	QListWidget *taskList = new QListWidget(&window);
	taskList->setSelectionMode(QAbstractItemView::NoSelection);	
	taskList->setStyleSheet(
		"QListWidget::item {"
		"	height: 50px;"
		"}"
	);

	maximizedLayout->addWidget(taskList);

	if(data.contains("tasks")){
		for(const nlohmann::json &task : data["tasks"]){
			QString taskString = QString::fromStdString(task["task"]);
			QListWidgetItem *item = new QListWidgetItem(taskString);
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
			item->setCheckState(task["completed"].get<bool>() ? Qt::Checked : Qt::Unchecked);

			taskList->addItem(item);
		}
	}

	layoutList->addWidget(maximizedLayoutWidget);

	//Layout 2
	QWidget *minimizedLayoutWidget = new QWidget;
	QVBoxLayout *minimizedLayout = new QVBoxLayout(minimizedLayoutWidget);

	QLabel *todoCount = new QLabel();
	todoCount->setAlignment(Qt::AlignCenter);

	updateTodoCount(todoCount, taskList);
	minimizedLayout->addWidget(todoCount);

	layoutList->addWidget(minimizedLayoutWidget);

	QObject::connect(taskList, &QListWidget::itemChanged, [todoCount, taskList](){
		updateTodoCount(todoCount, taskList);
	});

	mainLayout->addWidget(layoutList);

	//Button function
	QObject::connect(switchButton, &QPushButton::clicked, [switchButton, layoutList, &window]() {
		int currentIndex = layoutList->currentIndex();
		int nextIndex = (currentIndex + 1) % 2;
		layoutList->setCurrentIndex(nextIndex);

		if(nextIndex == 0){
			switchButton->setText("Minimize");
			window.setFixedSize(400, 300);
		}
		else{
			switchButton->setText("Maximize");
			window.setFixedSize(150, 100);
		}

	});

	window.setLayout(mainLayout);

	window.show();

	return app.exec();
}
