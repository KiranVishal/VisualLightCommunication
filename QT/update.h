#include <QApplication>
#include <QWidget>
#include <QPalette>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QObject>
#include <QPushButton>
#include <pthread.h>
#include <include/sndfile.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>


class UpdateInfo: public QObject
{
	Q_OBJECT

	public:
		QTimer* update_timer;
		QPushButton* btn_1;
		QPushButton* btn_2;
		QPushButton* btn_3;
		QPushButton* btn_stop;
		QLabel* track_name;
		QLineEdit* le_state;
		QWidget* screen_widget;
		pthread_t play;
		int playing;

		int arrow_pos;

		UpdateInfo(QPushButton* btn1, QPushButton* btn2, QPushButton* btn3, QLineEdit* state, QPushButton* stop, QLabel* track, QWidget* s_w) 
		{
			btn_1 = btn1;
			btn_2 = btn2;
			btn_3 = btn3;
			btn_stop = stop;
			track_name = track;
			le_state = state;
			screen_widget = s_w;
			arrow_pos = 0;
			playing = 0;
		}

		void startTimer()
		{
			update_timer = new QTimer(this);
			QObject::connect(update_timer, SIGNAL(timeout()), this, SLOT(updateUI()));
			update_timer->start(250);
			QObject::connect(btn_1, SIGNAL(clicked()), this, SLOT(playTrack1()));
			QObject::connect(btn_2, SIGNAL(clicked()), this, SLOT(playTrack2()));
			QObject::connect(btn_3, SIGNAL(clicked()), this, SLOT(playTrack3()));
			QObject::connect(btn_stop, SIGNAL(clicked()), this, SLOT(stopPlayback()));
		}

	public slots:
		void updateUI();
		void playTrack1();
		void playTrack2();
		void playTrack3();
		void stopPlayback();

};
