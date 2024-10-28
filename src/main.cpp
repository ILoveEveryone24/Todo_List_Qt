#include <QApplication>
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QStyle>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

int main(int argc, char *argv[]) {
	std::ifstream file("../todo.json");
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

	QApplication app(argc, argv);
	app.setStyle("Windows");

	QWidget window;
	window.setWindowTitle("My Qt To-Do App");
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

	QVBoxLayout *layout = new QVBoxLayout(&window);

	QListWidget *taskList = new QListWidget(&window);
	taskList->setSelectionMode(QAbstractItemView::NoSelection);
	taskList->setStyleSheet(
		"QListWidget::item {"
		"	height: 50px;"
		"}"
	);

	layout->addWidget(taskList);

	if(data.contains("tasks")){
		for(const nlohmann::json &task : data["tasks"]){
			QString taskString = QString::fromStdString(task["task"]);
			QListWidgetItem *item = new QListWidgetItem(taskString);
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
			item->setCheckState(task["completed"].get<bool>() ? Qt::Checked : Qt::Unchecked);

			taskList->addItem(item);
		}
	}
	
	window.setLayout(layout);

	window.show();

	return app.exec();
}
