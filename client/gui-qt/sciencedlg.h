/*####################################################################
###      ***                                     ***               ###
###     *****          ⒻⓇⒺⒺⒸⒾⓋ ②①         *****              ###
###      ***                                     ***               ###
#####################################################################*/

#ifndef FC__SCIDLGS_H
#define FC__SCIDLGS_H

#ifdef HAVE_CONFIG_H
#include <fc_config.h>
#endif

#include "repodlgs_g.h"

class progress_bar;

/****************************************************************************
  Helper item for research diagram, about drawn rectangles and what
  tech/unit/improvement they point to.
****************************************************************************/
class req_tooltip_help {
public:
  req_tooltip_help();
  QRect rect;
  Tech_type_id tech_id;
  struct unit_type *tunit;
  struct impr_type *timpr;
  struct government *tgov;
};

/****************************************************************************
  Custom widget representing research diagram in science_report
****************************************************************************/
class research_diagram : public QWidget {
  Q_OBJECT

public:
  research_diagram(QWidget *parent = 0);
  ~research_diagram();
  void update_reqtree();
  void reset();
  QSize size();
private slots:
  void show_tooltip();

private:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);
  void create_tooltip_help();
  struct canvas *pcanvas;
  struct reqtree *req;
  bool timer_active;
  int width;
  int height;
  QList<req_tooltip_help *> tt_help;
  QPoint tooltip_pos;
  QString tooltip_text;
  QRect tooltip_rect;
};

/****************************************************************************
  Helper item for comboboxes, holding string of tech and its id
****************************************************************************/
struct qlist_item {
  QString tech_str;
  Tech_type_id id;
};

/****************************************************************************
  Widget embedded as tab on game view (F6 default)
  Uses string "SCI" to mark it as opened
  You can check it using if (gui()->is_repo_dlg_open("SCI"))
****************************************************************************/
class science_report : public QWidget {
  Q_OBJECT

  QComboBox *goal_combo;
  QComboBox *researching_combo;
  QGridLayout *sci_layout;
  progress_bar *progress;
  QLabel *info_label;
  QLabel *progress_label;
  QList<qlist_item> *curr_list;
  QList<qlist_item> *goal_list;
  research_diagram *res_diag;
  QScrollArea *scroll;

public:
  science_report();
  ~science_report();
  void update_report();
  void init(bool raise);
  void redraw();
  void reset_tree();

private:
  void update_reqtree();
  int index;

private slots:
  void current_tech_changed(int index);
  void goal_tech_changed(int index);
};

void popdown_science_report();
bool comp_less_than(const qlist_item &q1, const qlist_item &q2);

#endif /* FC__REPODLGS_H */