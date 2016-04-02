/****************************************************************************
**
** Copyright (C) 2012 IPO.Plan GmbH
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qobjectlistmodeladapter.h"
#include "qobjectlistmodel.h"

void QObjectListModelAdapter::setListModel(QObjectListModel* list)
{
    if (m_list == list)
        return;
    if (m_enabled && m_list)
        disconnectAndClear();
    m_list = list;
    if (m_enabled && m_list)
        connectAndRefill();
    emit listModelChanged();
}

void QObjectListModelAdapter::connectAndRefill()
{
    loadAll();
    if (m_watchInsertions)
        connectInsertionSignals();
    if (m_watchRemovals)
        connectRemovalSignals();
}

void QObjectListModelAdapter::disconnectAndClear()
{
    m_list->disconnect(this);
    removeAll();
}

void QObjectListModelAdapter::loadAll()
{
    int n = m_list->count();
    for (int i = 0; i != n; ++i)
        insertFor(i, m_list->at(i), i + 1);
}

void QObjectListModelAdapter::removeAll()
{
    int n = m_list->count();
    for (int i = n; i != 0; --i)
        removeFor(i - 1, m_list->at(i - 1), i);
}

void QObjectListModelAdapter::listInsert(const QModelIndex& parent, int start, int end)
{
    Q_UNUSED(parent)
    for (int i = start; i <= end; ++i)
        insertFor(i, m_list->at(i), m_list->count());
}

void QObjectListModelAdapter::listRemove(const QModelIndex& parent, int start, int end)
{
    Q_UNUSED(parent)
    for (int i = start; i <= end; ++i)
        removeFor(i, m_list->at(i), m_list->count());
}

QObjectListModelAdapter::QObjectListModelAdapter(
    QObjectListModel* listModel, bool enabled, QObject* parent /*= NULL*/)
    : QObject(parent)
    , m_list(listModel)
    , m_enabled(enabled)
    , m_watchInsertions(true)
    , m_watchRemovals(true)
{
    if (m_list && m_enabled)
        connectAndRefill();
}

QObjectListModelAdapter::~QObjectListModelAdapter()
{
    if (m_list && m_enabled)
        removeAll();
}

void QObjectListModelAdapter::setEnabled(bool const value)
{
    if (m_enabled == value)
        return;
    m_enabled = value;
    if (m_list) {
        if (value) {
            connectAndRefill();
        } else {
            disconnectAndClear();
        }
    }
    emit enabledChanged();
}

void QObjectListModelAdapter::setWatchInsertions(bool val)
{
    if (m_watchInsertions == val)
        return;
    m_watchInsertions = val;
    if (m_list) {
        if (m_enabled) {
            connectInsertionSignals();
        } else {
            disconnectInsertionSignals();
        }
    }
    emit watchInsertionsChanged();
}

void QObjectListModelAdapter::setWatchRemovals(bool val)
{
    if (m_watchRemovals == val)
        return;
    m_watchRemovals = val;
    if (m_list) {
        if (m_enabled) {
            connectRemovalSignals();
        } else {
            disconnectRemovalSignals();
        }
    }
    emit watchRemovalsChanged();
}

void QObjectListModelAdapter::connectInsertionSignals() const
{
    connect(m_list, SIGNAL(modelReset()), this, SLOT(loadAll()));
    connect(m_list, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this,
        SLOT(listInsert(const QModelIndex&, int, int)));
}

void QObjectListModelAdapter::disconnectInsertionSignals() const
{
    disconnect(m_list, SIGNAL(modelReset()), this, SLOT(loadAll()));
    disconnect(m_list, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this,
        SLOT(listInsert(const QModelIndex&, int, int)));
}

void QObjectListModelAdapter::connectRemovalSignals() const
{
    connect(m_list, SIGNAL(modelAboutToBeReset()), this, SLOT(removeAll()));
    connect(m_list, SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)), this,
        SLOT(listRemove(const QModelIndex&, int, int)));
}

void QObjectListModelAdapter::disconnectRemovalSignals() const
{
    disconnect(m_list, SIGNAL(modelAboutToBeReset()), this, SLOT(removeAll()));
    disconnect(m_list, SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)), this,
        SLOT(listRemove(const QModelIndex&, int, int)));
}
