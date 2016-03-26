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

#pragma once

#include <QObject>
#include <QModelIndex>

class QObjectListModel;

class QObjectListModelAdapter : public QObject {
    Q_OBJECT
    Q_PROPERTY(QObjectListModel * listModel READ listModel NOTIFY listModelChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
public:
    QObjectListModelAdapter(QObjectListModel * listModel, bool enabled = true, QObject * parent = NULL);
    ~QObjectListModelAdapter();
    QObjectListModel* listModel() const { return m_list; }
    void setListModel(QObjectListModel* listModel );
    bool enabled() const { return m_enabled; }
    void setEnabled( bool const value );
    bool watchInsertions() const { return m_watchInsertions; }
    void setWatchInsertions(bool val);
    bool watchRemovals() const { return m_watchRemovals; }
    void setWatchRemovals(bool val);
signals:
    void listModelChanged();
    void enabledChanged();
    void watchInsertionsChanged();
    void watchRemovalsChanged();
protected slots:
    void listInsert(const QModelIndex & parent, int start, int end);
    void listRemove(const QModelIndex & parent, int start, int end);
    void loadAll();
    void removeAll();
protected:
    void connectAndRefill();
    void disconnectAndClear();

    virtual void insertFor(int index, QObject * obj, int listCount) = 0;
    virtual void removeFor(int index, QObject * obj, int listCount) = 0;

private:
    void connectInsertionSignals() const;
    void connectRemovalSignals() const;
    void disconnectInsertionSignals() const;
    void disconnectRemovalSignals() const;
    QObjectListModel *m_list;
    bool m_enabled;
    bool m_watchInsertions;
    bool m_watchRemovals;
};

class QObjectListModelAdapterSignal : public QObjectListModelAdapter {
    Q_OBJECT
public:
    QObjectListModelAdapterSignal(QObjectListModel * listModel, bool enabled = true, QObject * parent = NULL)
        : QObjectListModelAdapter(listModel, enabled, parent) {}
signals:
    void insert(int index, QObject * obj);
    void remove(int index, QObject * obj);

protected:
    virtual void insertFor(int index, QObject * obj, int listCount) override
    {
        Q_UNUSED(listCount)
        emit insert(index, obj);
    }
    virtual void removeFor(int index, QObject * obj, int listCount) override
    {
        Q_UNUSED(listCount)
        emit remove(index, obj);
    }
};
