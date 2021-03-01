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

#include <QFileDialog>

namespace QtHandles {
  namespace Utils {

class FileDialog : public QObject
  {
  Q_OBJECT

public:
  FileDialog(QObject *parent = nullptr);

  // FIXME: Is there a tidier way of doing this?
  QString
  getOpenFileName( QWidget *parent = nullptr,
                   const QString &caption = QString(),
                   const QString &dir = QString(),
                   const QString &filter = QString(),
                   QString *selectedFilter = nullptr,
                   QFileDialog::Options options = QFileDialog::Options() );

  QStringList
  getOpenFileNames( QWidget *parent = nullptr,
                    const QString &caption = QString(),
                    const QString &dir = QString(),
                    const QString &filter = QString(),
                    QString *selectedFilter = nullptr,
                    QFileDialog::Options options = QFileDialog::Options() );

  QString
  getExistingDirectory( QWidget *parent = nullptr,
                        const QString &caption = QString(),
                        const QString &dir = QString(),
                        QFileDialog::Options options = QFileDialog::ShowDirsOnly );

  QString
  getSaveFileName( QWidget *parent = nullptr,
                   const QString &caption = QString(),
                   const QString &dir = QString(),
                   const QString &filter = QString(),
                   QString *selectedFilter = nullptr,
                   QFileDialog::Options options = QFileDialog::Options());

private:
  Q_INVOKABLE QString
  _getOpenFileName( QWidget *parent,
                    const QString &caption = QString(),
                    const QString &dir = QString(),
                    const QString &filter = QString(),
                    QString *selectedFilter = nullptr,
                    QFileDialog::Options options = QFileDialog::Options() );

  Q_INVOKABLE QStringList
  _getOpenFileNames( QWidget *parent,
                     const QString &caption = QString(),
                     const QString &dir = QString(),
                     const QString &filter = QString(),
                     QString *selectedFilter = nullptr,
                     QFileDialog::Options options = QFileDialog::Options() );

  Q_INVOKABLE QString
  _getExistingDirectory( QWidget *parent,
                         const QString &caption = QString(),
                         const QString &dir = QString(),
                         QFileDialog::Options options = QFileDialog::ShowDirsOnly);

  Q_INVOKABLE QString
  _getSaveFileName( QWidget *parent = nullptr,
                    const QString &caption = QString(),
                    const QString &dir = QString(),
                    const QString &filter = QString(),
                    QString *selectedFilter = nullptr,
                    QFileDialog::Options options = QFileDialog::Options() );

  };

  } // namespace Utils
} // namespace QtHandles
