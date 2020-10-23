/*####################################################################
###      ***                                     ***               ###
###     *****          ⒻⓇⒺⒺⒸⒾⓋ ②①         *****              ###
###      ***                                     ***               ###
#####################################################################*/

// Qt
#include <QApplication>
#include <QComboBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QImage>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRadioButton>
#include <QRect>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QtMath>

// utility
#include "astring.h"
#include "fcintl.h"

// common
#include "actions.h"
#include "city.h"
#include "game.h"
#include "government.h"
#include "improvement.h"
#include "movement.h"
#include "nation.h"
#include "research.h"

// client
#include "audio.h"
#include "chatline_common.h"
#include "client_main.h"
#include "control.h"
#include "dialogs_g.h"
#include "helpdata.h"
#include "mapview_common.h"
#include "packhand.h"
#include "text.h"
#include "tilespec.h"

// gui-qt
#include "dialogs.h"
#include "notifyreport.h"
#include "fc_client.h"
#include "fonts.h"
#include "hudwidget.h"
#include "mapview.h"
#include "qtg_cxxside.h"
#include "sprite.h"

/***********************************************************************/ /**
   Constructor for notify dialog
 ***************************************************************************/
notify_dialog::notify_dialog(const char *caption, const char *headline,
                             const char *lines, QWidget *parent)
    : fcwidget()
{
  int x, y;
  QString qlines;

  setAttribute(Qt::WA_DeleteOnClose);
  setCursor(Qt::ArrowCursor);
  setParent(parent);
  setFrameStyle(QFrame::Box);
  cw = new close_widget(this);
  cw->put_to_corner();

  qcaption = QString(caption);
  qheadline = QString(headline);
  qlines = QString(lines);
  qlist = qlines.split("\n");
  small_font = *fc_font::instance()->get_font("gui_qt_font_notify_label");
  x = 0;
  y = 0;
  calc_size(x, y);
  resize(x, y);
  gui()->mapview_wdg->find_place(gui()->mapview_wdg->width() - x - 4, 4, x,
                                 y, x, y, 0);
  move(x, y);
  was_destroyed = false;
}

/***********************************************************************/ /**
   Starts new copy of notify dialog and closes current one
 ***************************************************************************/
void notify_dialog::restart()
{
  QString s, q;
  int i;
  QByteArray capt_bytes;
  QByteArray hl_bytes;
  QByteArray qb_bytes;

  for (i = 0; i < qlist.size(); ++i) {
    s = qlist.at(i);
    q = q + s;
    if (i < qlist.size() - 1) {
      q = q + QChar('\n');
    }
  }
  capt_bytes = qcaption.toLocal8Bit();
  hl_bytes = qheadline.toLocal8Bit();
  qb_bytes = q.toLocal8Bit();
  popup_notify_dialog(capt_bytes.data(), hl_bytes.data(), qb_bytes.data());
  close();
  destroy();
}

/***********************************************************************/ /**
   Calculates size of notify dialog
 ***************************************************************************/
void notify_dialog::calc_size(int &x, int &y)
{
  QFontMetrics fm(small_font);
  int i;
  QStringList str_list;

  str_list = qlist;
  str_list << qcaption << qheadline;

  for (i = 0; i < str_list.count(); i++) {
    x = qMax(x, fm.horizontalAdvance(str_list.at(i)));
    y = y + 3 + fm.height();
  }
  x = x + 15;
}

/***********************************************************************/ /**
   Paint Event for notify dialog
 ***************************************************************************/
void notify_dialog::paintEvent(QPaintEvent *paint_event)
{
  QPainter painter(this);
  QPen pen;
  QFontMetrics fm(small_font);
  int i;

  pen.setWidth(1);
  pen.setColor(palette().color(QPalette::Text));
  painter.setFont(small_font);
  painter.setPen(pen);
  painter.drawText(10, fm.height() + 3, qcaption);
  painter.drawText(10, 2 * fm.height() + 6, qheadline);
  for (i = 0; i < qlist.count(); i++) {
    painter.drawText(10, 3 + (fm.height() + 3) * (i + 3), qlist[i]);
  }
  cw->put_to_corner();
}

/***********************************************************************/ /**
   Called when mouse button was pressed, just to close on right click
 ***************************************************************************/
void notify_dialog::mousePressEvent(QMouseEvent *event)
{
  cursor = event->globalPos() - geometry().topLeft();
  if (event->button() == Qt::RightButton) {
    was_destroyed = true;
    close();
  }
}

/***********************************************************************/ /**
   Called when mouse button was pressed and moving around
 ***************************************************************************/
void notify_dialog::mouseMoveEvent(QMouseEvent *event)
{
  move(event->globalPos() - cursor);
  setCursor(Qt::SizeAllCursor);
}

/***********************************************************************/ /**
   Called when mouse button unpressed. Restores cursor.
 ***************************************************************************/
void notify_dialog::mouseReleaseEvent(QMouseEvent *event)
{
  setCursor(Qt::ArrowCursor);
}

/***********************************************************************/ /**
   Called when close button was pressed
 ***************************************************************************/
void notify_dialog::update_menu()
{
  was_destroyed = true;
  destroy();
}

/***********************************************************************/ /**
   Destructor for notify dialog, notice that somehow object is not destroyed
   immediately, so it can be still visible for parent, check boolean
   was_destroyed if u suspect it could not be destroyed yet
 ***************************************************************************/
notify_dialog::~notify_dialog()
{
  was_destroyed = true;
  destroy();
}


/***********************************************************************/ /**
   Restarts all notify dialogs
 ***************************************************************************/
void restart_notify_reports()
{
  QList<notify_dialog *> nd_list;
  int i;

  nd_list = gui()->mapview_wdg->findChildren<notify_dialog *>();
  for (i = 0; i < nd_list.count(); i++) {
    nd_list[i]->restart();
    delete nd_list[i];
  }
}