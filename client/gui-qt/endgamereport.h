/*####################################################################
###      ***                                     ***               ###
###     *****          ⒻⓇⒺⒺⒸⒾⓋ ②①         *****              ###
###      ***                                     ***               ###
#####################################################################*/

#ifndef FC__ENDGAMEREPORT_H
#define FC__ENDGAMEREPORT_H

#include "repodlgs_g.h"

#include <QObject>  // for Q_OBJECT
#include <QString>      // for QString
#include <QWidget>      // for QWidget
class QObject;
class QTableWidget;  // lines 12-12

/****************************************************************************
  Tab widget to display economy report (F5)
****************************************************************************/
class endgame_report : public QWidget {
  Q_OBJECT
  QTableWidget *end_widget;

public:
  endgame_report(const struct packet_endgame_report *packet);
  ~endgame_report();
  void update_report(const struct packet_endgame_player *packet);
  void init();

private:
  int index;
  int players;
};

void popdown_endgame_report();
void popup_endgame_report();

#endif /* FC__ENDGAMEREPORT_H */
