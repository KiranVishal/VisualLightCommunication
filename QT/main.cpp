#include <QApplication>
#include <QWidget>
#include <QPalette>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "update.h"


QPushButton* btn_track1;
QPushButton* btn_track2;
QPushButton* btn_track3;
QPushButton* btn_stop;
QLineEdit* le_state;
QLabel* lb_state;
QLabel* lb_track;

int main(int argc, char **argv){
	QApplication app(argc, argv);
	QWidget w;
	QPalette p = w.palette();
	p.setColor(QPalette::Window, Qt::blue);
	w.setPalette(p);
	w.setAutoFillBackground(true);

	/*
	le_value = new QLineEdit(&w);
	le_value->move(80, 100);
	le_value->resize(100,50);
	*/
	btn_track1 = new QPushButton("Track1", &w);
	btn_track1->move(80, 100);
	btn_track1->resize(100,50);

	/*
	le_speed = new QLineEdit(&w);
	le_speed->move(185, 100);
	le_speed->resize(100,50);
	*/
	btn_track2 = new QPushButton("Track2", &w);
	btn_track2->move(185, 100);
	btn_track2->resize(100,50);

	/*
	le_direction = new QLineEdit(&w);
	le_direction->move(290, 100);
	le_direction->resize(100,50);
	*/
	btn_track3 = new QPushButton("Track3", &w);
	btn_track3->move(290, 100);
	btn_track3->resize(100,50);

	btn_stop = new QPushButton("STOP", &w);
	btn_stop->move(80, 50);
	btn_stop->resize(315,40);

	lb_track = new QLabel(&w);
	lb_track->move(80, 20);
	lb_track->resize(315,25);
	lb_track->setText("Audio over VLC");

	QPalette p_track = lb_track->palette();
	p_track.setColor(QPalette::WindowText, Qt::red);
	lb_track->setPalette(p_track);


	le_state = new QLineEdit(&w);
	le_state->move(80, 210);
	le_state->resize(315,50);

	lb_state = new QLabel(&w);
	lb_state->move(80, 180);
	lb_state->resize(315,25);
	lb_state->setText("STATE OF THE MUSIC RUNNING/STOPPED");

	QPalette p_state = lb_state->palette();
	p_state.setColor(QPalette::WindowText, Qt::red);
	lb_state->setPalette(p_state);


	UpdateInfo* update_info = new UpdateInfo(btn_track1, btn_track2, btn_track3, le_state, btn_stop, lb_track, &w);
	update_info->startTimer();

	w.showFullScreen();

	return app.exec();
}

