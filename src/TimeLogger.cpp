/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file TimeLogger.cpp
 *
 *  Copyright 2017 Sebastien Fourey
 *
 *  This file is part of G'MIC-Qt, a generic plug-in for raster graphics
 *  editors, offering hundreds of filters thanks to the underlying G'MIC
 *  image processing framework.
 *
 *  gmic_qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gmic_qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gmic_qt.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "TimeLogger.h"
#include <QDebug>
#include <QString>
#include <cassert>
#include "CImg.h"
#include "Common.h"
#include "Utils.h"

std::unique_ptr<TimeLogger> TimeLogger::_instance = nullptr;

TimeLogger::TimeLogger()
{
  char filename[1024];
  sprintf(filename, "%stiming_log.txt", GmicQt::path_rc(true).toLocal8Bit().constData());
  _file = fopen(filename, "w");
  assert(_file);
}

TimeLogger::~TimeLogger()
{
  ENTERING;
  fclose(_file);
}

TimeLogger * TimeLogger::getInstance()
{
  if (!_instance) {
    _instance = std::unique_ptr<TimeLogger>(new TimeLogger);
  }
  return _instance.get();
}

void TimeLogger::step(const char * function, int line, const char * filename)
{
  static cimg_ulong first = 0;
  static cimg_ulong last = 0;
  static unsigned int count = 0;
  const cimg_ulong now = cimg_library::cimg::time();
  if (!last) {
    last = first = now;
  }
  double elapsed = (now - last) / 1000.0;
  const double total = (now - first) / 1000.0;
  printf("%02d @%2.3f +%2.3f %s <%s:%d>\n", count++, total, elapsed, function, filename, line);
  fprintf(_file, "%02d @%2.3f +%2.3f %s <%s:%d>\n", count++, total, elapsed, function, filename, line);
  last = now;
}
