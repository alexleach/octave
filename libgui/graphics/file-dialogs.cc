////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2011-2020 The Octave Project Developers
//
// See the file COPYRIGHT.md in the top-level directory of this
// distribution or <https://octave.org/copyright/>.
//
// This file is part of Octave.
//
// Octave is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Octave is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octave; see the file COPYING.  If not, see
// <https://www.gnu.org/licenses/>.
//
// Provides thread-safe access to QFileDialog static methods.
//
////////////////////////////////////////////////////////////////////////

#include "file-dialogs.h"

#include <QApplication>
#include <QThread>

namespace QtHandles {
  namespace Utils {

FileDialog::FileDialog(QObject *parent)
  : QObject (parent)
{
  moveToThread (qApp->thread());
}

QString
FileDialog::getOpenFileName(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  const QString &filter,
  QString *selectedFilter,
  QFileDialog::Options options)
{
    QString fileName;

    if (QThread::currentThread() != qApp->thread())
      {
      QMetaObject::invokeMethod(this, "_getOpenFileName",
                                Qt::BlockingQueuedConnection,
                                Q_RETURN_ARG(QString, fileName),
                                Q_ARG(QWidget*, parent),
                                Q_ARG(QString, caption),
                                Q_ARG(QString, dir),
                                Q_ARG(QString, filter),
                                Q_ARG(QString *, selectedFilter),
                                Q_ARG(QFileDialog::Options, options));
      }
    else
      {
      fileName = _getOpenFileName(parent, caption, dir, filter, selectedFilter, options);
      }
    return fileName;
}

QStringList
FileDialog::getOpenFileNames(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  const QString &filter,
  QString *selectedFilter,
  QFileDialog::Options options)
{
    QStringList fileNames;

    if (QThread::currentThread() != qApp->thread())
      {
      QMetaObject::invokeMethod(this, "_getOpenFileNames",
                                Qt::BlockingQueuedConnection,
                                Q_RETURN_ARG(QStringList, fileNames),
                                Q_ARG(QWidget*, parent),
                                Q_ARG(QString, caption),
                                Q_ARG(QString, dir),
                                Q_ARG(QString, filter),
                                Q_ARG(QString *, selectedFilter),
                                Q_ARG(QFileDialog::Options, options) );
      }
    else
      {
      fileNames = _getOpenFileNames(parent, caption, dir, filter, selectedFilter, options);
      }
    return fileNames;
}

QString
FileDialog::getExistingDirectory(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  QFileDialog::Options options)
{
    QString dirName;
    if (QThread::currentThread() != qApp->thread())
      {
      QMetaObject::invokeMethod(this, "_getExistingDirectory",
                                Qt::BlockingQueuedConnection,
                                Q_RETURN_ARG(QString, dirName),
                                Q_ARG(QWidget*, parent),
                                Q_ARG(QString, caption),
                                Q_ARG(QString, dir),
                                Q_ARG(QFileDialog::Options, options) );
      }
    else
      {
      dirName = _getExistingDirectory(parent, caption, dir, options);
      }
    return dirName;
}

QString
FileDialog::getSaveFileName(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  const QString &filter,
  QString *selectedFilter,
  QFileDialog::Options options)
{
    QString fileName;

    if (QThread::currentThread() != qApp->thread())
      {
      QMetaObject::invokeMethod(this, "_getSaveFileName",
                                Qt::BlockingQueuedConnection,
                                Q_RETURN_ARG(QString, fileName),
                                Q_ARG(QWidget*, parent),
                                Q_ARG(QString, caption),
                                Q_ARG(QString, dir),
                                Q_ARG(QString, filter),
                                Q_ARG(QString *, selectedFilter),
                                Q_ARG(QFileDialog::Options, options) );
      }
    else
      {
      fileName = _getSaveFileName(parent, caption, dir, filter, selectedFilter, options);
      }
    return fileName;
}

QString
FileDialog::_getOpenFileName(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  const QString &filter,
  QString *selectedFilter,
  QFileDialog::Options options)
{
  return QFileDialog::getOpenFileName(parent, caption, dir, filter, selectedFilter, options);
}

QStringList
FileDialog::_getOpenFileNames(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  const QString &filter,
  QString *selectedFilter,
  QFileDialog::Options options)
{
  return QFileDialog::getOpenFileNames(parent, caption, dir, filter, selectedFilter, options);
}

QString
FileDialog::_getExistingDirectory(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  QFileDialog::Options options)
{
  return QFileDialog::getExistingDirectory( parent, caption, dir, options );
}

QString
FileDialog::_getSaveFileName(
  QWidget *parent,
  const QString &caption,
  const QString &dir,
  const QString &filter,
  QString *selectedFilter,
  QFileDialog::Options options)
{
  return QFileDialog::getSaveFileName( parent, caption, dir, filter, selectedFilter, options );
}

  } // namespace Utils
} // namespace QtHandles
